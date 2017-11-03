// SessionSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "SessionSocket.h"
#include "Header.h"
#include "ClientDlg.h"
#include "ResetPswDlg.h"
#include "ChatDlg.h"

// CSessionSocket

CSessionSocket::CSessionSocket()
{
	Is_Connect = FALSE;
}

CSessionSocket::~CSessionSocket()
{
}


// CSessionSocket 成员函数


void CSessionSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	HEADER head;
	char* pHead = NULL;
	pHead = new char[sizeof(head)];
	memset(pHead, 0, sizeof(head));
	Receive(pHead, sizeof(head));

	head.type = ((LPHEADER)pHead)->type;
	head.nContentLen = ((LPHEADER)pHead)->nContentLen;
	memset(head.to_user, 0, sizeof(head.to_user));
	strcpy(head.to_user, ((LPHEADER)pHead)->to_user);
	memset(head.from_user, 0, sizeof(head.from_user));
	strcpy(head.from_user, ((LPHEADER)pHead)->from_user);
	delete pHead;
	pHead = NULL;

	char* pBuff = NULL;
	pBuff = new char[head.nContentLen];
	if (!pBuff)
	{
		AfxMessageBox(_T("内存不足！"));
		return;
	}
	memset(pBuff, 0, sizeof(char)*head.nContentLen);
	if (head.nContentLen != Receive(pBuff, head.nContentLen))
	{
		AfxMessageBox(_T("收到数据有误！"));
		delete pBuff;
		return;
	}
	CString strText(pBuff);
	switch (head.type)
	{
		case MSG_UPDATE:
		{
			CString strText(pBuff);
			((CClientDlg*)(AfxGetApp()->GetMainWnd()))->UpdateUserInfo(strText);
		}
		break;
		case MSG_REGIST:
			((CRegisterDlg*)(AfxGetApp()->GetMainWnd()))->getRegistMsg(pBuff);
			break;
		case MSG_LOGOIN:
			((CLogInDlg*)(AfxGetApp()->GetMainWnd()))->RvcFromServer(pBuff);
			break;
		case MSG_GETQUE:
			((CResetPswDlg*)(AfxGetApp()->GetMainWnd()))->RcvQuestion(pBuff);
			break;
		case MSG_RESET:
			((CResetPswDlg*)(AfxGetApp()->GetMainWnd()))->On_GetReset(pBuff);
			break;
		case MSG_GETIP:
			((CClientDlg*)(AfxGetApp()->GetMainWnd()))->OnGetIp(pBuff);
			break;
		case MSG_SEND:
		{
			CString Name(head.to_user);
			m_strUserName = Name;
			((CClientDlg*)(AfxGetApp()->GetMainWnd()))->GetNewMsg(head, pBuff);
			break;
		}
		case MSG_OFFLINE:
			((CClientDlg*)(AfxGetApp()->GetMainWnd()))->OnRcvOfflineMsg(head, pBuff);
			break;
		default: break;
	}
	delete pBuff;
	CAsyncSocket::OnReceive(nErrorCode);
}

BOOL CSessionSocket::SendMSG(LPSTR lpBuff, int nlen, char to_user[20], char from_user[20])
{
	//生成协议头
	HEADER head;
	head.type = MSG_SEND;
	head.nContentLen = nlen;
	strcpy(head.to_user, to_user);
	strcpy(head.from_user, from_user);

	int i = Send(&head, sizeof(HEADER));
	if (i == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送错误！"));
		return FALSE;
	};
	if (Send(lpBuff, nlen) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送错误！"));
		return FALSE;
	};

	return  TRUE;
}
//用户登陆
BOOL CSessionSocket::LogoIn(LPSTR lpBuff, int nlen, char from_user[20])
{
	HEADER _head;
	_head.type = MSG_LOGOIN;  //头部类型
	_head.nContentLen = nlen; //长度
	memset(_head.to_user, 0, 20);
	memset(_head.from_user, 0, 20);
	strcpy(_head.from_user, from_user);

	CSessionSocket* pSock = theApp.GetMainSocket();
	pSock->Send((char *)&_head, sizeof(_head));
	pSock->Send(lpBuff, nlen);

	return TRUE;
}

void CSessionSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	Is_Connect = FALSE;
	CAsyncSocket::OnClose(nErrorCode);
}
