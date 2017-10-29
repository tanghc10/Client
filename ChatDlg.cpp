// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ChatDlg.h"
#include "afxdialogex.h"

// LogInDlg 对话框

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


// ChatDlg 消息处理程序
BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(m_caption);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatDlg::OnBnClickedBtnSend()
{
	//发送消息
	UpdateData();
	if (m_strSend.IsEmpty())
	{
		AfxMessageBox(_T("发送类容不能为空！"));
		return;
	}
	CString m_strUserName = theApp.GetMainSocket()->m_strUserName;
	char from_user[20];
	memset(from_user, 0, sizeof(from_user));

	WChar2MByte(m_strUserName.GetBuffer(0), from_user, m_strUserName.GetLength() * 2);


	CString temp;
	CTime time = CTime::GetCurrentTime();
	temp = time.Format("%H:%M:%S");
	//姓名 +_T("\n\t") 时间

	m_strSend = m_strUserName + _T(" 发送给  ") + touser + _T("  ") + temp + _T("\r\n   ") + m_strSend + _T("\r\n");
	char* pBuff = new char[m_strSend.GetLength() * 2];
	memset(pBuff, 0, m_strSend.GetLength() * 2);

	//转换为多字节
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
	n = WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, 0, 0, FALSE);  //返回缓冲区大小
	if (nlen<n)
		return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, nlen, 0, FALSE);   //转换
	return TRUE;
}

void CChatDlg::UpdateText(CString &strText)
{
	((CEdit*)GetDlgItem(IDC_ET_TEXT))->ReplaceSel(strText);
}
