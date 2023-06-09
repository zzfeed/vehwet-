// Dlls.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "Dlls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <tlhelp32.h>
//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CDllsApp

BEGIN_MESSAGE_MAP(CDllsApp, CWinApp)
END_MESSAGE_MAP()


// CDllsApp 构造

CDllsApp::CDllsApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}



// 唯一的 CDllsApp 对象

CDllsApp  theApp;
CDllsApp* PtheApp;

// CDllsApp 初始化
HHOOK keyHook;
LRESULT CALLBACK KeyCallBack(int nCode, WPARAM w, LPARAM l);


BOOL CDllsApp::InitInstance()
{
	
	CWinApp::InitInstance();
	PtheApp = this;
	keyHook = SetWindowsHook(WH_KEYBOARD, KeyCallBack);	
	return TRUE;
}

int CDllsApp::ExitInstance()
{
	CWinApp::ExitInstance();
	
	return 0;
}




LRESULT CALLBACK KeyCallBack(int nCode,WPARAM w,LPARAM l)
{

	if (nCode == 0)
	{
		if ((l & (1 << 31)) == 0)
		{
			switch (w)
			{
			case VK_HOME:

				if (PtheApp->wndMain == NULL)
				{

					PtheApp->wndMain = new CWndMain();
					PtheApp->wndMain->Create(IDD_WNDMAIN);

				}

				PtheApp->wndMain->ShowWindow(TRUE);
				break;
			}
		}
	}
	return CallNextHookEx(keyHook,nCode,w,l);
}

