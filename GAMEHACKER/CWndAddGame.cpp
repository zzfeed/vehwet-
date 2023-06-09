// CWndAddGame.cpp: 实现文件
//

#include "pch.h"
#include "GAMEHACKER.h"
#include "CWndAddGame.h"
#include "CWndINJ.h"
#include "afxdialogex.h"


// CWndAddGame 对话框

IMPLEMENT_DYNAMIC(CWndAddGame, CDialogEx)

CWndAddGame::CWndAddGame(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDGAME, pParent)
	, GameName(_T(""))
	, GamePath(_T(""))
	, GameFullPath(_T(""))
	, GameCmds(_T(""))
	, DllPath(_T(""))
{

}

CWndAddGame::~CWndAddGame()
{
}

void CWndAddGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, GameName);
	DDX_Text(pDX, IDC_EDIT2, GamePath);
	DDX_Text(pDX, IDC_EDIT3, GameFullPath);
	DDX_Text(pDX, IDC_EDIT4, GameCmds);
	DDX_Text(pDX, IDC_EDIT5, DllPath);
}


BEGIN_MESSAGE_MAP(CWndAddGame, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndAddGame::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWndAddGame::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CWndAddGame::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CWndAddGame::OnBnClickedButton3)
END_MESSAGE_MAP()


// CWndAddGame 消息处理程序


void CWndAddGame::OnBnClickedButton1()
{
	UpdateData(TRUE);

	TCHAR szFilters[] = _T("exe (*.exe)|*.exe|dat (*.dat)|*.dat|All Files (*.*)|*.*||");
	CFileDialog dialog(TRUE,NULL,NULL,6,szFilters);
	if (dialog.DoModal() == IDOK)
	{
		GameFullPath = dialog.GetPathName();
		GameName = dialog.GetFileTitle();
		GamePath = dialog.GetFolderPath()+L"\\";
		GameCmds = L"";
		UpdateData(FALSE);
	}

}


void CWndAddGame::OnBnClickedButton2()
{
	UpdateData(TRUE);
	TCHAR szFilters[] = _T("动态链接库 (*.dll)|*.dll");
	CFileDialog dialog(TRUE, NULL, NULL, 6, szFilters);
	if (dialog.DoModal() == IDOK)
	{
		DllPath = dialog.GetPathName();
		UpdateData(FALSE);
	}
}


void CWndAddGame::OnBnClickedButton4()
{
	CDialog::OnCancel();
}


void CWndAddGame::OnBnClickedButton3()
{
	CWndINJ* p = (CWndINJ*)father;
	UpdateData(TRUE);

	if (IsCreate) //函数指针
	{
		p->AddGame(GameName, GameFullPath, GamePath, GameCmds, DllPath);
	}
	else
	{
		p->SaveGame(GameName, GameFullPath, GamePath, GameCmds, DllPath , index);
	}

	CDialog::OnCancel();
}

void CWndAddGame::Init(void* _father,bool Create,int _index)
{
	father = _father;
	IsCreate = Create;
	index = _index;
}

void CWndAddGame::SetData(CString& _GameName, CString& _GamePath, CString& _GameExe, CString& _GameCmds, CString& _DllPath)
{
	GameName = _GameName;
	GamePath = _GamePath;
	GameFullPath = _GameExe;
	GameCmds = _GameCmds;
	DllPath = _DllPath;
    //UpdateData(FALSE);
}
