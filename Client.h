
// Client.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "SessionSocket.h"
#include "ListenSocket.h"
#include "LogInDlg.h"

// CClientApp: 
// 有关此类的实现，请参阅 Client.cpp
//

class CClientApp : public CWinApp
{
public:
	CClientApp();

// 重写
public:
	virtual BOOL InitInstance();

	virtual int ExitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
public:
	CListenSocket *m_pListenSocket;//监听socket指针
	void ListenToPort(int Port);
	CSessionSocket *GetMainSocket() const;
	int ListenPort;
private:
	CSessionSocket *m_pSocket;
	CLogInDlg* pLogoinDlg;
};

extern CClientApp theApp;
