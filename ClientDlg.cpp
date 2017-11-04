
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "IPInfo.h"
#include "cJSON.h"
#include "Header.h"
#include "afxdialogex.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientDlg �Ի���



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


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	m_chatList = new CPtrList();
	if (!m_chatList) {
		AfxMessageBox(_T("�����Ự��ʧ��!"));
	}
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	SetWindowText(m_caption);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::UpdateUserInfo(CString strInfo)  //��ʾ�����û�
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
		if (json_root == NULL) {
			AfxMessageBox(_T("�յ����ݸ�ʽ����"));
			return;
		}
		char *name = cJSON_GetObjectItem(json_root, _NAME)->valuestring;
		char *status = cJSON_GetObjectItem(json_root, _STATUS)->valuestring;
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
	CSessionSocket* pSock = theApp.GetMainSocket();
	if (pSock->Is_Connect == FALSE) {
		CString ip(Server_IP);
		pSock->Connect(ip, Server_Port);
		pSock->Is_Connect = TRUE;
	}

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

	pSock->SendMSG(head, data);
}

void CClientDlg::OnGetIp(char *buf) {
	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buf);
	if (json_root == NULL) {
		return;
	}
	char *isOnline = cJSON_GetObjectItem(json_root, _ISONLINE)->valuestring;
	char *ip = cJSON_GetObjectItem(json_root, _IP)->valuestring;
	char *port = cJSON_GetObjectItem(json_root, _PORT)->valuestring;
	char *touser = cJSON_GetObjectItem(json_root, _TOUSER)->valuestring;
	CChatDlg *chatdlg;
	CString toUser(touser);
	chatdlg = new CChatDlg();
	chatdlg->m_caption = _T("�� ") + toUser + _T(" ������");
	chatdlg->m_strName = m_caption;
	chatdlg->to_user = toUser;
	chatdlg->to_IP = CString(ip);
	chatdlg->to_Port = atoi(port);
	m_chatList->AddTail(chatdlg);
	//chatdlg->DoModal(); ʹ��ģ̬�Ի����ʹԭ�Ի��򲻿���
	//�˴�Ӧʹ�÷�ģ̬�Ի���
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
			CString str = _T("�� ") + fromUser + _T(" ������");
			pTemp->SetCaption(str);
			pTemp->OnGetNewMsg(buf);
		}
	}
}

void CClientDlg::OnRcvOfflineMsg(HEADER head, char *buf) {
	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buf);
	if (json_root == NULL) {
		return;
	}
	char *offlineMsg = cJSON_GetObjectItem(json_root, _OFFLINEMSG)->valuestring;
	CString Str(offlineMsg);
	AfxMessageBox(Str);
}
