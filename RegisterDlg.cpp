// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include "MD5.h"
#include "cJSON.h"
#include "Header.h"

// CRegisterDlg 对话框

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


// CRegisterDlg 消息处理程序


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	comboBoxInit();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CRegisterDlg::comboBoxInit() {
	m_ComboBox.AddString(_T("您的学号（或工号）是？"));
	m_ComboBox.AddString(_T("您配偶的生日是？"));
	m_ComboBox.AddString(_T("您初中班主任的名字是？"));
	m_ComboBox.AddString(_T("您最熟悉的童年好友名字是？"));
}

void CRegisterDlg::OnRegist()
{
	UpdateData(TRUE);
	int nIndex = m_ComboBox.GetCurSel();
	CString question;
	m_ComboBox.GetLBText(nIndex, question);
	if (userName.IsEmpty() || userPassword.IsEmpty() || passwordConfirm.IsEmpty() || userAnswer.IsEmpty()) {
		AfxMessageBox(_T("请完成填写上述所有信息！"));
		return;
	}
	else if (nIndex == -1) {
		AfxMessageBox(_T("请选择密保问题！"));
		return;
	}
	else if (userPassword.Compare(passwordConfirm) != 0) {
		AfxMessageBox(_T("请检查两次密码输入是否相同！"));
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
		//开始只是创建了，并没有连接，这里连接socket，这个5050端口要和服务端监听的端口一直，否则监听不到的。
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
		CString str("注册成功！");
		AfxMessageBox(str);
		CDialogEx::EndDialog(IDOK);
	}
	else {
		CString str("用户名重复！");
		AfxMessageBox(str);
	}
}
