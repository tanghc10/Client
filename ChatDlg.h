#pragma once

#include "afxwin.h"

// ChatDlg �Ի���

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatDlg();

// �Ի�������
	enum { IDD = IDD_CHATDLG };
public:
	void UpdateText(CString& strText);  //���������¼

// ʵ��
protected:
	BOOL WChar2MByte(LPCTSTR lpSrc, LPSTR lpDest, int nlen);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnSend();
	void ConnnectToUser();
	void OnGetNewMsg(char *);
	CSessionSocket *GetSocket();
	void SetCaption(CString newCaption);
	CString m_strSend;
	CString m_strName;
	CString to_user;
	CString to_IP;
	int to_Port;
	BOOL is_connect;
	CString m_caption;//�Ի����������

public:
	CSessionSocket *pChatSocket;
	CEdit m_edit_text;
};
