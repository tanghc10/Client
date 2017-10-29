// LogInDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "LogInDlg.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include "SessionSocket.h"



// LogInDlg �Ի���

IMPLEMENT_DYNAMIC(CLogInDlg, CDialogEx)

CLogInDlg::CLogInDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOGIN, pParent)
	, m_strUser(_T(""))
{
	//��ʼ��IP��ַ
	//DWORD ����unSigned long
	m_dwIP = ntohl(inet_addr("192.168.11.1"));
}

CLogInDlg::~CLogInDlg()
{
}

void CLogInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ET_NAME, m_strUser);
}

BEGIN_MESSAGE_MAP(CLogInDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGOIN, &CLogInDlg::OnBnClickedBtnLogoin)
	ON_BN_CLICKED(IDC_BTN_REGIST, &CLogInDlg::to_Regist)
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

	if (m_dwIP == 0)
	{
		AfxMessageBox(_T("��ЧIP��ַ"));
		return;
	}

	CSessionSocket* pSock = theApp.GetMainSocket();
	IN_ADDR addr;
	addr.S_un.S_addr = htonl(m_dwIP);
	//inet_ntoa����һ��char *,�����char *�Ŀռ�����inet_ntoa���澲̬����
	CString strIP(inet_ntoa(addr));
	//��ʼֻ�Ǵ����ˣ���û�����ӣ���������socket�����5050�˿�Ҫ�ͷ���˼����Ķ˿�һֱ��������������ġ�
	CString ip = _T("192.168.11.1");
	pSock->Connect(ip, 5050);

	CString Cm_strUser = m_strUser;
	char from_user[20];
	memset(from_user, 0, sizeof(from_user));
	WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)m_strUser, -1, from_user, 260, 0, false);
	//����
	pSock->m_strUserName = m_strUser;  //���û����ִ��ݹ�ȥ
	char* pBuff = new char[m_strUser.GetLength() + 1];
	memset(pBuff, 0, m_strUser.GetLength());  //����һ�����洢�û������ڴ�ռ�
	if (WChar2MByte(m_strUser.GetBuffer(0), pBuff, m_strUser.GetLength() + 1))
		pSock->LogoIn(pBuff, m_strUser.GetLength() + 1, from_user);  //ͷ���ռ䣬��ͷ������
	delete pBuff;
	CDialogEx::OnOK();
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
	/*registDlg = new CRegisterDlg();
	registDlg->Create(IDD_REGISTERDLG);
	registDlg->ShowWindow(SW_SHOW);*/
	registDlg = new CRegisterDlg();
	INT_PTR rsp = registDlg->DoModal();
	if (rsp == IDCANCEL) {
		delete registDlg;
	}
	else {
		delete registDlg;
	}
}