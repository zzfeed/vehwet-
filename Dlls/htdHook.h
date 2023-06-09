#pragma once

#include "HOOKPOINT.h"

class htdHook
{
	PHOOKPOINT PPointLast{};
public:
	HOOKPOINT  Points;
	htdHook();
	void SetHook(LPVOID Address, uchar len, HOOKBACK hookBack, LPVOID AddressRet=0);

};

