
// ClientDlg.h : ͷ�ļ�
//

#pragma once

#include "MyListBox.h"
#include "Header.h"

// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif
	void UpdateUserInfo(CString strInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString m_caption;//�Ի����������
	afx_msg void On_choose();
	void GetNewMsg(HEADER head, char * buf);
	void OnGetIp(char *buf);
	void OnRcvOfflineMsg(HEADER head, char *buf);
public:
	CPtrList *m_chatList;
	CMyListBox m_listBox;
};
