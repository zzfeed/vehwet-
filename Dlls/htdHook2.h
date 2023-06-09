#pragma once

#include "HOOKPOINT.h"
class htdHook2
{
	
	PHOOKPOINT PPointLast{};
	
public:
	DBGPOINT   DbgPoints;
	HOOKPOINT  Points;
	void       Init();
public:
	htdHook2();
	void SetHook(LPVOID Address, uchar len,  HOOKBACK2 hookBack, LPVOID AddressRet=0);//, HOOKBACK hookBack, uchar len, LPVOID AddressRet);
	bool SetHook(LPVOID Address, HOOKBACK2 hookBack, LPVOID AddressRet = 0);
	void UnHook(LPVOID Address);
};

