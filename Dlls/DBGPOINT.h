#pragma once
typedef bool (*HOOKBACK2)(PCONTEXT);

typedef class HPOINT
{
	
public:
	HOOKBACK2  DestCall{};
	LPVOID     Address{};
	LPVOID     AddressRet{};
	void SetPoint(LPVOID, HOOKBACK2, LPVOID);
}*PHPOINT;

class DBGPOINT
{
	HPOINT     Point[4];
	short      PointCount{};
public:
	bool       AddHookPoint(LPVOID, HOOKBACK2, LPVOID);
	HPOINT*    FindPoint(LPVOID);
	void       OpenDbg(PCONTEXT _context);
};
