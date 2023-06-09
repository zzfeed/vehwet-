#include "pch.h"
#include "CParamContext.h"

CParamContext::CParamContext()
{
}

CParamContext::CParamContext(LPCWSTR _name, LPCWSTR _typename, LPCWSTR _note, CParamContext* _front)
{
	txtName = _name;
	txtType = _typename;
	txtNote = _note;
	Front   = _front;
}

CParamContext* CParamContext::Add(LPCWSTR _name, LPCWSTR _typename, LPCWSTR _note)
{
	PCParamContext p = new CParamContext(_name, _typename, _note, this);
	Next = p;
	return p;
}
