
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "cJSON.h"
#include "Header.h"
#include "afxdialogex.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LB_ONLINE, m_listBox);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_DBLCLK(IDC_LB_ONLINE, &CClientDlg::On_choose)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	m_chatList = new CPtrList();
	if (!m_chatList) {
		AfxMessageBox(_T("创建会话表失败!"));
	}
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetWindowText(m_caption);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::UpdateUserInfo(CString strInfo)  //显示所有用户
{
	CString strTmp;
	CString m_strUserName = theApp.GetMainSocket()->m_strUserName;
	m_listBox.ResetContent();

	while (!strInfo.IsEmpty())
	{
		int n = strInfo.Find('#');
		if (n == -1)
			break;
		strTmp = strInfo.Left(n);
		_bstr_t b(strTmp);
		char *data = b;
		cJSON *json_root = NULL;
		json_root = cJSON_Parse(data);
		char *name = cJSON_GetObjectItem(json_root, "name")->valuestring;
		char *status = cJSON_GetObjectItem(json_root, "status")->valuestring;
		CString Name(name);
		if (Name.Compare(m_strUserName) != 0)
		{
			if (*status == 'Y') {
				m_listBox.InsertString(0, Name, RGB(0, 250, 47));
			}
			else {
				m_listBox.AddString(Name);
			}
		}
		strInfo = strInfo.Right(strInfo.GetLength() - n - 1);
	}
}

void CClientDlg::On_choose()
{
	CString tep(_T(""));
	int nIndex = 0;
	nIndex = m_listBox.GetCurSel();
	if (LB_ERR == nIndex) {
		return;
	}
	m_listBox.GetText(nIndex, tep);
	DWORD m_dwIP = ntohl(inet_addr("192.168.11.1"));
	CSessionSocket* pSock = theApp.GetMainSocket();
	//if (pSock->Is_Connect == FALSE) {
		IN_ADDR addr;
		addr.S_un.S_addr = htonl(m_dwIP);
		//inet_ntoa返回一个char *,而这个char *的空间是在inet_ntoa里面静态分配
		CString strIP(inet_ntoa(addr));
		//开始只是创建了，并没有连接，这里连接socket，这个5050端口要和服务端监听的端口一直，否则监听不到的。
		CString ip = _T("192.168.11.1");
		pSock->Connect(ip, 5050);
		pSock->Is_Connect = TRUE;
	//}

	cJSON *json_root = NULL;
	CString str = _T("{\"touser\":\"") + tep + _T("\"}");
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *data = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, data, len, NULL, NULL);

	HEADER head;
	head.type = MSG_GETIP;
	head.nContentLen = len + 1;
	memset(head.to_user, 0, sizeof(head.to_user));
	_bstr_t tmp(m_caption);
	char *name = tmp;
	strcpy(head.to_user, "Server");
	memset(head.from_user, 0, sizeof(head.from_user));
	strcpy(head.from_user, name);

	pSock->Send(&head, sizeof(head));
	pSock->Send(data, len + 1);
}

BOOL CClientDlg::WChar2MByte(LPCWSTR lpSrc, LPSTR lpDest, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, 0, 0, FALSE);  //返回缓冲区大小
	if (nlen<n)
		return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, nlen, 0, FALSE);   //转换
	return TRUE;
}

void CClientDlg::OnGetIp(char *buf) {
	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buf);
	if (json_root == NULL) {
		return;
	}
	char *isOnline = cJSON_GetObjectItem(json_root, "isOnline")->valuestring;
	char *ip = cJSON_GetObjectItem(json_root, "ip")->valuestring;
	char *port = cJSON_GetObjectItem(json_root, "port")->valuestring;
	char *touser = cJSON_GetObjectItem(json_root, "touser")->valuestring;
	CChatDlg *chatdlg;
	CString toUser(touser);
	chatdlg = new CChatDlg();
	chatdlg->m_caption = _T("与 ") + toUser + _T(" 的聊天");
	chatdlg->m_strName = m_caption;
	chatdlg->to_user = toUser;
	chatdlg->to_IP = CString(ip);
	chatdlg->to_Port = atoi(port);
	m_chatList->AddTail(chatdlg);
	//chatdlg->DoModal(); 使用模态对话框会使原对话框不可用
	//此处应使用非模态对话框
	chatdlg->Create(IDD_CHATDLG);
	chatdlg->ShowWindow(SW_SHOW);
}

void CClientDlg::GetNewMsg(HEADER head, char * buf) {
	CString toUser(head.to_user);
	POSITION ps = m_chatList->GetHeadPosition();
	while (ps != NULL) {
		CChatDlg* pTemp = (CChatDlg*)m_chatList->GetNext(ps);
		if (pTemp->pChatSocket->m_strUserName == toUser) {
			CString fromUser(head.from_user);
			CString toUser(head.to_user);
			pTemp->m_strName = toUser;
			pTemp->to_user = fromUser;
			CString str = _T("与 ") + fromUser + _T(" 的聊天");
			pTemp->SetCaption(str);
			pTemp->OnGetNewMsg(buf);
		}
	}
}
