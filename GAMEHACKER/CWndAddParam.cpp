// CWndAddParam.cpp: 实现文件
//

#include "pch.h"
#include "GAMEHACKER.h"
#include "CWndAddParam.h"
#include "afxdialogex.h"
#include "CMEMContext.h"

extern DATA_TYPE data_TYPE[12];

// CWndAddParam 对话框

IMPLEMENT_DYNAMIC(CWndAddParam, CDialogEx)

CWndAddParam::CWndAddParam(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDPARAM, pParent)
	, txtName(_T(""))
	, txtNameEx(_T(""))
	, txtNote(_T(""))
{

}

CWndAddParam::~CWndAddParam()
{
}

void CWndAddParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, LstType);
	DDX_Text(pDX, IDC_EDIT1, txtName);
	DDX_Text(pDX, IDC_EDIT3, txtNameEx);
	DDX_Text(pDX, IDC_EDIT2, txtNote);
}

BOOL CWndAddParam::OnInitDialog()
{

		CDialogEx::OnInitDialog();
		for (auto& x : data_TYPE)
		{
			LstType.AddString(x.UName);
		}
		LstType.SetCurSel(TypeId);
		return TRUE;

}

void CWndAddParam::Init(PCParamContext _params)
{
	Params = _params;
}


BEGIN_MESSAGE_MAP(CWndAddParam, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWndAddParam::OnBnClickedOk)
END_MESSAGE_MAP()


// CWndAddParam 消息处理程序


void CWndAddParam::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (txtNameEx == L"")
	{
		auto sel = LstType.GetCurSel();
		txtNameEx.Format(L"%s" , data_TYPE[sel].UName);
		txtTypeName = txtNameEx;
	}
	else
	{
		txtTypeName = txtNameEx;
	}

	while (Params)
	{
		if (Params->txtName == txtName)
		{
			AfxMessageBox(L"参数名称重复!");
			return;
		}
		Params = Params->Next;
	}

	CDialogEx::OnOK();
}
