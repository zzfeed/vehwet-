
// GAMEHACKERDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GAMEHACKER.h"
#include "GAMEHACKERDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CGAMEHACKERDlg 对话框



CGAMEHACKERDlg::CGAMEHACKERDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAMEHACKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CGAMEHACKERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, mTab);
}

BEGIN_MESSAGE_MAP(CGAMEHACKERDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CGAMEHACKERDlg::OnTcnSelchangeTab1)
	
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGAMEHACKERDlg 消息处理程序



BOOL CGAMEHACKERDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	TCHAR    FileModule[0x100]{};
	GetModuleFileName(NULL, FileModule, 0x100);
	int i;
	for (i = 0x100 - 1; FileModule[i] != L'\\'; i--);
	FileModule[i] = 0;
	wAppPath.Format(L"%s", FileModule);
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	

	InstallPage(&PageINJ, IDD_PAGE_0,L"注入",TRUE);
	InstallPage(&PageINC, IDD_PAGE_1, L"注入检测");
	InstallPage(&PageRAN, IDD_PAGE_2, L"分析工具");

	PageINJ.Init(wAppPath);
	PageRAN.SetAppPath(wAppPath);
	
	//
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

bool CGAMEHACKERDlg::InstallPage(CDialogEx* wnd, int IDD_WND, CString&& _Name,BOOL IsShow)
{

	if (CurPage >= MAX_PAGE_MAIN) return false;
	Pages[CurPage] = wnd;
	Pages[CurPage]->Create(IDD_WND,this);
	//Pages[CurPage]->SetParent(this);
	Pages[CurPage]->ShowWindow(IsShow);

	CRect rect;
	mTab.GetClientRect(&rect);
	rect.top += 46;
	rect.left += 20;
	rect.bottom += 5;
	rect.right += 5;
	Pages[CurPage]->MoveWindow(&rect);
	mTab.InsertItem(CurPage, _Name);

	CurPage++;
	return true;
}

void CGAMEHACKERDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGAMEHACKERDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGAMEHACKERDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGAMEHACKERDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int n = mTab.GetCurSel();
	for (int i = 0; i < CurPage; i++)
	{
		Pages[i]->ShowWindow(i==n);
	}
}



void CGAMEHACKERDlg::OnSize(UINT nType, int cx, int cy)
{

	
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == 1) return;
	CRect rect;
	mTab.GetClientRect(&rect);
	rect.top += 46;
	rect.left += 20;
	rect.bottom += 5;
	rect.right += 5;
	for (int i = 0; i < CurPage; i++)
	{
		if (Pages[i])Pages[i]->MoveWindow(&rect);
	}
	
}

void CGAMEHACKERDlg::OnOK()
{
}

void CGAMEHACKERDlg::OnCancel()
{
	
}


//WM_CLOSE

void CGAMEHACKERDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnCancel();
}






