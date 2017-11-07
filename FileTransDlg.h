#pragma once

#include "SessionSocket.h"
#include "afxcmn.h"

// FileTransDlg �Ի���

class FileTransDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FileTransDlg)

public:
	FileTransDlg(CSessionSocket* pSocket);
	FileTransDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FileTransDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILETRANS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 
	DECLARE_MESSAGE_MAP()

public:
	CSessionSocket* m_SessionSocket;
	afx_msg void On_Select_file();
	CString m_strFileName;
	CString m_strFilePath;
	CString to_userName;
	CProgressCtrl m_progress;
	CString to_IP;
	CString local_IP;
	int to_Port;
	int m_percent;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
