// ListenSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ListenSocket.h"
#include "ChatDlg.h"
#include "ClientDlg.h"


// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket 成员函数


void CListenSocket::OnAccept(int nErrorCode)
{
	CChatDlg *pNewDlg = new CChatDlg();
	CSessionSocket *pNewSocket = new CSessionSocket();
	if (Accept(*pNewSocket)) {
		pNewDlg->pChatSocket = pNewSocket;
		pNewDlg->is_connect = true;
		pNewDlg->Create(IDD_CHATDLG);
		pNewDlg->ShowWindow(SW_SHOW);
		pNewDlg->pChatSocket->AsyncSelect(FD_READ | FD_CLOSE);
		((CClientDlg*)(AfxGetApp()->GetMainWnd()))->m_chatList->AddTail(pNewDlg);
	}
	else {
		delete pNewDlg;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}
