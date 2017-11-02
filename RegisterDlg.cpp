// RegisterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include "MD5.h"
#include "cJSON.h"
#include "Header.h"

// CRegisterDlg �Ի���

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REGISTERDLG, pParent)
	, userName(_T(""))
	, userPassword(_T(""))
	, passwordConfirm(_T(""))
	, userAnswer(_T(""))
{
	m_dwIP = ntohl(inet_addr("192.168.11.1"));
}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, userName);
	DDV_MaxChars(pDX, userName, 20);
	DDX_Text(pDX, IDC_EDIT2, userPassword);
	DDV_MaxChars(pDX, userPassword, 20);
	DDX_Text(pDX, IDC_EDIT3, passwordConfirm);
	DDV_MaxChars(pDX, passwordConfirm, 20);
	DDX_Text(pDX, IDC_EDIT4, userAnswer);
	DDV_MaxChars(pDX, userAnswer, 20);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(BTN_REGISTER, &CRegisterDlg::OnRegist)
END_MESSAGE_MAP()


// CRegisterDlg ��Ϣ�������


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	comboBoxInit();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CRegisterDlg::comboBoxInit() {
	m_ComboBox.AddString(_T("����ѧ�ţ��򹤺ţ��ǣ�"));
	m_ComboBox.AddString(_T("����ż�������ǣ�"));
	m_ComboBox.AddString(_T("�����а����ε������ǣ�"));
	m_ComboBox.AddString(_T("������Ϥ��ͯ����������ǣ�"));
}

void CRegisterDlg::OnRegist()
{
	UpdateData(TRUE);
	int nIndex = m_ComboBox.GetCurSel();
	CString question;
	m_ComboBox.GetLBText(nIndex, question);
	if (userName.IsEmpty() || userPassword.IsEmpty() || passwordConfirm.IsEmpty() || userAnswer.IsEmpty()) {
		AfxMessageBox(_T("�������д����������Ϣ��"));
		return;
	}
	else if (nIndex == -1) {
		AfxMessageBox(_T("��ѡ���ܱ����⣡"));
		return;
	}
	else if (userPassword.Compare(passwordConfirm) != 0) {
		AfxMessageBox(_T("�����������������Ƿ���ͬ��"));
		return;
	}

	_bstr_t b(userPassword);
	char* c = b;
	string Psw = c;
	string a = MD5(Psw).toStr();
	CString MD5Password(a.data());

	CString str = _T("{\"username\":\"") + userName + _T("\", \"password\":\"") + MD5Password + _T("\", \"question\":\"") + question + _T("\", \"answer\":\"") + userAnswer + _T("\"}");
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *data = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, data, len, NULL, NULL);

	HEADER head;
	head.type = MSG_REGIST;
	head.nContentLen = len + 1;
	memset(head.from_user, 0, sizeof(head.from_user));
	_bstr_t tempName(userName);
	char *name = tempName;
	strcpy(head.from_user, name);
	memset(head.to_user, 0, sizeof(head.to_user));

	CSessionSocket* pSock = theApp.GetMainSocket();
	if (pSock->Is_Connect == FALSE) {
		//��ʼֻ�Ǵ����ˣ���û�����ӣ���������socket�����5050�˿�Ҫ�ͷ���˼����Ķ˿�һֱ��������������ġ�
		CString ip = _T("192.168.11.1");
		pSock->Connect(ip, 5050);
		pSock->Is_Connect = TRUE;
	}
	pSock->Send(&head, sizeof(head));
	pSock->Send(data, len + 1);
}

void CRegisterDlg::getRegistMsg(char *buf) {

	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buf);
	int cmd = cJSON_GetObjectItem(json_root, "cmd")->valueint;

	if (cmd == 1) {
		CString str("ע��ɹ���");
		AfxMessageBox(str);
		CDialogEx::EndDialog(IDOK);
	}
	else {
		CString str("�û����ظ���");
		AfxMessageBox(str);
	}
}
