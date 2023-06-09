#pragma once
#define _HTDHOOK2_
#include "DBGPOINT.h"
typedef struct CPUINFO
{
	unsigned eflags;
	unsigned Edi;
	unsigned Esi;
	unsigned Ebp;
	unsigned Esp;
	unsigned Ebx;
	unsigned Edx;
	unsigned Ecx;
	unsigned Eax;
	unsigned Eip;

}*PCPUINFO;


#define HOOKREFS2 PCONTEXT val
#define HOOKREFS PCPUINFO val


#define _ESI val->Esi
#define _EDI val->Edi
#define _EBP val->Ebp
#define _ESP val->Esp
#define _EBX val->Ebx
#define _EDX val->Edx
#define _ECX val->Ecx
#define _EAX val->Eax
#define _EIP val->Eip


typedef bool (*HOOKBACK)(PCPUINFO);


typedef unsigned char uchar;



typedef class HOOKPOINT
{
	LPVOID     Address;
	
	
	HOOKBACK   DestCall; //HOOK跳转地址
	uchar      len;
	char*      CodeOld;
public:

	HOOKPOINT* NextPoint;
	HOOKPOINT* BackPoint;

	char*      CodeFix;  //修复区的代码
	

	LPVOID     AddressRet;

	HOOKPOINT();
	~HOOKPOINT();
	HOOKPOINT(LPVOID, LPVOID, LPVOID _HOOKBACK, uchar lenth, HOOKPOINT*,HOOKPOINT* _NextPoint= NULL);
	HOOKPOINT* AddPonit(LPVOID, LPVOID, LPVOID _HOOKBACK, uchar lenth);
	HOOKPOINT* FindPoint(LPVOID);

	HOOKBACK2 GetHookBack2();
	HOOKBACK  GetHookBack();
	//恢复破坏的点
	void      Recover();
}*PHOOKPOINT;

