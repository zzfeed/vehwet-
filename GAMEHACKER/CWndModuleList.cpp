// CWndModuleList.cpp: 实现文件
//

#include "pch.h"
#include "GAMEHACKER.h"
#include "CWndModuleList.h"
#include "afxdialogex.h"

#include <TlHelp32.h>
// CWndModuleList 对话框

IMPLEMENT_DYNAMIC(CWndModuleList, CDialogEx)

CWndModuleList::CWndModuleList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOD_LIST, pParent)
{

}

CWndModuleList::~CWndModuleList()
{
}

BOOL CWndModuleList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//
	//ExeLst
	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(LstModule.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(LstModule.m_hWnd, GWL_STYLE, lStyle);
	DWORD dStyle = LstModule.GetExtendedStyle();
	dStyle |= LVS_EX_FULLROWSELECT;
	dStyle |= LVS_EX_GRIDLINES;
	LstModule.SetExtendedStyle(dStyle);

	LstModule.InsertColumn(0, L"模块名称", 0, 100);
	LstModule.InsertColumn(1, L"基址", 0, 200);
	LstModule.InsertColumn(2, L"大小", 0, 200);
	LstModule.InsertColumn(3, L"文件位置", 0, 200);
	
	//
	return TRUE;
}

void CWndModuleList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, LstModule);
}


BEGIN_MESSAGE_MAP(CWndModuleList, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CWndModuleList::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CWndModuleList 消息处理程序


void CWndModuleList::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CWndModuleList::GetModList(DWORD Pid)
{
	LstModule.DeleteAllItems();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Pid);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modl{sizeof(MODULEENTRY32)};
		BOOL BMORE = Module32First(hSnap, &modl);
		while (BMORE)
		{
			CString txt;
			txt.Format(L"%X", modl.hModule);
			LstModule.InsertItem(0, modl.szModule);
			LstModule.SetItemText(0, 1, txt);
			txt.Format(L"%X", modl.modBaseSize);
			LstModule.SetItemText(0, 2, txt);
			LstModule.SetItemText(0, 3, modl.szExePath);
			BMORE = Module32Next(hSnap, &modl);
		}
	}
	CloseHandle(hSnap);
}
