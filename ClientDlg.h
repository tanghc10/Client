
// ClientDlg.h : 头文件
//

#pragma once

#include "MyListBox.h"
#include "Header.h"

// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif
	void UpdateUserInfo(CString strInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString m_caption;//对话框标题设置
	afx_msg void On_choose();
	void GetNewMsg(HEADER head, char * buf);
	void OnGetIp(char *buf);
	void OnRcvOfflineMsg(HEADER head, char *buf);
public:
	CPtrList *m_chatList;
	CMyListBox m_listBox;
};
