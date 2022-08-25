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

class CDetoursHookApp : public CWinApp
{
public:
	CDetoursHookApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
