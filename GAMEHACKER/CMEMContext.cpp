#include "pch.h"
#include "CMEMContext.h"



DATA_TYPE data_TYPE[12]{};


void InitDataTypeTable()
{
	data_TYPE[T_bool].UName = L"bool";
	data_TYPE[T_bool].Size = 1;

	data_TYPE[T_char].UName = L"char";
	data_TYPE[T_char].Size = 1;

	data_TYPE[T_uchar].UName = L"unsigned char";
	data_TYPE[T_uchar].Size = 1;

	data_TYPE[T_short].UName = L"short";
	data_TYPE[T_short].Size = 2;

	data_TYPE[T_ushort].UName = L"unsigned short";
	data_TYPE[T_ushort].Size = 2;

	data_TYPE[T_int].UName = L"int";
	data_TYPE[T_int].Size = 4;

	data_TYPE[T_uint].UName = L"unsigned";
	data_TYPE[T_uint].Size = 4;

	data_TYPE[T_float].UName = L"float";
	data_TYPE[T_float].Size = 4;

	data_TYPE[T_llong].UName = L"long long";
	data_TYPE[T_llong].Size = 8;

	data_TYPE[T_ullong].UName = L"unsigned long long";
	data_TYPE[T_ullong].Size = 8;

	data_TYPE[T_double].UName = L"double";
	data_TYPE[T_double].Size = 8;

	data_TYPE[T_pvoid].UName = L"void*";
	data_TYPE[T_pvoid].Size = sizeof(void*);
}

CMEMContext::CMEMContext()
{
	type = &data_TYPE[0];
	fix = 0;
}

CMEMContext::CMEMContext(int TypeId, int _len,const wchar_t* _name, const wchar_t* _note, CMEMContext* _front,bool _used)
{
	type_id = TypeId;
	lenth = _len;
	type = &data_TYPE[TypeId];
	txtName = _name;
	txtNote = _note;
	Front = _front;
	Used = _used;
	switch (TypeId)
	{
	case T_char:
	case T_uchar:
		Extend = false;
		break;
	}
	//¼ÆËãÆ«ÒÆ
	if (Front)
	{
		//offset = Front->offset + Front->type->Size*Front->lenth;
		unsigned align = 4;
		unsigned allSize = Front->offset + Front->type->Size * Front->lenth;
		unsigned m = allSize % align;
		unsigned free = align - m;
		
		if (free < type->Size)
		{
			fix = free;
		}
		else
		{
			fix = (type->Size - m % type->Size) % type->Size;
		}
		offset = allSize + fix;
	}

}

PCMEMContext CMEMContext::AddContext(int TypeId, int _len, const wchar_t* _name,  const wchar_t* _note,bool _used)
{
	PCMEMContext _context = new CMEMContext(TypeId, _len, _name, _note, this, _used);
	Next = _context;
	return Next;
}

PCMEMContext CMEMContext::InsertContext(int TypeId, int _len, const wchar_t* _name, const wchar_t* _note, bool _used)
{
	PCMEMContext _context = new CMEMContext(TypeId, _len, _name, _note, this, _used);
	_context->Next = Next;
	if(Next)Next->Front = _context;
	Next = _context;
	return Next;
}
