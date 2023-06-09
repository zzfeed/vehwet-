#include "pch.h"
#include "htdHook2.h"

htdHook2* htdHook2Ptr;

LONG  _stdcall PvectoredExceptionHandler(PEXCEPTION_POINTERS val)
{
	
	//CString wTxt;
	//wTxt.Format(L"%X", val->ExceptionRecord->ExceptionCode);
	//AfxMessageBox(wTxt);
	unsigned _eip = val->ContextRecord->Eip;
	if (val->ExceptionRecord->ExceptionCode == STATUS_BREAKPOINT)
	{
		
		PHOOKPOINT point = htdHook2Ptr->Points.FindPoint((LPVOID)_eip);
		if (point)
		{
			if (point->GetHookBack2()(val->ContextRecord))
			{
				val->ContextRecord->Eip = (unsigned)point->CodeFix;
			}
			else
			{
				val->ContextRecord->Eip = (unsigned)point->AddressRet;
			}

			return EXCEPTION_CONTINUE_EXECUTION;
		}
		else return EXCEPTION_CONTINUE_SEARCH;
	}

	if (val->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP)
	{
		//
		//AfxMessageBox(L"1");
		auto point=htdHook2Ptr->DbgPoints.FindPoint((LPVOID)_eip);
		if (point)
		{
			
			//AfxMessageBox(L"2");
			if (point->DestCall(val->ContextRecord))
			{
				//AfxMessageBox(L"3");
				val->ContextRecord->Dr7 = 0; //调试取消掉
				val->ContextRecord->EFlags |= 0x100;
			}
			else return EXCEPTION_CONTINUE_EXECUTION;
		}
		else
		{
			htdHook2Ptr->DbgPoints.OpenDbg(val->ContextRecord);
		}

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}



void ThreadTrap()
{
     _asm
	 {
	     mov eax,eax
		 mov eax,eax
		 mov eax,eax
	 }
}


bool InThread(HOOKREFS2)
{
	htdHook2Ptr->UnHook((LPVOID)_EIP);
	htdHook2Ptr->DbgPoints.OpenDbg(val);
	return false;
}
void htdHook2::Init()
{
	SetHook(ThreadTrap, 1, InThread, ThreadTrap);
	DWORD dRet=(DWORD)ThreadTrap;
	_asm call dRet;
}

htdHook2::htdHook2()
{
	htdHook2Ptr = this;
	PPointLast = &Points;
	AddVectoredExceptionHandler(1, PvectoredExceptionHandler);//完成了异常的接管
	
}

void htdHook2::SetHook(LPVOID Address, uchar len, HOOKBACK2 hookBack,LPVOID AddressRet)
{
	DWORD dOld;
	DWORD dNew;

	VirtualProtect(Address, 0x1, PAGE_EXECUTE_READWRITE, &dOld); //修改HookFactroy内存属性为可以执行
	PPointLast = PPointLast->AddPonit(Address, AddressRet, hookBack, len);
	char* code = (char*)(Address);
	code[0] = 0xCC;
	VirtualProtect(Address, 0x1, dOld, &dNew);
}

bool htdHook2::SetHook(LPVOID Address, HOOKBACK2 hookBack, LPVOID AddressRet)
{
	return DbgPoints.AddHookPoint(Address, hookBack, AddressRet);
}

void htdHook2::UnHook(LPVOID Address)
{
	//卸载HOOK
	PHOOKPOINT _point=Points.FindPoint(Address);
	if (_point)
	{
		_point->Recover();
		_point->BackPoint->NextPoint = _point->NextPoint;
		if (_point->NextPoint)_point->NextPoint->BackPoint = _point->BackPoint;
		delete _point;
	}
}
