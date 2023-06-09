// CWndProcess.cpp: 实现文件
//

#include "pch.h"
#include "GAMEHACKER.h"
#include "CWndProcess.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "CWndINC.h"

CWndINC* wndINC;

// CWndProcess 对话框

IMPLEMENT_DYNAMIC(CWndProcess, CDialogEx)

CWndProcess::CWndProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_LST, pParent)
{

}

CWndProcess::~CWndProcess()
{
}

BOOL CWndProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//
	//ExeLst
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(ProcessLst.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(ProcessLst.m_hWnd, GWL_STYLE, lStyle);
	DWORD dStyle = ProcessLst.GetExtendedStyle();
	dStyle |= LVS_EX_FULLROWSELECT;
	dStyle |= LVS_EX_GRIDLINES;
	ProcessLst.SetExtendedStyle(dStyle);

	ProcessLst.InsertColumn(0, L"PID", 0, 100);
	ProcessLst.InsertColumn(1, L"文件名", 0, 200);
	RefreshProcess();
	//
	return TRUE;
}

void CWndProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, ProcessLst);
}

void CWndProcess::RefreshProcess()
{
	ProcessLst.DeleteAllItems();//

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(L"获取进程列表失败!检查是否具备管理员权限!");
		return;
	}

	PROCESSENTRY32 pe{sizeof(pe)};
	BOOL BMORE=Process32First(hSnap, &pe);
	while (BMORE)
	{
		CString txt;
		txt.Format(L"%d", pe.th32ProcessID);
		ProcessLst.InsertItem(0,txt);
		ProcessLst.SetItemText(0, 1, pe.szExeFile);
		BMORE = Process32Next(hSnap, &pe);
	}

	CloseHandle(hSnap);
}


BEGIN_MESSAGE_MAP(CWndProcess, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CWndProcess::OnNMDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CWndProcess::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CWndProcess 消息处理程序


void CWndProcess::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NM_LISTVIEW* View = (NM_LISTVIEW*)pNMHDR;
	if (View->iItem != -1)
	{
		wPid = ProcessLst.GetItemText(View->iItem,0);
		wExe = ProcessLst.GetItemText(View->iItem, 1);
		dPid = wcstoul(wPid.GetBuffer(), 0, 10);
		OnOK();
	}
	
}


void CWndProcess::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


