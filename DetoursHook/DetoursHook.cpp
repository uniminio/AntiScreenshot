// DetoursHook.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "DetoursHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

// CDetoursHookApp

BEGIN_MESSAGE_MAP(CDetoursHookApp, CWinApp)
END_MESSAGE_MAP()


extern BOOL injected = FALSE;
extern HHOOK g_hhk = nullptr;
extern HINSTANCE g_instance = nullptr;

// CDetoursHookApp 构造

CDetoursHookApp::CDetoursHookApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CDetoursHookApp 对象

CDetoursHookApp theApp;


// CDetoursHookApp 初始化

BOOL WINAPI NewMessageBoxA(HWND, LPCSTR, LPCSTR, UINT)
{
	return MyMessageBoxA(NULL, "Hook", "Hook", 0);
}

BOOL WINAPI NewBitBlt(
	HDC   hdc,
	int   x,
	int   y,
	int   cx,
	int   cy,
	HDC   hdcSrc,
	int   x1,
	int   y1,
	DWORD rop
)
{

	HWND hdesk = GetDesktopWindow();
	RECT deskRect;
	GetWindowRect(hdesk, &deskRect);
	if (cx == deskRect.right - deskRect.left && cy == deskRect.bottom - deskRect.top)
	{
		auto hclient =  FindWindow(NULL, L"AntiCapture");
		RECT clientRect;
		GetWindowRect(hclient, &clientRect);
		if (MyBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop))
		{
			if (MyBitBlt(hdc, clientRect.left, clientRect.top, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, hdcSrc, clientRect.left, clientRect.top, BLACKNESS))
			{
				return TRUE;
			}

		}
		return FALSE;
	}
	else
	{
		return MyBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	}
	return TRUE;
}

BOOL CDetoursHookApp::InitInstance()
{
	CWinApp::InitInstance();
	g_instance = AfxGetInstanceHandle();
	return Inject();
}

BOOL CDetoursHookApp::ExitInstance()
{
	return HookOff();
}

BOOL Inject()
{
	if (injected)
	{
		return TRUE;
	}
	injected = TRUE;
	return HookOn();
}

BOOL HookOn()
{
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)MyBitBlt, NewBitBlt);
	DetourTransactionCommit();
	return TRUE;
}

BOOL HookOff()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)MyBitBlt, NewBitBlt);
	DetourTransactionCommit();
	return TRUE;
}

LRESULT CALLBACK GetMsgProc(
	_In_ int    code,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	return CallNextHookEx(g_hhk, code, wParam, lParam);
}

BOOL StartGlobalHook()
{
	if (g_hhk == nullptr)
	{
		g_hhk = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, g_instance, 0);
	}
	return g_hhk != nullptr;
}

BOOL StopGlobalHook()
{
	HookOff();
	if (g_hhk != nullptr)
	{
		UnhookWindowsHookEx(g_hhk);
		g_hhk = nullptr;
	}
	return true;
}


