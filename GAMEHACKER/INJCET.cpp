#include "pch.h"
#include "INJCET.h"
#include <fstream>

typedef void(*_Init)();
//注入的函数
void _stdcall INJECTCode()
{
	
	unsigned       address = 0xCCCCCCCC;
	PREMOTE_DATA   p = (PREMOTE_DATA)address;
	unsigned int hMod = p->f_LoadLibrary(p->dllName);
	
	//_Init init=(_Init)p->f_GetProcAddress((HMODULE)hMod, p->Init);
	

	

	
	unsigned dEntry = p->EntryPoint;
	char* entryCode = (char*)p->EntryPoint;
	entryCode[0] = p->oldCode[0];
	entryCode[1] = p->oldCode[1];
	entryCode[2] = p->oldCode[2];
	entryCode[3] = p->oldCode[3];
	entryCode[4] = p->oldCode[4];
	_asm
	{
		mov eax,dEntry
		jmp eax
	}
}

DWORD _stdcall RemoteThreadProc(PREMOTE_DATA p)
{
	unsigned base = p->f_GetModuleHandleA(0);
	DWORD dRet;
	p->EntryPoint += base; //400000+25D20
	p->f_VirtualProtect((LPVOID)p->EntryPoint, 0x5, PAGE_EXECUTE_READWRITE, &dRet);
	
	

	char* entryCode = (char*)p->EntryPoint;
	//保存入口点代码
	p->oldCode[0] = entryCode[0];
	p->oldCode[1] = entryCode[1];
	p->oldCode[2] = entryCode[2];
	p->oldCode[3] = entryCode[3];
	p->oldCode[4] = entryCode[4];

	int* entryDis = (int*)(p->EntryPoint + 1);

	entryCode[0] = 0xE9;//JMP
	int  Distance = p->HOOKFunction - p->EntryPoint - 5;
	entryDis[0]   = Distance;
	return 1;
}


BOOL INJCET::StartProcess(const wchar_t* GameExe, 
	const wchar_t* GamePath, 
	wchar_t* GameCmds, PROCESS_INFORMATION* LPinfo,
	bool  Pause
)
{
	
	STARTUPINFO si{};
	si.cb = sizeof(si);

	DWORD dFlags = 0; 
	if(Pause)dFlags= CREATE_SUSPENDED;

	return CreateProcess(GameExe,
		GameCmds,
		NULL, NULL, true,
		dFlags,
		NULL,
		GamePath,
		&si,
		LPinfo
	);
}


void* INJCET::ImageLoad(const wchar_t* filename)
{
	std::ifstream streamReader(filename, std::ios::binary); //以二进制的形式读取文件
	streamReader.seekg(0, std::ios::end);//跳转到尾部
	unsigned filesize = streamReader.tellg();//得到文件大小
	char* _data = new char[filesize];
	streamReader.seekg(0, std::ios::beg);//跳转到开始
	streamReader.read(_data, filesize);
	streamReader.close();
	return _data;
}

void INJCET::UnloadImage(void* _data)
{
	delete[] _data; //
}

DWORD INJCET::GetEntryPoint(const wchar_t* filename)
{

	
	void* image = ImageLoad(filename);
	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)image;
	unsigned PEAddress = dosHeader->e_lfanew + (unsigned)image;

	IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)PEAddress;
	DWORD dEntryPoint = ntHeader->OptionalHeader.AddressOfEntryPoint;

	UnloadImage(image);

	return dEntryPoint;
}

BOOL INJCET::CreateRemoteData(HANDLE hProcess, const wchar_t* GameExe, const wchar_t* dllName)
{
	LPVOID adrRemote    = VirtualAllocEx(hProcess, 0, 0x3000, MEM_COMMIT, PAGE_EXECUTE_READWRITE); //分配内存
	SIZE_T lwt;
	LPVOID adrRemoteData = (LPVOID)((unsigned)adrRemote + 0x2000);
	LPVOID adrRemoteProc = (LPVOID)((unsigned)adrRemote + 0x500);
	//写入远程代码
	_REMOTE_DATA remoteData{};
	remoteData.EntryPoint = (unsigned)GetEntryPoint(GameExe);

	//CString wTxt;
	//wTxt.Format(L"%X", remoteData.EntryPoint);
	//AfxMessageBox(wTxt);

	remoteData.HOOKFunction = (unsigned)adrRemote;//
	CodeRemoteData(&remoteData, dllName);
	WriteProcessMemory(hProcess, adrRemoteData, &remoteData, sizeof(remoteData), &lwt);
	//修正远程代码
	char _code[0x200];
	memcpy(_code, INJECTCode, sizeof(_code));
	for (int i = 0; i < 0x100; i++)
	{
		unsigned* pcode = (unsigned*)(&_code[i]);
		if (pcode[0] == 0xCCCCCCCC)
		{
			pcode[0] = (unsigned)adrRemoteData;
			break;
		}
	}
	
	//0-0x200 注入代码
	//0-0x500 远程代码区
	//0x2000  远程数据
	WriteProcessMemory(hProcess, adrRemote, _code, 0x200, &lwt);
	WriteProcessMemory(hProcess, adrRemoteProc, RemoteThreadProc, 0x200, &lwt);
	
	
	//wTxt.Format(L"%X %X", adrRemoteProc,adrRemote);
	//AfxMessageBox(wTxt);

	DWORD dwThreadId;
	HANDLE remoteHdl = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)adrRemoteProc, adrRemoteData, 0, &dwThreadId);
	WaitForSingleObject(remoteHdl, INFINITE);
	CloseHandle(remoteHdl);
	
	/*
	远程线程注入 测试
	DWORD dwThreadId;
	HANDLE remoteHdl = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)adrRemote, NULL, 0, &dwThreadId);
	WaitForSingleObject(remoteHdl, INFINITE);
	CloseHandle(remoteHdl);
	*/

	return TRUE;
}

void INJCET::CodeRemoteData(PREMOTE_DATA _data,const wchar_t* dllName)
{
	short lenth;
	for (lenth = 0; dllName[lenth]; lenth++);
	HMODULE hKernel = LoadLibrary(_T("kernel32.dll"));
	_data->f_LoadLibrary = (_LoadLibrary)GetProcAddress(hKernel,"LoadLibraryW");
	_data->f_GetModuleHandleA = (_GetModuleHandleA)GetProcAddress(hKernel, "GetModuleHandleA");
	_data->f_VirtualProtect = (_VirtualProtect)GetProcAddress(hKernel, "VirtualProtect");
	
	memcpy(_data->dllName, dllName, (lenth+1)*2);

	

}
