#pragma once


// CWndAddFunction 对话框
#include "CWndAddParam.h"
#include "CFUNContext.h"
#include "COBJContext.h"
class CWndAddFunction : public CDialogEx
{
	DECLARE_DYNAMIC(CWndAddFunction)

public:
	CWndAddFunction(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndAddFunction();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDFUNCTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CWndAddParam wndAddParam;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int TypeId{6};
	virtual BOOL OnInitDialog();
	CComboBox LstType;

	PCOBJContext _OBJContext{};
	PCParamContext _ParamContext{};
	PCFUNContext   _FunReturn{};

	void Init(PCFUNContext val,PCOBJContext obj);
	CString txtName;
	CString txtNameEx;
	CString txtAddress;
	CListBox LstParam;
	CString txtNote;
	afx_msg void OnBnClickedOk();
};
