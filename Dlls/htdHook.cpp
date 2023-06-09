#include "pch.h"
#include "htdHook.h"

htdHook* htdHookPtr;

unsigned GetJMPCode(unsigned _target, unsigned eip)
{
	return _target - eip - 0x5;
}

void _stdcall DisHook(PCPUINFO e)
{
	unsigned _eip = e->Eip - 0x5;
	
	PHOOKPOINT point = htdHookPtr->Points.FindPoint((LPVOID)_eip);
	if (point)
	{
		
		if (point->GetHookBack()(e))
		{
			e->Eip = (unsigned)point->CodeFix;
		}
		else
		{
			e->Eip = (unsigned)point->AddressRet;
		}
	}

}

//HookFactroy
char data_code[]
{
	0x60,                    //pushad
	0x9C,                    //pushfd
	0x54,                    //push esp
	0xE8,0xCC,0xCC,0xCC,0xCC,//call DisHook 
	0x9D,                    //popfd
	0x61,                    //popad
	0xC3                     //retn
};

htdHook::htdHook()
{
	htdHookPtr = this;
	PPointLast = &Points;

	DWORD dOld;
	VirtualProtect(data_code, sizeof(data_code), PAGE_EXECUTE_READWRITE, &dOld); //修改HookFactroy内存属性为可以执行
	unsigned* Adr = (unsigned*)(data_code + 0x4);
	unsigned  target = (unsigned)DisHook;
	Adr[0] = GetJMPCode(target, (unsigned)(data_code + 0x3));
}

void htdHook::SetHook(LPVOID Address, uchar len, HOOKBACK hookBack, LPVOID AddressRet)
{
	DWORD dOld;
	DWORD dNew;

	VirtualProtect(Address, 0x5, PAGE_EXECUTE_READWRITE, &dOld); //修改HookFactroy内存属性为可以执行
	PPointLast = PPointLast->AddPonit(Address, AddressRet, hookBack, len);
	char* code = (char*)Address;
	code[0] = 0xE8; //CALL
	unsigned* Adr = (unsigned*)(code + 1);
	Adr[0] = GetJMPCode((unsigned)data_code, (unsigned)Address);
	VirtualProtect(Address, 0x5, dOld, &dNew);

}


