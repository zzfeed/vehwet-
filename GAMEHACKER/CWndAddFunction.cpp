// CWndAddFunction.cpp: 实现文件
//

#include "pch.h"
#include "GAMEHACKER.h"
#include "CWndAddFunction.h"
#include "afxdialogex.h"
#include "CMEMContext.h"

extern DATA_TYPE data_TYPE[12];

CString arrys[]
{
	L"void",
    L"bool",
	L"char",
	L"unsigned char",
	L"short",
	L"unsigned short",
	L"int",
	L"unsigned",
	L"long long",
	L"unsigned long long",
	L"float",
	L"double",
	L"void*"
};
// CWndAddFunction 对话框

IMPLEMENT_DYNAMIC(CWndAddFunction, CDialogEx)

CWndAddFunction::CWndAddFunction(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDFUNCTION, pParent)
	, txtName(_T(""))
	, txtNameEx(_T(""))
	, txtAddress(_T(""))
	, txtNote(_T(""))
{

}

CWndAddFunction::~CWndAddFunction()
{
}

void CWndAddFunction::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, LstType);
	DDX_Text(pDX, IDC_EDIT1, txtName);
	DDX_Text(pDX, IDC_EDIT4, txtNameEx);
	DDX_Text(pDX, IDC_EDIT3, txtAddress);
	DDX_Control(pDX, IDC_LIST1, LstParam);
	DDX_Text(pDX, IDC_EDIT2, txtNote);
}


BEGIN_MESSAGE_MAP(CWndAddFunction, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndAddFunction::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CWndAddFunction::OnBnClickedOk)
END_MESSAGE_MAP()


// CWndAddFunction 消息处理程序


void CWndAddFunction::OnBnClickedButton1()
{
	//
	wndAddParam.Init(_ParamContext);
	if (wndAddParam.DoModal() == IDOK)
	{
		
		
		if (!_ParamContext)
			_ParamContext = new CParamContext(wndAddParam.txtName, wndAddParam.txtTypeName, wndAddParam.txtNote);
		else
			_ParamContext->Add(wndAddParam.txtName, wndAddParam.txtTypeName, wndAddParam.txtNote);

		CString val;
		val.Format(L"%s %s", wndAddParam.txtTypeName.GetBuffer(), wndAddParam.txtName.GetBuffer());
		LstParam.AddString(val);
		//AfxMessageBox(wndAddParam.txtName);
	}
}

BOOL CWndAddFunction::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for (auto& x : arrys)
	{
		LstType.AddString(x);
	}
	LstType.SetCurSel(TypeId);
	return TRUE;
}

void CWndAddFunction::Init(PCFUNContext val,PCOBJContext obj)
{
	_ParamContext = nullptr;
	if (val)
	{
		_ParamContext = val->Params;
	}
	
	_OBJContext = obj;
}


void CWndAddFunction::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	int typeId = LstType.GetCurSel();

	if (!_OBJContext->TestContextName(txtName))
	{
		AfxMessageBox(L"函数名称与变量名称重复!");
		return;
	}

	auto p = _ParamContext;
	CString valParams=L"";
	while (p)
	{
		valParams = valParams + p->txtType + L",";
		p = p->Next;
	}
	CString txtShot;
	txtShot.Format(L"%s(%s)", txtName.GetBuffer(), valParams.GetBuffer()); //生成基本信息

	

	if (!_OBJContext->TestFuncName(txtShot))
	{
		AfxMessageBox(L"函数与其他函数重复!");
		return;
	}

	if (txtNameEx == L"")
	{
		txtNameEx = arrys[LstType.GetCurSel()];
	}

	_FunReturn=_OBJContext->AddFunContext(txtName, txtNameEx, txtNote, L"", 0, _ParamContext);
	CDialogEx::OnOK();
}
