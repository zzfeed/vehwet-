#include "pch.h"
#include "COBJContext.h"


extern DATA_TYPE data_TYPE[12];

COBJContext::~COBJContext()
{
	if (data)
	{
		delete[] data;
		data = 0;
	}
}

DWORD COBJContext::GetSize()
{
	return Size;
}

LPVOID COBJContext::GetMemAddress()
{
	return Address;
}

CString& COBJContext::GetAddress()
{
	return txtAddress;
}

CString& COBJContext::GetNote()
{
	return txtNote;
}

CString& COBJContext::GetName()
{
	return txtName;
}

void COBJContext::CreateMEMContext(PCMEMContext _front, DWORD _size)
{

	
	PCMEMContext _headC{}, _body{}, _foot{};
	int          _headCount{}, _bodyCount{}, _footCount{};
	_headCount = (4 - (_front->lenth * _front->type->Size) % 4)%4;
	_bodyCount = _size - _headCount;
	if (_bodyCount < 0)
	{
		_bodyCount = 0;
		_headCount = _size;
	}

	CString txtH,txtB,txtF;
	do
	{
		txtH.Format(L"unknownH%d", NameId++);
	} while (!TestContextName(txtH));
	do
	{
		txtB.Format(L"unknownB%d", NameId++);
	} while (!TestContextName(txtB));
	do
	{
		txtF.Format(L"unknownF%d", NameId++);
	} while (!TestContextName(txtF));

	if (_headCount > 0)
	{
		_front = _front->InsertContext(T_char, _headCount, txtH, L"unknown");
	}

	if (_bodyCount > 0)
	{
		_footCount = _bodyCount % 4;
		_bodyCount = _bodyCount / 4;
		_front = _front->InsertContext(T_int, _bodyCount, txtB, L"unknown");
	}

	if (_footCount > 0)
	{
		_front = _front->InsertContext(T_char,_footCount,txtF,L"unknown");
	}

	
}



CString COBJContext::ReadValue(PCMEMContext val, int icount)
{
	CString rt;
	unsigned  offset = val->offset;
	CStringA rtA;

	LPVOID _offset = data + offset;

	switch (val->type_id)
	{
	case T_bool:
		{
			bool* read = (bool*)(_offset);
			if (read[icount])rt = "true";
			else rt = "false";
			break;
		}
	case T_short:
		{
			short* read = (short*)(_offset);
			rt.Format(L"%d", read[icount]);
			break;
		}
	case T_ushort:
		{
			unsigned short* read = (unsigned short*)(_offset);
			rt.Format(L"%u", read[icount]);
			break;
		}
	case T_float:
		{
			float* read = (float*)(_offset);
			rt.Format(L"%f",read[icount]);
			break;
		}
	case T_int:
	    {
			int* read = (int*)(_offset);
			rt.Format(L"%d", read[icount]);
			break;
		}
	case T_uint:
	{
		unsigned* read = (unsigned*)(_offset);
		rt.Format(L"%u", read[icount]);
		break;
	}
	case T_char:
	     {
		    char* read = (char*)(_offset);
			if (!val->Extend)
			{
				rtA.Format("%s", read);
			}
			else
			{
				rtA.Format("%X", read[icount]);
			}
			rt = rtA;
	     }
		 break;
	case T_uchar:
		{
		  wchar_t* read = (wchar_t*)(_offset);
		  if (!val->Extend)
		  {
			rt.Format(L"%s", read);
		  }
		  else
		  {
			  rt.Format(L"%X", read[icount]);
		  }
		  break;
		}
	case T_llong:
		{
			long long* read = (long long*)(_offset);
			rt.Format(L"%lld", read[icount]);
			break;
		}
	case T_ullong:
		{
			unsigned long long* read = (unsigned long long*)(_offset);
			rt.Format(L"%llu", read[icount]);
			break;
		}
	case T_double:
		{
			double* read = (double*)(_offset);
			rt.Format(L"%lf", read[icount]);
			break;
		}
	case T_pvoid:
	    {
			DWORD_PTR* dRead = (DWORD_PTR*)(_offset);
			rt.Format(L"0x%X", dRead[icount]);
			break;
	    }

	}
	return rt;
}

CString COBJContext::ReadGValue(PCMEMContext val, int icount)
{
	CString rt;
	CStringA rtl;
	unsigned  offset = val->offset;
	LPVOID adr = (LPVOID)((DWORD_PTR)data + offset + icount * val->type->Size);
	wchar_t* txtBool[]{ L"false",L"true" };
	wchar_t* txtB;
	switch (GId)
	{
	case 0://uincode 字符串
		{
			wchar_t* read = (wchar_t*)(adr);
			rt.Format(L"%s", read);
			break;
		}
	case 1://ascii 字符串
		{
			char* read = (char*)(adr);
			rtl.Format("%s", read);
			rt = rtl;
			break;
		}
	case 2://8
		{
			long long* llRead = (long long*)(adr);
			unsigned long long* ullRead = (unsigned long long*)(adr);
			double* dRead = (double*)(adr);
			rt.Format(L"%lld|%llu|%lf", llRead[0], ullRead[0], dRead[0]);
			break;
	    }
	case 3://4
		{
			int* llRead = (int*)(adr);
			unsigned* ullRead = (unsigned*)(adr);
			float* dRead = (float*)(adr);
			rt.Format(L"%d|%u|%f", llRead[0], ullRead[0], dRead[0]);
			break;
		}
	case 4://2
		{
			short* llRead = (short*)(adr);
			unsigned short* ullRead = (unsigned short*)(adr);
			rt.Format(L"%d|%u", llRead[0], ullRead[0]);
			break;
		}
	case 5://1
		{
		    
			char* llRead = (char*)(adr);
			unsigned char* ullRead = (unsigned char*)(adr);
			bool* boolRead = (bool*)(adr);
			if (boolRead[0])
			{
				txtB = txtBool[1];
			}
			else
			{
				txtB = txtBool[0];
			}
			rt.Format(L"%d|%u|%s", llRead[0], ullRead[0],txtB);
			break;
		}
	case 6://0
		{
			DWORD_PTR* dRead = (DWORD_PTR*)(adr);
			rt.Format(L"%X", dRead[0]);
			break;
	    }
	}
	return rt;
}

CString COBJContext::ReadGAllValue(PCMEMContext val, int icount)
{
	CString rt,ls;
	CStringA rtl;
	unsigned  offset = val->offset;
	LPVOID adr = (LPVOID)((DWORD_PTR)data + offset + icount * val->type->Size);
	wchar_t* txtBool[]{ L"false",L"true" };
	wchar_t* txtB;
	{
		wchar_t* read = (wchar_t*)(adr);
		ls.Format(L"%s", read);
	}

	rt = rt + L"UNICODE:" + ls+L"\r\n";
	{
		char* read = (char*)(adr);
		rtl.Format("%s", read);
		ls = rtl;
	}
	rt = rt + L"ASCII:" + ls + L"\r\n";
	{
		long long* llRead = (long long*)(adr);
		unsigned long long* ullRead = (unsigned long long*)(adr);
		double* dRead = (double*)(adr);
		ls.Format(L"long long:%lld\r\nunsigned long long:%llu\r\ndouble:%lf", llRead[0], ullRead[0], dRead[0]);
	}
	rt = rt + ls + L"\r\n";
	{
		short* llRead = (short*)(adr);
		unsigned short* ullRead = (unsigned short*)(adr);
		ls.Format(L"short:%d\r\nunsigned short:%u", llRead[0], ullRead[0]);
	}
	rt = rt + ls + L"\r\n";
	{
		char* llRead = (char*)(adr);
		unsigned char* ullRead = (unsigned char*)(adr);
		bool* boolRead = (bool*)(adr);
		if (boolRead[0])
		{
			txtB = txtBool[1];
		}
		else
		{
			txtB = txtBool[0];
		}
		ls.Format(L"char:%d\r\nunsigned char:%u\r\nbool:%s", llRead[0], ullRead[0], txtB);
	}
	rt = rt + ls + L"\r\n";

	DWORD_PTR* dRead = (DWORD_PTR*)(adr);
	ls.Format(L"LPVOID:0x%X", dRead[0]);
	rt = rt + ls + L"\r\n";
	
	return rt;
}

int COBJContext::SetMemContext(PCMEMContext val, const wchar_t* _name, const wchar_t* _note, DWORD _lenth, DWORD _typeId,DWORD _index)
{
	//设置
	//1 名称
	//2 备注
	if (_index)
	{
		//新增
		DWORD needSize     = data_TYPE[_typeId].Size * _lenth;//需要的大小
		DWORD startOffSet  = val->offset+val->type->Size*_index;//能用的大小
		
		auto p = val;
		while (p->Next)
		{
			if (p->Next->fix) break;
			p = p->Next;
		}

		DWORD lastOffSet{};
		if (p->Next)
		{
			lastOffSet = p->Next->offset;
		}
		else
		{
			lastOffSet = p->offset + p->type->Size * p->lenth;
		}

		
		DWORD freeSize = lastOffSet-startOffSet;

		

		if (needSize > freeSize) return -1; //内存空间不足

		//DWORD freeSize = val->
		//
		val->lenth = _index; //20 //2 3 
		auto next = val->InsertContext(_typeId, _lenth, _name, _note,true);
		DWORD dSize;
		if (next->Next)
		{
			dSize = next->Next->offset - (next->offset + next->type->Size * next->lenth);
			//
		}
		else
		{
			dSize= Size - (next->type->Size * next->lenth + next->offset);
		}
		CreateMEMContext(next, dSize);
		SaveMemContext();
	}
	else
	{
		//修改
		

		bool bReset{};
		auto oldSize = val->type->Size;

		unsigned allMemorySize    = data_TYPE[_typeId].Size * _lenth;
		unsigned allMemorySizeOld = val->type->Size * val->lenth;
		//新的大小
		if (val->Next)
		{
			allMemorySize += val->Next->fix;
			allMemorySizeOld += val->Next->fix;
		}
		
		

		if (allMemorySize < allMemorySizeOld)
		{		
			val->txtName = _name;
			val->txtNote = _note;
			val->Used = true;
			val->type_id = _typeId;
			val->type = &data_TYPE[_typeId];
			val->lenth = _lenth;
			
			if ((_typeId == T_char) || (_typeId == T_uchar))
			{
				val->Extend = false;
			}
			else
			{
				val->Extend = true;
			}

			CreateMEMContext(val, allMemorySizeOld - allMemorySize);
			SaveMemContext();
			return 0;
		}

		if (allMemorySize > allMemorySizeOld)
		{
			//新的内存占用超出实际可分配
			return -1;
		}

		val->txtName = _name;
		val->txtNote = _note;
		val->Used = true;
		val->type_id = _typeId;
		val->type = &data_TYPE[_typeId];
		val->lenth = _lenth;
		SaveMemContext();
		

		return 0;
	}
	//=====>_size 大小
	//=====>_typeId
}

void COBJContext::SaveMemContext()
{
	//
	int count = 0;
	CString App;
	
	
	auto p = _MEMCONTEXT.Next;

	int     val;
	CString valTxt;
	while (p)
	{
		//int val = p->fix;
		App.Format(L"MEMContext_%d", count);
		valTxt.Format(L"%d", p->fix);
		WritePrivateProfileString(App, L"fix", valTxt, txtFile);
		if (p->Used)
		{
			WritePrivateProfileString(App, L"used", L"1", txtFile);
		}
		else
		{
			WritePrivateProfileString(App, L"used", L"0", txtFile);
		}
		valTxt.Format(L"%d", p->type_id);
		WritePrivateProfileString(App, L"typeid", valTxt, txtFile);
		valTxt.Format(L"%d", p->lenth);
		WritePrivateProfileString(App, L"lenth", valTxt, txtFile);
		valTxt.Format(L"%d", p->offset);
		WritePrivateProfileString(App, L"offset", valTxt, txtFile);
		WritePrivateProfileString(App, L"txtName", p->txtName, txtFile);
		WritePrivateProfileString(App, L"txtNote", p->txtNote, txtFile);
		p = p->Next;
		count++;
	}
	valTxt.Format(L"%d", count);
	WritePrivateProfileString(L"main", L"count", valTxt, txtFile);

	/*
	
	..unsigned     fix;
	..bool         Used{};    //false true 
	
	unsigned     type_id{};
	unsigned     lenth{};    //1/575/
	unsigned     offset{};   //偏移
	CString      txtName{};  //hp    int hp;
	CString      txtNote{};  //备注
	

	*/

	//AfxMessageBox(txtFile);
}

void COBJContext::LoadMemContext()
{
	//
	
	CString App;

	auto p = &_MEMCONTEXT;

	int     val;
	CString valTxt;
	wchar_t buffNote[0xFF]{};
	wchar_t buffName[0xFF]{};
	int count = GetPrivateProfileInt(L"main",L"count",0,txtFile);

	
	for (int i = 0; i < count; i++)
	{
		App.Format(L"MEMContext_%d", i);
		int fix  = GetPrivateProfileInt(App, L"fix", 0, txtFile);
		int used = GetPrivateProfileInt(App, L"used", 0, txtFile);
		int typedid = GetPrivateProfileInt(App, L"typeid", 0, txtFile);
		int lenth = GetPrivateProfileInt(App, L"lenth", 0, txtFile);
		int offset = GetPrivateProfileInt(App, L"offset", 0, txtFile);

		GetPrivateProfileString(App, L"txtName", L"txtName", buffName, 0xFF, txtFile);
		GetPrivateProfileString(App, L"txtNote", L"txtNote", buffNote, 0xFF, txtFile);

		//AfxMessageBox(buffName);
		//CString h;
		//h.Format(L"%d", typedid);
		//AfxMessageBox(h);

		p=p->AddContext(typedid, lenth, buffName, buffNote,used);
		
	}

}

void COBJContext::CreateCPP()
{
	CString fileName = txtFolder + L"\\" + txtName + L".h";
	std::locale& loc = std::locale::global(std::locale(std::locale(),"",LC_CTYPE));
	std::wofstream ofs(fileName);
	auto p = MEMContext;
	bool bused{};

	if (!ofs.bad())
	{
		ofs << L"#pragma once" << std::endl;
		ofs << L"//"<< txtNote.GetBuffer() << std::endl;
		ofs << L"typedef class " << txtName.GetBuffer() << std::endl;
		ofs << L"{" << std::endl;

		CString val;
		while (p)
		{
			if (bused != p->Used)
			{
				if (p->Used) ofs << "public:" << std::endl;
				else ofs << "protected:" << std::endl;
				bused = p->Used;
			}

			if (p->lenth > 1)
			{
				val.Format(L"%s[%d]", p->txtName.GetBuffer(), p->lenth);
			}
			else
			{
				val.Format(L"%s", p->txtName.GetBuffer());
			}

			ofs <<"		" << p->type->UName << "  " << val.GetBuffer() << ";//" << p->txtNote.GetBuffer()<<std::endl;
			p = p->Next;
		}

		ofs << L"}*P" << txtName.GetBuffer() <<";"<< std::endl;

		std::locale::global(loc);
		ofs.close();
	}
}

bool COBJContext::TestContextName(CString& _name)
{
	//CString val = _name;
	auto p = MEMContext;
	while (p)
	{
		if (p->txtName == _name) return false;
		p = p->Next;
	}
	return true;
}

bool COBJContext::TestFuncName(CString& _name)
{
	auto p = _FUNCONTEXT.Next;
	while (p)
	{
		
		if (p->txtShot == _name) return false;
		p = p->Next;
	}
	return true;
}

PCFUNContext COBJContext::AddFunContext(LPCWSTR _name, LPCWSTR _typename, LPCWSTR _note, LPCWSTR _base, unsigned _offset, PCParamContext _params)
{
	lastFUNContext=lastFUNContext->Add(_name, _typename, _note, _base, _offset, _params);
	return lastFUNContext;
}



void COBJContext::SetGID(int val)
{
	GId = val;
}

BOOL COBJContext::UpdateData(HANDLE _hProcess)
{
	
	BOOL bRet= ReadProcessMemory(_hProcess, Address, data, Size, NULL);
	return bRet;
}

void COBJContext::Set(const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* _note,bool isset)
{
	CString _txtName;
	_txtName.Format(L"%s", _name);//名称改变
	
	txtAddress.Format(L"%s", _address);
	txtNote.Format(L"%s", _note);
	if (_size>Size) //变化检测
	{
		//Size = _size; //涉及到内存的重新分配
		if (data) delete[] data;
		data = new char[_size]; //重新分配内存
	}
	Size = _size; //涉及到内存的重新分配
	Address = (LPVOID)wcstoul(_address, 0, 16);

	if ((_txtName != txtName)&&(isset))
	{
		//说明名字发生了变化
		DeleteFile(txtFile); //文件删除
	}
	
	txtName = _txtName;
	txtFile = txtFolder + L"\\" + txtName + L".ini";
	Save();
}

void COBJContext::Save()
{
	//保存数据到文件
	CString val;
	val.Format(L"%d", Size);
	WritePrivateProfileString(L"main", L"address", txtAddress.GetBuffer(), txtFile);
	WritePrivateProfileString(L"main", L"size", val.GetBuffer(), txtFile);
	WritePrivateProfileString(L"main", L"note", txtNote.GetBuffer(), txtFile);
}

void COBJContext::Delete()
{
	DeleteFile(txtFile); //文件删除
}

COBJContext::COBJContext(const wchar_t* folder,const wchar_t* _name)
{
	

	txtFolder = folder;
	txtFile = txtFolder + L"\\" + _name + L".ini";

	wchar_t _address[0xFF]{};
	wchar_t _note[0xFF]{};

	GetPrivateProfileString(L"main", L"address", L"0", _address, 0xFF, txtFile);
	GetPrivateProfileString(L"main", L"note", L"", _note, 0xFF, txtFile);
	DWORD _size=GetPrivateProfileInt(L"main", L"size", 0, txtFile);

	Set(_name, _address, _size, _note, false);
	MEMContext = &_MEMCONTEXT;
	//CreateMEMContext(MEMContext,Size);
	LoadMemContext();
	MEMContext = _MEMCONTEXT.Next;

	FUNContext = _FUNCONTEXT.Next;
	lastFUNContext = &_FUNCONTEXT;
}

COBJContext::COBJContext(const wchar_t* folder, const wchar_t* _name, const wchar_t* _address, DWORD _size, const wchar_t* _note)
{
	txtFolder = folder;
	txtFile = txtFolder + L"\\" + _name + L".ini";
	Set(_name, _address, _size, _note, false);
	MEMContext = &_MEMCONTEXT;
	CreateMEMContext(MEMContext, Size);
	MEMContext = _MEMCONTEXT.Next;

	FUNContext = _FUNCONTEXT.Next;
	lastFUNContext = &_FUNCONTEXT;
}
