#pragma once


// CResetPswDlg 对话框

class CResetPswDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResetPswDlg)

public:
	CResetPswDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResetPswDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESETPSWDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
