#pragma once


// CResetPswDlg �Ի���

class CResetPswDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResetPswDlg)

public:
	CResetPswDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResetPswDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESETPSWDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwIP;
	CString m_answer;
	CString m_pswComfirm;
	CString m_password;
	CString m_question;
	afx_msg void On_ResetPsw();
	virtual BOOL OnInitDialog();
	void RcvQuestion(char *buf);
	void On_GetReset(char *buff);
	CString m_username;
	afx_msg void On_GetQuestion();
};
