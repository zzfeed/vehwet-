#pragma once
#include "CParamContext.h"

// CWndAddParam 对话框

class CWndAddParam : public CDialogEx
{
	DECLARE_DYNAMIC(CWndAddParam)

public:
	CWndAddParam(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndAddParam();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDPARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	int TypeId{5};
public:
	CComboBox LstType;
	CString txtName;
	CString txtNameEx;
	CString txtNote;

	CString txtTypeName;
	
	PCParamContext Params{};
	void Init(PCParamContext _params);
	afx_msg void OnBnClickedOk();
};
