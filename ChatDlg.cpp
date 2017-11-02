// ChatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "Header.h"
#include "ChatDlg.h"
#include "afxdialogex.h"

// LogInDlg �Ի���

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
	, m_strSend(_T(""))
	, m_strName(_T(""))
{
	is_connect = FALSE;
}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ET_SEND, m_strSend);
	DDX_Control(pDX, IDC_ET_TEXT, m_edit_text);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CChatDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// ChatDlg ��Ϣ�������
BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(m_caption);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChatDlg::OnBnClickedBtnSend()
{
	//������Ϣ
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
	{
		AfxMessageBox(_T("�������ݲ���Ϊ�գ�"));
		return;
	}
	CTime time;
	time = CTime::GetCurrentTime();  //��ȡ����ʱ��
	CString strTime = time.Format("%Y-%m-%d %H:%M:%S ");
	m_strSend = strTime + m_strName + _T("��\r\n") + m_strSend + _T("\r\n");
	int len = WideCharToMultiByte(CP_ACP, 0, m_strSend, -1, NULL, 0, NULL, NULL);
	char *data = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_strSend, -1, data, len, NULL, NULL);

	if (is_connect == FALSE) {
		pChatSocket = new CSessionSocket();
		if (!pChatSocket)
		{
			AfxMessageBox(_T("�ڴ治�㣡"));
			return;
		}

		if (pChatSocket->Create(0, SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, _T("192.168.11.1")) == FALSE)
		{
			AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
			return;
		}

		pChatSocket->m_strUserName = m_strName;
		pChatSocket->Connect(to_IP, to_Port);
		is_connect = true;
	}
	m_edit_text.ReplaceSel(m_strSend);
	HEADER head;
	head.type = MSG_SEND;
	head.nContentLen = len + 1;
	memset(head.from_user, 0, sizeof(head.from_user));
	_bstr_t b(m_strName);
	char *name = b;
	strcpy(head.from_user, name);
	memset(head.to_user, 0, sizeof(head.to_user));
	_bstr_t c(to_user);
	char *toUser = c;
	strcpy(head.to_user, toUser);

	pChatSocket->Send(&head, sizeof(head));
	pChatSocket->Send(data, len + 1);

 	m_strSend.Empty();
	UpdateData(FALSE);
}

BOOL CChatDlg::WChar2MByte(LPCWSTR lpSrc, LPSTR lpDest, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, 0, 0, FALSE);  //���ػ�������С
	if (nlen<n)
		return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, nlen, 0, FALSE);   //ת��
	return TRUE;
}

void CChatDlg::UpdateText(CString &strText)
{
	((CEdit*)GetDlgItem(IDC_ET_TEXT))->ReplaceSel(strText);
}

void CChatDlg::ConnnectToUser() {
	pChatSocket = new CSessionSocket();
	if (!pChatSocket)
	{
		AfxMessageBox(_T("�ڴ治�㣡"));
		return;
	}

	if (pChatSocket->Create(0, SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, _T("192.168.11.1")) == FALSE)
	{
		AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
		return;
	}
	//CSessionSocket *pSock = theApp.GetMainSocket();
	pChatSocket->Connect(to_IP, to_Port);
	is_connect = TRUE;
}

void CChatDlg::OnGetNewMsg(char *buf) {
	CString Msg(buf);
	m_edit_text.ReplaceSel(Msg);
}

CSessionSocket *CChatDlg::GetSocket() {
	return pChatSocket;
}

void CChatDlg::SetCaption(CString newCaption) {
	SetWindowText(newCaption);
}