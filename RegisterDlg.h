#pragma once
#include "afxwin.h"


// CRegisterDlg �Ի���

class CRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegisterDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	void comboBoxInit();
	afx_msg void OnRegist();
	DECLARE_MESSAGE_MAP()
public:
	void getRegistMsg(char *buf);
public:
	CString userName;
	CString userPassword;
	CString passwordConfirm;
	CString userAnswer;
	CComboBox m_ComboBox;
};
