// LogInDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "MD5.h"
#include "LogInDlg.h"
#include "IPInfo.h"
#include "Header.h"
#include "cJSON.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include "SessionSocket.h"



// LogInDlg �Ի���

IMPLEMENT_DYNAMIC(CLogInDlg, CDialogEx)

CLogInDlg::CLogInDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOGIN, pParent)
	, m_strUser(_T(""))
	, m_strPsw(_T(""))
{
}

CLogInDlg::~CLogInDlg()
{
}

void CLogInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ET_NAME, m_strUser);
	DDX_Text(pDX, IDC_ET_PSW, m_strPsw);
}

BEGIN_MESSAGE_MAP(CLogInDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGOIN, &CLogInDlg::OnBnClickedBtnLogoin)
	ON_BN_CLICKED(IDC_BTN_REGIST, &CLogInDlg::to_Regist)
	ON_BN_CLICKED(IDC_BUTTON2, &CLogInDlg::to_ResetPsw)
END_MESSAGE_MAP()


void CLogInDlg::OnBnClickedBtnLogoin()
{
	//��¼	
	UpdateData();
	if (m_strUser.IsEmpty())
	{
		AfxMessageBox(_T("�û�������Ϊ�գ�"));
		return;
	}
	/*���������������*/
	CSessionSocket* pSock = theApp.GetMainSocket();
	if (pSock->Is_Connect == FALSE) {
		CString ip(Server_IP);
		pSock->Connect(ip, Server_Port);
	}

	/*������ܣ�MD5*/
	_bstr_t b(m_strPsw);
	char* c = b;
	string Psw = c;
	string a = MD5(Psw).toStr();
	CString MD5Password(a.data());

	/*��json��ʽ���������������*/
	cJSON *json_root = NULL;
	CString str = _T("{\"username\":\"") + m_strUser + _T("\", \"password\":\"") + MD5Password + _T("\"}");
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *data = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, data, len, NULL, NULL);
	_bstr_t Name(m_strUser);
	char *name = Name;
	pSock->m_strUserName = m_strUser;  //���û����ִ��ݹ�ȥ
	HEADER head;
	head.type = MSG_LOGIN;
	head.nContentLen = len + 1;
	memset(head.to_user, 0, sizeof(head.to_user));
	strcpy(head.to_user, "Server");
	memset(head.from_user, 0, sizeof(head.from_user));
	strcpy(head.from_user, name);
	pSock->SendMSG(head, data);
}

void CLogInDlg::RvcFromServer(char *buf) {
	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buf);
	if (json_root == NULL) {
		AfxMessageBox(_T("�������ݸ�ʽ����"));
		return;
	}
	int cmd = cJSON_GetObjectItem(json_root, _CMD)->valueint;	//�Ƿ��¼�ɹ�
	if (cmd == 1) {
		int port = cJSON_GetObjectItem(json_root, _PORT)->valueint;
		theApp.ListenToPort(port);
		theApp.ListenPort = port;
		CDialogEx::OnOK();
	}
	else {
		CString str("�û������������");
		AfxMessageBox(str);
		return;
	}
}

void CLogInDlg::to_Regist()
{
	registDlg = new CRegisterDlg();
	INT_PTR rsp = registDlg->DoModal();
	if (rsp == IDCANCEL) {
		delete registDlg;
	}
	else {
		delete registDlg;
	}
}

void CLogInDlg::to_ResetPsw()
{
	resetDlg = new CResetPswDlg();
	INT_PTR rsp = resetDlg->DoModal();
	if (rsp == IDCANCEL) {
		delete resetDlg;
	}
	else {
		delete resetDlg;
	}
}
