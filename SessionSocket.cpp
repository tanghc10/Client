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

void CSessionSocket::OnReceive(int nErrorCode)
{
	//先获取头部信息
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
	/*根据头部信息中nContentLen的值动态分配空间接收后续的信息*/
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
	/*根据头部信息中的类型将信息分发到不同的地方处理*/
	switch (head.type)
	{
		case MSG_UPDATE:
			((CClientDlg*)(AfxGetApp()->GetMainWnd()))->UpdateUserInfo(strText);
			break;
		case MSG_REGIST:
			((CRegisterDlg*)(AfxGetApp()->GetMainWnd()))->getRegistMsg(pBuff);
			break;
		case MSG_LOGIN:
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
		case MSG_FILEINFO:
			((CChatDlg*)(AfxGetApp()->GetMainWnd()))->On_RcvFileMsg(head, pBuff);
			break;
		default: break;
	}
	delete pBuff;
	pBuff = NULL;
	CSocket::OnReceive(nErrorCode);
}

BOOL CSessionSocket::SendMSG(HEADER head, char* data)
{
	int i = Send(&head, sizeof(HEADER));
	if (i == SOCKET_ERROR)
	{
		AfxMessageBox(_T("Send::head 发送错误！"));
		return FALSE;
	};
	if (Send(data, head.nContentLen) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("Send::data 发送错误！"));
		return FALSE;
	};
	return  TRUE;
}

void CSessionSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	Is_Connect = FALSE;
	CSocket::OnClose(nErrorCode);
}
