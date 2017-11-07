
// Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "SessionSocket.h"
#include "ListenSocket.h"
#include "LogInDlg.h"

// CClientApp: 
// �йش����ʵ�֣������ Client.cpp
//

class CClientApp : public CWinApp
{
public:
	CClientApp();

// ��д
public:
	virtual BOOL InitInstance();

	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	CListenSocket *m_pListenSocket;//����socketָ��
	void ListenToPort(int Port);
	CSessionSocket *GetMainSocket() const;
	int ListenPort;
private:
	CSessionSocket *m_pSocket;
	CLogInDlg* pLogoinDlg;
};

extern CClientApp theApp;
