// ChatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ChatDlg.h"
#include "afxdialogex.h"

// LogInDlg �Ի���

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
	, m_strSend(_T(""))
{

}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ET_SEND, m_strSend);
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
	UpdateData();
	if (m_strSend.IsEmpty())
	{
		AfxMessageBox(_T("�������ݲ���Ϊ�գ�"));
		return;
	}
	CString m_strUserName = theApp.GetMainSocket()->m_strUserName;
	char from_user[20];
	memset(from_user, 0, sizeof(from_user));

	WChar2MByte(m_strUserName.GetBuffer(0), from_user, m_strUserName.GetLength() * 2);


	CString temp;
	CTime time = CTime::GetCurrentTime();
	temp = time.Format("%H:%M:%S");
	//���� +_T("\n\t") ʱ��

	m_strSend = m_strUserName + _T(" ���͸�  ") + touser + _T("  ") + temp + _T("\r\n   ") + m_strSend + _T("\r\n");
	char* pBuff = new char[m_strSend.GetLength() * 2];
	memset(pBuff, 0, m_strSend.GetLength() * 2);

	//ת��Ϊ���ֽ�
	WChar2MByte(m_strSend.GetBuffer(0), pBuff, m_strSend.GetLength() * 2);
	theApp.GetMainSocket()->SendMSG(pBuff, m_strSend.GetLength() * 2, touser, from_user);

	delete pBuff;

	m_strSend.Empty();
	UpdateData(0);
	CDialogEx::OnOK();
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
