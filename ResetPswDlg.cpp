// ResetPswDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "Header.h"
#include "cJSON.h"
#include "ResetPswDlg.h"
#include "afxdialogex.h"


// CResetPswDlg �Ի���

IMPLEMENT_DYNAMIC(CResetPswDlg, CDialogEx)

CResetPswDlg::CResetPswDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RESETPSWDLG, pParent)
	, m_answer(_T(""))
	, m_pswComfirm(_T(""))
	, m_password(_T(""))
	, m_question(_T(""))
	, m_username(_T(""))
{

}

CResetPswDlg::~CResetPswDlg()
{
	m_dwIP = ntohl(inet_addr("192.168.11.1"));
}

void CResetPswDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, RESET_ANS, m_answer);
	DDV_MaxChars(pDX, m_answer, 20);
	DDX_Text(pDX, RESET_CFM, m_pswComfirm);
	DDV_MaxChars(pDX, m_pswComfirm, 20);
	DDX_Text(pDX, RESET_PSW, m_password);
	DDV_MaxChars(pDX, m_password, 20);
	DDX_Text(pDX, RESET_QUE, m_question);
	DDV_MaxChars(pDX, m_question, 20);
	DDX_Text(pDX, RESET_NAME, m_username);
	DDV_MaxChars(pDX, m_username, 20);
}


BEGIN_MESSAGE_MAP(CResetPswDlg, CDialogEx)
	ON_BN_CLICKED(BTN_RESET, &CResetPswDlg::On_ResetPsw)
	ON_BN_CLICKED(BTN_GETQUE, &CResetPswDlg::On_GetQuestion)
END_MESSAGE_MAP()


// CResetPswDlg ��Ϣ�������


void CResetPswDlg::On_ResetPsw()
{
	UpdateData(TRUE);
	if (m_username.IsEmpty() || m_answer.IsEmpty() || m_password.IsEmpty()) {
		AfxMessageBox(_T("�������д����������Ϣ��"));
	}else if (m_password.Compare(m_pswComfirm) != 0){
		AfxMessageBox(_T("�����������������Ƿ���ͬ��"));
	}
	CString str = _T("{\"username\":\"") + m_username + _T("\", \"password\":\"") + m_password + _T("\", \"answer\":\"") + m_answer + _T("\"}");
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *data = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, data, len, NULL, NULL);

	_bstr_t b(m_username);
	char *name = b;

	HEADER head;
	head.type = MSG_RESET;
	head.nContentLen = len + 1;
	memset(head.from_user, 0, sizeof(head.from_user));
	strcpy(head.from_user, name);
	memset(head.to_user, 0, sizeof(head.to_user));
	strcpy(head.to_user, "Server");

	CSessionSocket* pSock = theApp.GetMainSocket();
	IN_ADDR addr;
	addr.S_un.S_addr = htonl(m_dwIP);
	//inet_ntoa����һ��char *,�����char *�Ŀռ�����inet_ntoa���澲̬����
	CString strIP(inet_ntoa(addr));
	//��ʼֻ�Ǵ����ˣ���û�����ӣ���������socket�����5050�˿�Ҫ�ͷ���˼����Ķ˿�һֱ��������������ġ�
	CString ip = _T("192.168.11.1");
	pSock->Connect(ip, 5050);
	pSock->Send(&head, sizeof(head));
	pSock->Send(data, len + 1);
}

BOOL CResetPswDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CResetPswDlg::On_GetQuestion()
{
	UpdateData(TRUE);
	if (!m_username) {
		AfxMessageBox(_T("�������û���"));
		return;
	}
	CString str = _T("{\"username\":\"") + m_username + _T("\"}");
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *data = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, data, len, NULL, NULL);

	_bstr_t b(m_username);
	char *name = b;

	HEADER head;
	head.type = MSG_GETQUE;
	head.nContentLen = len + 1;
	memset(head.from_user, 0, sizeof(head.from_user));
	strcpy(head.from_user, name);
	memset(head.to_user, 0, sizeof(head.to_user));
	strcpy(head.to_user, "Server");

	CSessionSocket* pSock = theApp.GetMainSocket();
	IN_ADDR addr;
	addr.S_un.S_addr = htonl(m_dwIP);
	//inet_ntoa����һ��char *,�����char *�Ŀռ�����inet_ntoa���澲̬����
	CString strIP(inet_ntoa(addr));
	//��ʼֻ�Ǵ����ˣ���û�����ӣ���������socket�����5050�˿�Ҫ�ͷ���˼����Ķ˿�һֱ��������������ġ�
	CString ip = _T("192.168.11.1");
	pSock->Connect(ip, 5050);
	pSock->Send(&head, sizeof(head));
	pSock->Send(data, len + 1);
}

void CResetPswDlg::RcvQuestion(char *buff) {
	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buff);
	int cmd = cJSON_GetObjectItem(json_root, "cmd")->valueint;
	if (cmd == 0) {
		AfxMessageBox(_T("��Ч���û���"));
		return;
	}
	char *name = cJSON_GetObjectItem(json_root, "question")->valuestring;
	CString str(name);
	SetDlgItemText(RESET_QUE, str);
}

void CResetPswDlg::On_GetReset(char *buff) {
	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buff);
	int cmd = cJSON_GetObjectItem(json_root, "cmd")->valueint;
	if (cmd == 1) {
		AfxMessageBox(_T("�޸ĳɹ�"));
		CDialogEx::OnCancel();
	}
	else {
		AfxMessageBox(_T("�û��������ڻ��ܱ��������"));
		return;
	}
}
