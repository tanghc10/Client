
// Client.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "ChatDlg.h"
#include "LogInDlg.h"
#include "IPInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClientApp 构造

CClientApp::CClientApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CClientApp 对象

CClientApp theApp;


// CClientApp 初始化

BOOL CClientApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	SetRegistryKey(_T("ChatClient"));

	//初始化Socket
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("初始化Socket库失败！"));
		return false;
	}
	m_pSocket = new CSessionSocket();
	if (!m_pSocket)
	{
		AfxMessageBox(_T("内存不足！"));
		return false;
	}
	CString IP(Server_IP);
	if (m_pSocket->Create(0, SOCK_STREAM) == FALSE)
	{
		AfxMessageBox(_T("创建套接字失败！"));
		return false;
	}
	pLogoinDlg = new CLogInDlg();
	CString m_strUser;
	INT_PTR rsp = pLogoinDlg->DoModal();
	if (rsp == IDCANCEL)
	{
		delete pLogoinDlg;
		m_pSocket->Close();
		return false;
	}
	else
	{
		m_strUser = pLogoinDlg->m_strUser;
		delete pLogoinDlg;
	}
	CClientDlg dlg;
	dlg.m_caption = m_strUser;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		delete dlg;
	}
	else if (nResponse == IDCANCEL)
	{
		delete dlg;
	}
	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CClientApp::ExitInstance()
{
	if (m_pSocket)
	{
		delete m_pSocket;
		m_pSocket = NULL;
	}
	return CWinApp::ExitInstance();
}

CSessionSocket* CClientApp::GetMainSocket() const
{
	return m_pSocket;
}

void CClientApp::ListenToPort(int Port) {
	m_pListenSocket = new CListenSocket();
	if (!m_pListenSocket)
	{
		AfxMessageBox(_T("动态创建服务器套接字出错!"));
		return;
	}
	if (m_pListenSocket->Create(Port, SOCK_STREAM) == FALSE)
	{
		AfxMessageBox(_T("创建套接字失败!"), MB_OK | MB_ICONEXCLAMATION);
		m_pListenSocket->Close();
		return;
	}
	// 侦听成功，等待连接请求
	if (m_pListenSocket->Listen() == FALSE)
	{
		if (m_pListenSocket->GetLastError() == WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("网络侦听失败!"));//vs默认使用的是unicode字符编码集，而unicode要占2byte,通常的字符只占1byte,所以导致无法转换，故需要加上 _T("") 或 L"" 进行转换。
			m_pListenSocket->Close();
			return;
		}
	}
}