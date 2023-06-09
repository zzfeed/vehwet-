#pragma once
#include "CMEMContext.h"
#include "CFUNContext.h"
#include <fstream>

typedef class COBJContext
{
	CString txtFolder;
	CString txtFile;

	CString  txtName;
	LPVOID   Address;//
	CString  txtAddress;// [sword2.exe+EE232]+2
	DWORD    Size{};
	CString  txtNote;//

	char* data{};

	int   GId{};

	unsigned   NameId{};
public:
	void SetGID(int val);
public:
	BOOL UpdateData(HANDLE _hProcess);
	void Set(const wchar_t* _name,const wchar_t* _address,DWORD _size,const wchar_t* _note,bool IsSet=true);
	void Save();
	void Delete();
	COBJContext(const wchar_t* folder,const wchar_t* _name);
	COBJContext(const wchar_t* folder, const wchar_t* _name,const wchar_t* _address, DWORD _size, const wchar_t* _note);
	~COBJContext();
public:
	DWORD GetSize();
	LPVOID   GetMemAddress();
	CString& GetAddress();
	CString& GetNote();
	CString& GetName();
public:
	
	CMEMContext  _MEMCONTEXT;
	PCMEMContext MEMContext{}; //
	
	void         CreateMEMContext(PCMEMContext _front, DWORD _size);//创建主要分析 
	CString      ReadValue(PCMEMContext val, int icount);
	CString      ReadGValue(PCMEMContext val, int icount);
	CString      ReadGAllValue(PCMEMContext val, int icount);
public:
	int          SetMemContext(PCMEMContext val,const wchar_t* _name,const wchar_t* _note,DWORD _lenth,DWORD _typeId,DWORD _index);
	void         SaveMemContext();
	void         LoadMemContext();
	void         CreateCPP();

	bool         TestContextName(CString& _name);
	bool         TestFuncName(CString& _name);
public:
	//函数的功能
	CFUNContext   _FUNCONTEXT;
	PCFUNContext  FUNContext{};
	PCFUNContext  lastFUNContext{};
	PCFUNContext  AddFunContext(LPCWSTR _name, LPCWSTR _typename, LPCWSTR _note, LPCWSTR _base, unsigned _offset, PCParamContext _params);

}*PCOBJContext;


typedef struct TREE_DATA
{
	DWORD      MenuId{};
	LPVOID     DATA_PTR{};
}*PTREE_DATA;

typedef struct LIST_DATA
{
	DWORD_PTR      ExId; //扩展值
	PCMEMContext   MContext;//指针
	DWORD          index;//索引
}*PLIST_DATA;
/*
//txtNote
class MONSETR
{

};
*/

