#include "pch.h"
#include "CFUNContext.h"

CFUNContext::CFUNContext()
{
}

CFUNContext* CFUNContext::Add(LPCWSTR _name, LPCWSTR _typename,  LPCWSTR _note, LPCWSTR _base, unsigned _offset, PCParamContext _params)
{
	auto p = new CFUNContext(_name, _typename, _note, _base, _offset, _params, this);
	Next = p;
	return p;
}

CFUNContext::CFUNContext(LPCWSTR _name, LPCWSTR _typename, LPCWSTR _note, LPCWSTR _base, unsigned _offset, PCParamContext _params, CFUNContext* _front)
{
	auto p = _params;
	CString valParams;
	while (p)
	{
		valParams = valParams + p->txtType +L",";
		p = p->Next;
	}
	txtShot.Format(L"%s(%s)", _name,valParams.GetBuffer()); //生成基本信息
	Front = _front;
	txtName = _name;
	typeName = _typename;
	txtNote = _note;
	txtBase = _base;
	OffSet = _offset;
	Params = _params;

}
