#include "pch.h"
#include "DBGPOINT.h"

bool DBGPOINT::AddHookPoint(LPVOID _adr, HOOKBACK2 _hbk, LPVOID _adrRet)
{
    if(PointCount>3) return false;
    Point[PointCount++].SetPoint(_adr,_hbk,_adrRet);
}

PHPOINT DBGPOINT::FindPoint(LPVOID _adr)
{
    for (int i = 0; i < PointCount; i++)
    {
       // CString wTxt;
        //wTxt.Format(L"%X %X", _adr, Point[i].Address);
       // AfxMessageBox(wTxt);
        if (Point[i].Address == _adr) return &Point[i];
    }
    return nullptr;
}

void DBGPOINT::OpenDbg(PCONTEXT _context)
{
    _context->Dr0 = (DWORD)Point[0].Address;
    _context->Dr1 = (DWORD)Point[1].Address;
    _context->Dr2 = (DWORD)Point[2].Address;
    _context->Dr3 = (DWORD)Point[3].Address;
    _context->Dr7 = 0b01010101;
}

void HPOINT::SetPoint(LPVOID _adr, HOOKBACK2 _hbk, LPVOID _adrRet)
{
    Address    = _adr;
    DestCall   = _hbk;
    AddressRet = _adrRet;
}
