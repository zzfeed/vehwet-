
// GAMEHACKERDlg.h: 头文件
//

#pragma once
#include "CWndINC.h"
#include "CWndINJ.h"
#include "CWndRAN.h"

#define MAX_PAGE_MAIN 3
// CGAMEHACKERDlg 对话框
class CGAMEHACKERDlg : public CDialogEx
{
// 构造
public:
	CGAMEHACKERDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMEHACKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	//
	CWndINC   PageINC;
	CWndINJ   PageINJ;
	CWndRAN   PageRAN;

	CDialogEx* Pages[MAX_PAGE_MAIN];
	//CString    PageName[MAX_PAGE];
	short      CurPage = 0;
	bool InstallPage(CDialogEx* wnd,int IDD_WND,CString&& _Name,BOOL IsShow=FALSE);


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl mTab;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	CString  wAppPath;
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void OnOK();
	void OnCancel();
	
	afx_msg void OnClose();
};
