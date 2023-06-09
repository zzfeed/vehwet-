// CWndINC.cpp: 实现文件
//

#include "pch.h"
#include "GAMEHACKER.h"
#include "CWndINC.h"
#include "afxdialogex.h"


// CWndINC 对话框

IMPLEMENT_DYNAMIC(CWndINC, CDialogEx)

CWndINC::CWndINC(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_1, pParent)
	, UExe(_T(""))
	, UExeRoot(_T(""))
{

}

CWndINC::~CWndINC()
{
}

BOOL CWndINC::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//mTab.InsertItem(0, L"正常进程");
	//mTab.InsertItem(1, L"感染进程");
	//mTab.InsertItem(2, L"分析结果");

	//

	InstallPage(&modPage[0], IDD_MOD_LIST, L"正常进程", TRUE);
	InstallPage(&modPage[1], IDD_MOD_LIST, L"感染进程", FALSE);
	InstallPage(&modAnly, IDD_MOD_ANLY, L"分析对比", FALSE);
	return TRUE;
}

void CWndINC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, UExe);
	DDX_Text(pDX, IDC_EDIT2, UExeRoot);
	DDX_Control(pDX, IDC_TAB1, mTab);
}


BEGIN_MESSAGE_MAP(CWndINC, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndINC::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWndINC::OnBnClickedButton2)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CWndINC::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON3, &CWndINC::OnBnClickedButton3)
END_MESSAGE_MAP()


// CWndINC 消息处理程序


void CWndINC::OnBnClickedButton1()
{
	
	
	if (wndProcess.DoModal() == IDOK)
	{
		UExe = wndProcess.wExe;
		modPage[0].GetModList(wndProcess.dPid);
		UpdateData(FALSE);
	};
	// TODO: 在此添加控件通知处理程序代码
}


void CWndINC::OnBnClickedButton2()
{
	if (wndProcess.DoModal() == IDOK)
	{
		UExeRoot = wndProcess.wExe;
		modPage[1].GetModList(wndProcess.dPid);
		UpdateData(FALSE);
	};
}



bool CWndINC::InstallPage(CDialogEx* wnd, int IDD_WND, CString&& _Name, BOOL IsShow)
{
	if (CurPage >= MAX_PAGE) return false;
	Pages[CurPage] = wnd;
	Pages[CurPage]->Create(IDD_WND, &mTab);
	//Pages[CurPage]->SetParent(this);
	Pages[CurPage]->ShowWindow(IsShow);

	CRect rect;
	mTab.GetClientRect(&rect);
	rect.top += 0;
	rect.left += 0;
	rect.bottom += 0;
	rect.right += 0;
	Pages[CurPage]->MoveWindow(&rect);
	mTab.InsertItem(CurPage, _Name);

	CurPage++;
	return true;
}




void CWndINC::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == 1) return;
	CRect rect;
	mTab.GetClientRect(&rect);
	rect.top += 0;
	rect.left += 0;
	rect.bottom -= 30;
	rect.right += 0;
	for (int i = 0; i < CurPage; i++)
	{
		if (Pages[i])Pages[i]->MoveWindow(&rect);
	}

	// TODO: 在此处添加消息处理程序代码
}


void CWndINC::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	int n = mTab.GetCurSel();
	for (int i = 0; i < CurPage; i++)
	{
		Pages[i]->ShowWindow(i == n);
	}
}


void CWndINC::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	// 开始分析比对
	//modPage[0].
	int CountNormal = modPage[0].LstModule.GetItemCount();
	int CountRoot   = modPage[1].LstModule.GetItemCount();
	if ((CountNormal) && (CountRoot))
	{
		modAnly.LstAnly.DeleteAllItems();
		for (int i = 0; i < CountNormal; i++)
		{
			
			CString txt = modPage[0].LstModule.GetItemText(i, 3);
			for (int y = 0; y < CountRoot; y++)
			{
				if (modPage[1].LstModule.GetItemText(y, 3).MakeUpper() == txt.MakeUpper())
				{
					//找到了  1 正常
					//        2 减少
					//        3 多余
					modPage[1].LstModule.SetItemData(y, 1);
					modPage[0].LstModule.SetItemData(i, 1);
					break;
				}
			}
		}

		//比对完成
		CString wTxt[2]{ L"减少",L"正常" };
		DWORD   dState[2]{ 0,2 };

		for (int i = 0; i < CountNormal; i++)
		{
			DWORD state = modPage[0].LstModule.GetItemData(i); //0 1
			

			modAnly.LstAnly.InsertItem (0, modPage[0].LstModule.GetItemText(i, 0));
			modAnly.LstAnly.SetItemText(0, 1,modPage[0].LstModule.GetItemText(i, 1));
			modAnly.LstAnly.SetItemText(0, 2, modPage[0].LstModule.GetItemText(i, 2));
			modAnly.LstAnly.SetItemText(0, 3, modPage[0].LstModule.GetItemText(i, 3));
			modAnly.LstAnly.SetItemText(0, 4, wTxt[state]);
			modAnly.LstAnly.SetItemData(0, dState[state]);
			//modPage[0].LstModule.GetItemText(i, 0);
		}

		for (int i = 0; i < CountRoot; i++)
		{
			DWORD state = modPage[1].LstModule.GetItemData(i);
			if (state == 0)
			{

				modAnly.LstAnly.InsertItem(0, modPage[1].LstModule.GetItemText(i, 0));
				modAnly.LstAnly.SetItemText(0, 1, modPage[1].LstModule.GetItemText(i, 1));
				modAnly.LstAnly.SetItemText(0, 2, modPage[1].LstModule.GetItemText(i, 2));
				modAnly.LstAnly.SetItemText(0, 3, modPage[1].LstModule.GetItemText(i, 3));
				modAnly.LstAnly.SetItemText(0, 4, L"增加");
				modAnly.LstAnly.SetItemData(0, 1);
				//0 1
				
			}
		}
	}
}
