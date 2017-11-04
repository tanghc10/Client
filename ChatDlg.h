#pragma once

#include "afxwin.h"

// ChatDlg 对话框

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatDlg();

// 对话框数据
	enum { IDD = IDD_CHATDLG };

// 实现
protected:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnSend();
	void OnGetNewMsg(char *);
	void SetCaption(CString newCaption);
	

public:
	CSessionSocket *pChatSocket;	//用于聊天的socket
	CEdit m_edit_text;	//输入框
	CString m_strSend;	//发送的字符串
	CString m_strName;	//本地用户名
	CString to_user;	//目标用户名
	CString to_IP;		//目标用户IP
	int to_Port;		//目标用户端口
	BOOL is_connect;	//用于标识是否已建立连接
	CString m_caption;//对话框标题设置
};
