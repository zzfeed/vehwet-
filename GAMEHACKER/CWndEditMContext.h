#pragma once


// CWndEditMContext 对话框

class CWndEditMContext : public CDialogEx
{
	DECLARE_DYNAMIC(CWndEditMContext)

public:
	CWndEditMContext(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWndEditMContext();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITMEMCONTEXT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString txtAddress;
	CString txtOffSet;
	CComboBox LstType;
	CString txtName;
	unsigned Size;
	CString txtNote;
	CString txtData;
	int     TypeId;
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

	bool    NoEdit();
};
