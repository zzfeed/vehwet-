// CWndMain.cpp: 实现文件
//

#include "pch.h"
#include "CWndMain.h"
#include "afxdialogex.h"


// CWndMain 对话框

IMPLEMENT_DYNAMIC(CWndMain, CDialogEx)

CWndMain::CWndMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN, pParent)
{

}

CWndMain::~CWndMain()
{
}

void CWndMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndMain, CDialogEx)
END_MESSAGE_MAP()


// CWndMain 消息处理程序
