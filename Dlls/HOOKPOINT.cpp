#include "pch.h"
#include "HOOKPOINT.h"
unsigned GetJMPCode(unsigned _target, unsigned eip);

HOOKPOINT::HOOKPOINT()
{

}

HOOKPOINT::~HOOKPOINT()
{
	delete[] CodeFix;
	delete[] CodeOld;
}

HOOKPOINT::HOOKPOINT(LPVOID _adr, LPVOID _adrRet , LPVOID _hbk, uchar lenth, HOOKPOINT* _BackP, HOOKPOINT* _NextP):
	Address(_adr), AddressRet(_adrRet) , DestCall((HOOKBACK)_hbk), BackPoint{ _BackP }, NextPoint{ _NextP }
{
	//为了恢复准备的
	len = lenth;
	CodeOld = new char[len];
	memcpy(CodeOld, _adr, len);
	
	//修复过程 JMP CALL
	DWORD offcount;

	char*      code = (char*)(_adr);
	unsigned* u = (unsigned*)(code + 1);
	unsigned dest = u[0] + (unsigned)_adr + 5;// = mubiao
	bool fix{};
	
	
	//E8/E9

	//char a = 0xE8
	//int  b = a;
	//b = 0xFFFFFFE8;

	switch (code[0]) //E8 int a=E8 0000E8 
	{
	case 0xFFFFFFE8:
	case 0xFFFFFFE9:
		fix = true;
		break;
	}

	DWORD dOld;
	CodeFix = new char[lenth + 0x5]; //分配内存 jmp
	memcpy(CodeFix, _adr, lenth);


	if (fix)
	{
		u = (unsigned*)(CodeFix + 1);
		u[0]= GetJMPCode(dest,(unsigned)CodeFix);
	}
	
	CodeFix[lenth] = 0xE9;
	unsigned* adr = (unsigned*)(CodeFix+lenth + 1);
	adr[0] = GetJMPCode((unsigned)_adr + lenth, (unsigned)&CodeFix[lenth]);
	VirtualProtect(CodeFix, lenth + 0x5, PAGE_EXECUTE_READWRITE, &dOld);
}

//增加HOOK点
HOOKPOINT* HOOKPOINT::AddPonit(LPVOID _adr, LPVOID _adrRet, LPVOID _hbk, uchar lenth)
{
	NextPoint = new HOOKPOINT(_adr, _adrRet, _hbk, lenth,this);
	return NextPoint;
}

//搜素HOOK点
HOOKPOINT* HOOKPOINT::FindPoint(LPVOID _adr)
{
	PHOOKPOINT _point=this;
	do
	{
		if (_point->Address == _adr) return _point;
		_point=_point->NextPoint;
	} while (_point);
	return nullptr;
}

HOOKBACK2 HOOKPOINT::GetHookBack2()
{
	HOOKBACK2 _rt = (HOOKBACK2)DestCall;
	return _rt;
}

HOOKBACK HOOKPOINT::GetHookBack()
{
	return DestCall;
}

void HOOKPOINT::Recover()
{
	//
	DWORD dOld;
	DWORD dNew;
	VirtualProtect(Address, len, PAGE_EXECUTE_READWRITE, &dOld);
	memcpy(Address, CodeOld, len);
	VirtualProtect(Address, len, dOld, &dNew);
}
