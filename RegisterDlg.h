#pragma once
#include "afxwin.h"


// CRegisterDlg 对话框

class CRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegisterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void comboBoxInit();

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwIP;
	CString userName;
	CString userPassword;
	CString passwordConfirm;
	CString userAnswer;
	CComboBox m_ComboBox;
	afx_msg void OnRegist();
	void getRegistMsg(char *buf);
};
