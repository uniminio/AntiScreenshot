// DetoursHook.h: DetoursHook DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号


// CDetoursHookApp
// 有关此类实现的信息，请参阅 DetoursHook.cpp
//

static BOOL(WINAPI *MyMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT) = MessageBoxA;

static BOOL(WINAPI *MyBitBlt)(
	HDC   hdc,
	int   x,
	int   y,
	int   cx,
	int   cy,
	HDC   hdcSrc,
	int   x1,
	int   y1,
	DWORD rop
	) = BitBlt;

class CDetoursHookApp : public CWinApp
{
public:
	CDetoursHookApp();

	// 重写
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

	DECLARE_MESSAGE_MAP()
};



BOOL Inject();
BOOL HookOn();
BOOL HookOff();

BOOL StartGlobalHook();
BOOL StopGlobalHook();
