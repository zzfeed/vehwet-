#pragma once

#define T_bool   0
#define T_char   1
#define T_uchar  2
#define T_short  3
#define T_ushort 4
#define T_int    5
#define T_uint   6
#define T_float  7
#define T_llong  8
#define T_ullong 9
#define T_double 10
#define T_pvoid  11

void InitDataTypeTable();

struct DATA_TYPE
{
	wchar_t* UName;
	unsigned Size;
};

typedef class CMEMContext
{
	
public:
	bool         Extend{true};//À©Õ¹
	unsigned fix;
	bool         Used{};    //false true 
	DATA_TYPE*   type{};
	unsigned     type_id{};
	unsigned     lenth{};    //1/575/
	unsigned     offset{};   //Æ«ÒÆ
	CString      txtName{};  //hp    int hp;
	CString      txtNote{};  //±¸×¢
	CMEMContext* Front{};
	CMEMContext* Next{};

public:
	CMEMContext();
	CMEMContext(int TypeId,int _len,const wchar_t* _name,const wchar_t* _txt,CMEMContext* _front,bool _used = false);
	CMEMContext* AddContext(int TypeId, int _len, const wchar_t* _name, const wchar_t* _txt,bool _used=false);
	CMEMContext* InsertContext(int TypeId, int _len, const wchar_t* _name, const wchar_t* _txt,bool _used = false);
	//2300 575
	//1 2 4
	//3 short char
}*PCMEMContext;

