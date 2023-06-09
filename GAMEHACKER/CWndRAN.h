#pragma once
#include "CWndAddClass.h"
#include "CWndProcess.h"
#include "COBJContext.h"
#include "CWndEditMContext.h"
#include "CWndAddFunction.h"
// CWndRAN 对话框

class CWndRAN : public CDialogEx
{
	DECLARE_DYNAMIC(CWndRAN)

public:
	CWndRAN(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndRAN();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	void OnOk();
	void OnCancel();
public:
	//
	CWndAddClass          wndAddClass;
	CWndProcess           wndSelProcess;
	CWndEditMContext      wndEditMC;
	CWndAddFunction       wndAddFunction;

	CTreeCtrl mTree;
	CListCtrl LstData;
	afx_msg void OnBnClickedButton1();

	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLoadGame();

	HANDLE   hProcess{};
	CString  wAppPath;
	CString  wAnlyPath;
	CString  wAnlyData;



	void SetAppPath(const wchar_t* path);
	HTREEITEM InsertItemEx(const wchar_t* txt, DWORD _menuId ,LPVOID _data , HTREEITEM h = TVI_ROOT);
	void      DeleteItemEx(HTREEITEM h);
	afx_msg void OnAddClass();
	afx_msg void OnDeleteClass();

	HTREEITEM GetFirstItem(HTREEITEM h);
	HTREEITEM GetSonItem(HTREEITEM h,int index);
	afx_msg void OnSetClass();
	PCOBJContext CurOBJ{};
protected:
	PCOBJContext GetOBJPtr(HTREEITEM h);
	HTREEITEM    AddOBJTree(PCOBJContext obj);
	void         SetOBJTree(HTREEITEM _h,PCOBJContext obj);
	
	void        AddMEMList(PCMEMContext context, DWORD index, DWORD lstIndex,DWORD_PTR exId);

public:
	afx_msg void OnAnlyClass();
	void         SHOWMemContext();
	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetGIDUNICODE();
	afx_msg void OnSetGIDASCII();
	afx_msg void OnSetGID8();
	afx_msg void OnSetGID4();
	afx_msg void OnSetGID2();
	afx_msg void OnSetGID1();
	afx_msg void OnSetGIDP();
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnSaveOBJ();
	afx_msg void OnAddFunction();
};
