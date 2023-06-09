// CWndINJ.cpp: 实现文件
//

#include "pch.h"
#include "GAMEHACKER.h"
#include "CWndINJ.h"
#include "afxdialogex.h"

#include <ImageHlp.h>
#include <fstream>
#pragma comment(lib,"imagehlp.lib")

#include <tlhelp32.h>
// CWndINJ 对话框

IMPLEMENT_DYNAMIC(CWndINJ, CDialogEx)



void _stdcall INJECTCode();

CWndINJ::CWndINJ(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_0, pParent)
	, B_INJCET(FALSE)
	, B_PAUSE(FALSE)
	, B_DEBUG(FALSE)
{

}

CWndINJ::~CWndINJ()
{
}

BOOL CWndINJ::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	/*
	*/

	//ExeLst
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(ExeLst.m_hWnd, GWL_STYLE);
	lStyle|=LVS_REPORT;
	SetWindowLongPtr(ExeLst.m_hWnd, GWL_STYLE, lStyle);
	DWORD dStyle=ExeLst.GetExtendedStyle();
	dStyle |= LVS_EX_FULLROWSELECT;
	dStyle |= LVS_EX_GRIDLINES;
	ExeLst.SetExtendedStyle(dStyle);

	ExeLst.InsertColumn(0, L"名称", 0, 100);
	ExeLst.InsertColumn(1, L"可执行文件", 0, 200);
	ExeLst.InsertColumn(2, L"文件夹", 0, 200);
	ExeLst.InsertColumn(3, L"命令行", 0, 200);
	ExeLst.InsertColumn(4, L"注入模块", 0, 200);
	//加载内容
	
	return TRUE;
}

void CWndINJ::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, ExeLst);
	DDX_Check(pDX, IDC_CHECK1, B_INJCET);
	DDX_Check(pDX, IDC_CHECK2, B_PAUSE);
	DDX_Check(pDX, IDC_CHECK3, B_DEBUG);
}


BEGIN_MESSAGE_MAP(CWndINJ, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndINJ::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CWndINJ::OnNMDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CWndINJ::OnLvnItemchangedList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CWndINJ::OnNMRClickList1)
	ON_COMMAND(ID_32771, &CWndINJ::OnMenuSet)
	ON_COMMAND(ID_32772, &CWndINJ::OnMenuDel)
	ON_BN_CLICKED(IDC_CHECK1, &CWndINJ::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK3, &CWndINJ::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK2, &CWndINJ::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CWndINJ 消息处理程序

DWORD DllSize(HMODULE hDll)
{
	IMAGE_DOS_HEADER* pDosHeader;
	IMAGE_NT_HEADERS* pNtHeader;
	IMAGE_OPTIONAL_HEADER* pOptionalHeader;
	pDosHeader = (IMAGE_DOS_HEADER*)hDll;
	pNtHeader = (IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)hDll);
	pOptionalHeader = (IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;
	return pOptionalHeader->SizeOfImage;
}

void LockAllModules()
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 me = { sizeof(me) };

		BOOL fOk = Module32First(hSnapshot, &me);
		for (fOk = Module32Next(hSnapshot, &me); fOk; fOk = Module32Next(hSnapshot, &me))
		{
			//跳过第一个(自身)  
			CString wInfo;
			wInfo.Format(_T("%s"), me.szModule);
			wInfo.MakeLower();
			if (wInfo != _T("dlls.dll"))LoadLibrary(me.szModule);
		}
	}
}

BOOL CopycatAndHide(HMODULE hDll)
{
	// 整体思路：先把DLL加载到当前进程，然后将该加载的DLL再备份到当前进程空间；  
	// 接下来该DLL再Free了，此时进程再访问该DLL的话会出错；  
	// Free后，再把预先备份的DLL数据还原，而且还原的数据地址是原先DLL加载的地址  
	// 如此，进程内再调用该DLL的话，由于数据完整，一切OK  

	DWORD g_dwImageSize = 0;
	VOID* g_lpNewImage = NULL;

	IMAGE_DOS_HEADER* pDosHeader;
	IMAGE_NT_HEADERS* pNtHeader;
	IMAGE_OPTIONAL_HEADER* pOptionalHeader;
	LPVOID lpBackMem = 0;
	DWORD dwOldProtect;
	DWORD dwCount = 30;


	pDosHeader = (IMAGE_DOS_HEADER*)hDll;
	pNtHeader = (IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)hDll);
	pOptionalHeader = (IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;

	LockAllModules();

	// 找一块内存把需要隐藏而且已经加载到内存的DLL备份  
	// SizeOfImage，4个字节，表示程序调入后占用内存大小（字节），等于所有段的长度之和。  
	lpBackMem = VirtualAlloc(0, pOptionalHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!lpBackMem)
		return FALSE;
	if (!VirtualProtect((LPVOID)hDll, pOptionalHeader->SizeOfImage, PAGE_EXECUTE_READWRITE, &dwOldProtect))
		return FALSE;

	g_dwImageSize = pOptionalHeader->SizeOfImage;
	memcpy(lpBackMem, (LPVOID)hDll, g_dwImageSize);
	// 抹掉PE头  
	//memset(lpBackMem, 0, 0x200);
	*((PBYTE)hDll + pOptionalHeader->AddressOfEntryPoint) = (BYTE)0xc3;

	//  DWORD dwRet =0;  
	// Free掉DLL  
	do
	{
		dwCount--;
	} while (FreeLibrary(hDll) && dwCount);

	// 把备份的DLL数据还原回来，使得预先引用该DLL的程序能够继续正常运行  
	g_lpNewImage = VirtualAlloc((LPVOID)hDll, g_dwImageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (g_lpNewImage != (LPVOID)hDll)
		return FALSE;

	memcpy(g_lpNewImage, lpBackMem, g_dwImageSize);
	VirtualFree(lpBackMem, 0, MEM_RELEASE);

	return TRUE;
}


void CWndINJ::OnBnClickedButton1()
{
	/*
	auto hMod = LoadLibrary(L"Dlls.dll"); // +1
	
	//if(hMod)FreeLibrary(hMod); //
	PIMAGE_DOS_HEADER  PDosHeader = (PIMAGE_DOS_HEADER)hMod;
	PIMAGE_NT_HEADERS  PNTHead = (PIMAGE_NT_HEADERS)((unsigned)hMod + PDosHeader->e_lfanew);
	PIMAGE_OPTIONAL_HEADER POPHeader = (PIMAGE_OPTIONAL_HEADER)(&PNTHead->OptionalHeader);
	DWORD  dImageSize = POPHeader->SizeOfImage;
	DWORD  dOld;
	LPVOID dllCode = VirtualAlloc(0, dImageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	VirtualProtect(hMod, dImageSize, PAGE_EXECUTE_READWRITE, &dOld);
	memcpy(dllCode, hMod, dImageSize);//DLL的代码保存下来了
	FreeLibrary(hMod);

	LPVOID dllCodeNew = VirtualAlloc(hMod, dImageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(dllCodeNew, dllCode, dImageSize);

	//MessageBoxA() mfcdll -> mfc free...

	VirtualFree(dllCode, 0, MEM_RELEASE);
	*/
	//CString wTxt;
	//wTxt.Format(L"%X %X",dllCodeNew,dImageSize);
	//OutputDebugString(wTxt);
	
	wndAddGame.Init(this);
	wndAddGame.DoModal();

	// TODO: 在此添加控件通知处理程序代码
	//test lst
	//ExeLst.InsertItem(0, L"DNF");
	//ExeLst.SetItemText(0, 1, L"dlls.dll");
    /*
	STARTUPINFO si{};
	si.cb = sizeof(si);
	PROCESS_INFORMATION prinfo{};
	//prinfo.

	CreateProcess(L"F:\\Games\\JX2\\Sword2.exe",
		L"\"F:\\Games\\JX2\\Sword2.exe\" -windows",
		NULL, NULL, true,
		CREATE_SUSPENDED,
		NULL,
		L"F:\\Games\\JX2\\",
		&si,
		&prinfo
	);

	
	ResumeThread(prinfo.hThread);
	
	*/
}

void CWndINJ::Init(CString& _AppPath)
{
	AppPath = _AppPath;
	GamesIni.Format(L"%s\\set\\Games.ini", AppPath);
	LoadGames();
}

void CWndINJ::AddGame(CString& _GameName, CString& _GameExe, CString& _GamePath, CString& _GameCmds, CString& _dlls)
{
	//AfxMessageBox(_GameName);
	int count = GetPrivateProfileIntW(L"main", L"count", 0, GamesIni);
	

	CString key;
	key.Format(L"count_%d", count);
	WritePrivateProfileStringW(key, L"GameName", _GameName, GamesIni);
	WritePrivateProfileStringW(key, L"GameExe", _GameExe, GamesIni);
	WritePrivateProfileStringW(key, L"GamePath", _GamePath, GamesIni);
	WritePrivateProfileStringW(key, L"GameCmds", _GameCmds, GamesIni);
	WritePrivateProfileStringW(key, L"GameDlls", _dlls, GamesIni);
	count++;
	CString wCount;
	wCount.Format(L"%d", count);
	WritePrivateProfileStringW(L"main", L"count", wCount, GamesIni);
	
	int icount = ExeLst.GetItemCount();
	ExeLst.InsertItem(icount, _GameName);
	ExeLst.SetItemText(icount, 1, _GameExe);
	ExeLst.SetItemText(icount, 2, _GamePath);
	ExeLst.SetItemText(icount, 3, _GameCmds);
	ExeLst.SetItemText(icount, 4, _dlls);
}

void CWndINJ::SaveGame(CString& _GameName, CString& _GameExe, CString& _GamePath, CString& _GameCmds, CString& _dlls, int index)
{
	CString key;
	key.Format(L"count_%d", index);
	WritePrivateProfileStringW(key, L"GameName", _GameName, GamesIni);
	WritePrivateProfileStringW(key, L"GameExe", _GameExe, GamesIni);
	WritePrivateProfileStringW(key, L"GamePath", _GamePath, GamesIni);
	WritePrivateProfileStringW(key, L"GameCmds", _GameCmds, GamesIni);
	WritePrivateProfileStringW(key, L"GameDlls", _dlls, GamesIni);

	ExeLst.SetItemText(index, 0,_GameName);
	ExeLst.SetItemText(index, 1, _GameExe);
	ExeLst.SetItemText(index, 2, _GamePath);
	ExeLst.SetItemText(index, 3, _GameCmds);
	ExeLst.SetItemText(index, 4, _dlls);

}

void CWndINJ::LoadGames()
{
	int count = GetPrivateProfileIntW(L"main", L"count", 0, GamesIni);
	for (int i = 0; i < count; i++)
	{
		CString GameName, GameExe, GamePath, GameCmds, GameDlls,_AppName;
		_AppName.Format(L"count_%d", i);

		wchar_t  wRead[0xFF]; //重点
		GetPrivateProfileStringW(_AppName,L"GameName",L"", wRead,0xFF,GamesIni);
		GameName.Format(L"%s", wRead);
		GetPrivateProfileStringW(_AppName, L"GameExe", L"", wRead, 0xFF, GamesIni);
		GameExe.Format(L"%s", wRead);
		GetPrivateProfileStringW(_AppName, L"GamePath", L"", wRead, 0xFF, GamesIni);
		GamePath.Format(L"%s", wRead);
		GetPrivateProfileStringW(_AppName, L"GameCmds", L"", wRead, 0xFF, GamesIni);
		GameCmds.Format(L"%s", wRead);
		GetPrivateProfileStringW(_AppName, L"GameDlls", L"", wRead, 0xFF, GamesIni);
		GameDlls.Format(L"%s", wRead);

		ExeLst.InsertItem(i, GameName);
		ExeLst.SetItemText(i, 1, GameExe);
		ExeLst.SetItemText(i, 2, GamePath);
		ExeLst.SetItemText(i, 3, GameCmds);
		ExeLst.SetItemText(i, 4, GameDlls);

	}
}




void CWndINJ::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	int index = pNMItemActivate->iItem;
	if (index < 0) return;
	
	CString GamePath = ExeLst.GetItemText(index, 2);
	CString GameExe  = ExeLst.GetItemText(index, 1);
	CString GameCmds = ExeLst.GetItemText(index, 3);
	CString GameDlls = ExeLst.GetItemText(index, 4);

	UpdateData(TRUE);
	PROCESS_INFORMATION prinfo{};
	bool Pause = B_PAUSE;
	if ((B_INJCET) && (GameDlls.GetLength() > 1))
	{
		Pause = true;
	}
	
	m_INJECT.StartProcess(GameExe, GamePath, GameCmds.GetBuffer(), &prinfo,Pause);
	if ((B_INJCET) && (GameDlls.GetLength() > 1))
	{
		m_INJECT.CreateRemoteData(prinfo.hProcess, GameExe, GameDlls);
	}

	if (B_DEBUG)
	{
		PROCESS_INFORMATION odinfo{};
		CString dbgExe, dbgPath, dbgCmds;
		dbgExe = L"D:\\rvs\\OllyDbg\\Ollydbg.exe";
		dbgPath = L"D:\\rvs\\OllyDbg\\";
		dbgCmds.Format(L"%s -p %d", dbgExe, prinfo.dwProcessId);
		m_INJECT.StartProcess(dbgExe, dbgPath, dbgCmds.GetBuffer(), &odinfo, false);
	}

	if (B_PAUSE)
	{
		AfxMessageBox(L"按下确认,进程开始执行!");
	}

	ResumeThread(prinfo.hThread);
	//STARTUPINFO si{};
	//si.cb = sizeof(si);
	
	/*
	
	*/
	
	
	
	//_REMOTE_DATA _data{};
	//m_INJECT.CodeRemoteData(_data);
	//prinfo.
	
	/*
	CreateProcess(GameExe,
		GameCmds.GetBuffer(),
		NULL, NULL, true,
		CREATE_SUSPENDED,
		NULL,
		GamePath,
		&si,
		&prinfo
	);
	*/

	/*
	
	

	*/

	//API 获取PE头
	//CStringA GameExeA;
	//GameExeA = GameExe;
	//PLOADED_IMAGE image = ImageLoad(GameExeA, NULL);
	//DWORD dEntryPoint = image->FileHeader->OptionalHeader.AddressOfEntryPoint;
	/*
	void* image = _imageload(GameExe.GetBuffer());
	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)image;
	unsigned PEAddress = dosHeader->e_lfanew + (unsigned)image;

	IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)PEAddress;
	DWORD dEntryPoint = ntHeader->OptionalHeader.AddressOfEntryPoint;

	//CString wTxt;
	//wTxt.Format(L"%X", dEntryPoint);
	//AfxMessageBox(wTxt);

	_unloadimage(image);
	*/
	//ImageUnload(image);

	//sizeof(IMAGE_DOS_HEADER)
    //0x3C
	//IMAGE_NT_HEADERS
	
	//构建汇编代码
	//INJECTCode();
	//LPVOID adrRemote = VirtualAllocEx(prinfo.hProcess, 0, 0x3000, MEM_COMMIT,PAGE_EXECUTE_READWRITE); //分配内存
	//SIZE_T lwt;
	//WriteProcessMemory(prinfo.hProcess, adrRemote, INJECTCode, 0x200, &lwt);

	//CString wTxt;
	//wTxt.Format(L"%X", adrRemote);
	//AfxMessageBox(wTxt);

	
}


void CWndINJ::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}



//组织远程数据

void CodeRemoteData()
{

}

void CWndINJ::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pActive = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pActive->iItem != -1)
	{
		lstSel = pActive->iItem;
		DWORD   dwPos=GetMessagePos();
		CPoint  point(LOWORD(dwPos),HIWORD(dwPos));   //0X ffff ffff
		CMenu   menu;
		menu.LoadMenuW(IDR_MENU1);
		CMenu* pop = menu.GetSubMenu(0);
		pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}


void CWndINJ::OnMenuSet()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox(L"按下了设置!");
	// 设置
	CString GameName = ExeLst.GetItemText(lstSel, 0);
	CString GamePath = ExeLst.GetItemText(lstSel, 2);
	CString GameExe  = ExeLst.GetItemText(lstSel, 1);
	CString GameCmds = ExeLst.GetItemText(lstSel, 3);
	CString GameDlls = ExeLst.GetItemText(lstSel, 4);

	wndAddGame.Init(this,false,lstSel);
	wndAddGame.SetData(GameName, GamePath, GameExe, GameCmds, GameDlls);
	wndAddGame.DoModal();
}


void CWndINJ::OnMenuDel()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox(L"按下了删除!");
	ExeLst.DeleteItem(lstSel);
	SaveLstToIni();
}

void CWndINJ::SaveLstToIni()
{
	int count = ExeLst.GetItemCount();
	for (int i = 0; i < count; i++)
	{
		CString GameName = ExeLst.GetItemText(i, 0);
		CString GamePath = ExeLst.GetItemText(i, 2);
		CString GameExe  = ExeLst.GetItemText(i, 1);
		CString GameCmds = ExeLst.GetItemText(i, 3);
		CString GameDlls = ExeLst.GetItemText(i, 4);

		CString key;
		key.Format(L"count_%d", i);
		WritePrivateProfileStringW(key, L"GameName", GameName, GamesIni);
		WritePrivateProfileStringW(key, L"GameExe", GameExe, GamesIni);
		WritePrivateProfileStringW(key, L"GamePath", GamePath, GamesIni);
		WritePrivateProfileStringW(key, L"GameCmds", GameCmds, GamesIni);
		WritePrivateProfileStringW(key, L"GameDlls", GameDlls, GamesIni);

	}
	CString wCount;
	wCount.Format(L"%d", count);
	WritePrivateProfileStringW(L"main", L"count", wCount, GamesIni);

	
}


void CWndINJ::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CWndINJ::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CWndINJ::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
}
