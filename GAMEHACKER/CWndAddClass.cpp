// CWndAddClass.cpp: 实现文件
//

#include "pch.h"
#include "GAMEHACKER.h"
#include "CWndAddClass.h"
#include "afxdialogex.h"


// CWndAddClass 对话框

IMPLEMENT_DYNAMIC(CWndAddClass, CDialogEx)

CWndAddClass::CWndAddClass(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDCLASS, pParent)
	, txtAddress(_T("400000"))
	, txtName(_T("UNNAME"))
	, txtNote(_T("未定义"))
{

}

CWndAddClass::~CWndAddClass()
{
}

void CWndAddClass::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, txtAddress);
	DDX_Text(pDX, IDC_EDIT2, Size);
	DDX_Text(pDX, IDC_EDIT3, txtName);
	DDX_Text(pDX, IDC_EDIT4, txtNote);
}


BEGIN_MESSAGE_MAP(CWndAddClass, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWndAddClass::OnBnClickedOk)
END_MESSAGE_MAP()


// CWndAddClass 消息处理程序


void CWndAddClass::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CDialogEx::OnOK();
}
