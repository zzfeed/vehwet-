// CWndEditMContext.cpp: 实现文件
//

#include "pch.h"
#include "GAMEHACKER.h"
#include "CWndEditMContext.h"
#include "afxdialogex.h"
#include "CMEMContext.h"

extern DATA_TYPE data_TYPE[12];
// CWndEditMContext 对话框

IMPLEMENT_DYNAMIC(CWndEditMContext, CDialogEx)

CWndEditMContext::CWndEditMContext(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDITMEMCONTEXT, pParent)
	, txtAddress(_T(""))
	, txtOffSet(_T(""))
	, txtName(_T(""))
	, txtNote(_T(""))
	, txtData(_T(""))
{

}

CWndEditMContext::~CWndEditMContext()
{
}

void CWndEditMContext::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, txtAddress);
	DDX_Text(pDX, IDC_EDIT5, txtOffSet);
	DDX_Control(pDX, IDC_COMBO1, LstType);
	DDX_Text(pDX, IDC_EDIT1, txtName);
	DDX_Text(pDX, IDC_EDIT2, Size);
	DDX_Text(pDX, IDC_EDIT3, txtNote);
	DDX_Text(pDX, IDC_EDIT6, txtData);
}

BOOL CWndEditMContext::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for (auto& x : data_TYPE)
	{
		LstType.AddString(x.UName);
	}
	LstType.SetCurSel(TypeId);
	return TRUE;
}


BEGIN_MESSAGE_MAP(CWndEditMContext, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWndEditMContext::OnBnClickedOk)
END_MESSAGE_MAP()


// CWndEditMContext 消息处理程序





void CWndEditMContext::OnBnClickedOk()
{
	if (NoEdit()) return CDialogEx::OnCancel();
	CDialogEx::OnOK();
}

bool CWndEditMContext::NoEdit()
{
	//UpdateData(FALSE);
	CString _txtName=txtName;
	unsigned _Size=Size;
	CString _txtNote=txtNote;
	int     _TypeId=TypeId;
	UpdateData(TRUE);
	TypeId = LstType.GetCurSel();
	return (_txtName == txtName) && (_Size == Size) && (_txtNote == txtNote) && (_TypeId == TypeId );
	
}
