#pragma once


// CWndINC 对话框
#include "CWndProcess.h"
#include "CWndModuleList.h"
#include "CWndModAnly.h"
#define MAX_PAGE 3
class CWndINC : public CDialogEx
{
	DECLARE_DYNAMIC(CWndINC)

public:
	CWndINC(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndINC();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_1 };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CWndProcess  wndProcess;
	afx_msg void OnBnClickedButton2();
	
	//
	
	CString  UExe;
	CString  UExeRoot;
	CTabCtrl mTab;
	int CurPage{};
	CWndModuleList modPage[2];
	CWndModAnly    modAnly;
	CDialogEx*     Pages[MAX_PAGE];
	bool InstallPage(CDialogEx* wnd, int IDD_WND, CString&& _Name, BOOL IsShow);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton3();
};
