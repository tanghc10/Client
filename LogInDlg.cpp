// LogInDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "MD5.h"
#include "LogInDlg.h"
#include "Header.h"
#include "cJSON.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include "SessionSocket.h"



// LogInDlg 对话框

IMPLEMENT_DYNAMIC(CLogInDlg, CDialogEx)

CLogInDlg::CLogInDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOGIN, pParent)
	, m_strUser(_T(""))
	, m_strPsw(_T(""))
{
	//初始化IP地址
	//DWORD 就是unSigned long
	m_dwIP = ntohl(inet_addr("192.168.11.1"));
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
	//登录	
	UpdateData();
	if (m_strUser.IsEmpty())
	{
		AfxMessageBox(_T("用户名不能为空！"));
		return;
	}

	CSessionSocket* pSock = theApp.GetMainSocket();
	if (pSock->Is_Connect == FALSE) {
		IN_ADDR addr;
		addr.S_un.S_addr = htonl(m_dwIP);
		//inet_ntoa返回一个char *,而这个char *的空间是在inet_ntoa里面静态分配
		CString strIP(inet_ntoa(addr));
		//开始只是创建了，并没有连接，这里连接socket，这个5050端口要和服务端监听的端口一直，否则监听不到的。
		CString ip = _T("192.168.11.1");
		pSock->Connect(ip, 5050);
		pSock->Is_Connect = TRUE;
	}

	_bstr_t b(m_strPsw);
	char* c = b;
	string Psw = c;
	string a = MD5(Psw).toStr();
	CString MD5Password(a.data());

	cJSON *json_root = NULL;
	CString str = _T("{\"username\":\"") + m_strUser + _T("\", \"password\":\"") + MD5Password + _T("\"}");
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *data = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, data, len, NULL, NULL);

	_bstr_t Name(m_strUser);
	char *name = Name;
	//发送
	pSock->m_strUserName = m_strUser;  //将用户名字传递过去
	pSock->LogoIn(data, strlen(data), name);
}

void CLogInDlg::RvcFromServer(char *buf) {

	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buf);
	int cmd = cJSON_GetObjectItem(json_root, "cmd")->valueint;

	if (cmd == 1) {
		CDialogEx::OnOK();
	}
	else {
		CString str("用户名或密码错误！");
		AfxMessageBox(str);
		return;
	}
}

BOOL CLogInDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(0);
	((CEdit*)GetDlgItem(IDC_ET_PSW))->SetWindowTextW(_T(""));
	return TRUE;
}

BOOL CLogInDlg::WChar2MByte(LPCWSTR lpSrc, LPSTR lpDest, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, 0, 0, FALSE);
	if (n<nlen)
		return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, nlen, 0, FALSE);
	return TRUE;
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
