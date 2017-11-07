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

// ʵ��
protected:
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnSend();
	void OnGetNewMsg(char *);
	void SetCaption(CString newCaption);
	afx_msg void to_TransFile();
	void On_RcvFileMsg(HEADER head, char *buf);

public:
	CSessionSocket *pChatSocket;	//���������socket
	CEdit m_edit_text;	//�����
	CString m_strSend;	//���͵��ַ���
	CString m_strName;	//�����û���
	CString to_user;	//Ŀ���û���
	CString to_IP;		//Ŀ���û�IP
	CString local_IP;	//����IP
	int to_Port;		//Ŀ���û��˿�
	BOOL is_connect;	//���ڱ�ʶ�Ƿ��ѽ�������
	CString m_caption;//�Ի����������
	//FileTransDlg *fileDlg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
