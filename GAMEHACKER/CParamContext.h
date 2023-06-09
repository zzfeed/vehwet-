#pragma once
typedef class CParamContext
{
	
		

		
public:
	CParamContext();
	CParamContext(LPCWSTR _name, LPCWSTR _typename,LPCWSTR _note, CParamContext* _front=NULL);
	CParamContext* Add(LPCWSTR _name, LPCWSTR _typename, LPCWSTR _note);
	    CString txtName;
		CString txtType;
		CString txtNote;

		CParamContext* Front{};
		CParamContext* Next{};

}*PCParamContext;


