// CWndMain.cpp: 实现文件
//

#include "pch.h"
#include "Dlls.h"
#include "CWndMain.h"



// CWndMain 对话框

IMPLEMENT_DYNAMIC(CWndMain, CDialogEx)

CWndMain::CWndMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WNDMAIN, pParent)
	, wInfo(_T(""))
{

}

CWndMain::~CWndMain()
{
}

void CWndMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, wInfo);
}


BEGIN_MESSAGE_MAP(CWndMain, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndMain::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWndMain::OnBnClickedButton2)
END_MESSAGE_MAP()


// CWndMain 消息处理程序


void CWndMain::OnBnClickedButton1()
{
	int* hp = (int*)(0x4CEF18);
	wInfo.Format(L"HP:%d/%d", hp[0], hp[1]);
	UpdateData(FALSE);
	unsigned decx   = 0x4CEF08;
	unsigned dcall = 0x41FD40;
	_asm
	{
	    push 0x02
		push 0x9999
		mov  ecx,decx
		call dcall
	}

	
	//HOOK ..... 

}

bool Wudi(HOOKREFS2)
{
	if (_ESI == 0x4CEF08)
	{
		_EDI = 0;
	}
	return true;
}

bool NewWudi(PCPUINFO e)
{
	return true;
}


void CWndMain::OnBnClickedButton2()
{
	//hook.SetHook((LPVOID)0x41FDB2, 3, Wudi,(LPVOID)0x41FDB5);// Wudi, 6, 0);
	//hook.SetHook((LPVOID)0x41FD2C, NewWudi, 5, (LPVOID)0x41FE40);
	//hook.SetHook(FindWindowA,)
	//hook.SetHook((LPVOID)0x41FDB2, Wudi, 0);
	//hook.Init();
	
	//htdModule* p{};
	//p->HideDll(L"Dlls.dll");
	
	
}
