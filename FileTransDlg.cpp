// FileTransDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "FileTransDlg.h"
#include "afxdialogex.h"
#include "SessionSocket.h"
#include "FileSocket.h"
#include "cJSON.h"

// FileTransDlg 对话框

IMPLEMENT_DYNAMIC(FileTransDlg, CDialogEx)

FileTransDlg::FileTransDlg(CSessionSocket* pSocket)
	: CDialogEx(IDD_FILETRANS)
	, m_strFileName(_T(""))
	, m_percent(0)
{
	m_SessionSocket = pSocket;
}


FileTransDlg::FileTransDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILETRANS, pParent)
{

}

FileTransDlg::~FileTransDlg()
{
}

void FileTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILENAME, m_strFileName);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, IDC_TRANS_PERCENT, m_percent);
	DDV_MinMaxInt(pDX, m_percent, 0, 100);
}


BEGIN_MESSAGE_MAP(FileTransDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SELECT_FILE, &FileTransDlg::On_Select_file)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// FileTransDlg 消息处理程序


void FileTransDlg::On_Select_file()
{
	/*创建文件选择窗口*/
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("所有文件 (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal()) {
		m_strFileName = dlgFile.GetFileName();
		m_strFilePath = dlgFile.GetFolderPath() + _T("\\") + m_strFileName;
	}
	CFile FileIn;
	FileIn.Open(m_strFilePath, CFile::typeBinary);
	start_send = 0;

	/*向接收方发送消息，告知文件信息*/
	DWORD len = FileIn.GetLength();
	CString Clen;
	Clen.Format(_T("%d"), len);
	CString myPort;
	myPort.Format(_T("%d"), theApp.ListenPort + 1);
	CString Str = _T("{\"type\":\"sender\",\"filename\":\"") + m_strFileName + _T("\",\"filesize\":") + Clen + _T(",\"myPort\":") + myPort + _T(", \"myIP\":\"") + local_IP +_T("\"}");
	int lenTemp = WideCharToMultiByte(CP_ACP, 0, Str, -1, NULL, 0, NULL, NULL);
	char *data = new char[lenTemp + 1];
	WideCharToMultiByte(CP_ACP, 0, Str, -1, data, lenTemp, NULL, NULL);
	HEADER head;
	head.type = MSG_FILEINFO;
	head.nContentLen = lenTemp + 1;
	memset(head.to_user, 0, sizeof(head.to_user));
	memset(head.from_user, 0, sizeof(head.from_user));
	_bstr_t b(m_SessionSocket->m_strUserName);
	char *fromName = b;
	_bstr_t c(to_userName);
	char *toName = c;
	strcpy(head.from_user, fromName);
	strcpy(head.to_user, toName);
	m_SessionSocket->SendMSG(head, data);
	
	/*等待确认接收*/
	//while (start_send == 0) {
	//	Sleep(50);
	//}
	Sleep(10 * 1000);
	/*准备发送*/
	CFileSocket *pSocket = new CFileSocket();
	u_long* p;
	p = new u_long;
	*p = 0;
	ioctlsocket(pSocket->m_hSocket, FIONBIO, p);
	if (!pSocket->Create(theApp.ListenPort+1, SOCK_DGRAM))
	{
		AfxMessageBox(_T("Creat error"));
		return;
	}
	pSocket->AsyncSelect(FD_READ|FD_WRITE);
	m_progress.SetRange(0, 100);
	m_percent = 0;
	m_progress.SetPos(m_percent);
	m_progress.SetStep(1);
	UpdateData(FALSE);

	char buffer[1024*32];
	memset(buffer, 0, 1024*32);
	int tolNum = 0;
	SOCKADDR_IN addr_receive;
	addr_receive.sin_family = AF_INET;
	addr_receive.sin_port = to_Port+1;
	_bstr_t temp(to_IP);
	char* to_ip = temp;
	addr_receive.sin_addr.S_un.S_addr = inet_addr(to_ip);
	int addr_len_rece = sizeof(SOCKADDR_IN);
	char ack[64];
	memset(ack, 0, 64);
	int count = 0;
	int receive_count = 0;
	filePacket packet;
	while (tolNum < len) {
		FileIn.Seek(tolNum, CFile::begin);
		int num = FileIn.Read(buffer, sizeof(buffer));
		/*packet.head.SegIndex = count;
		packet.head.SegLength = num;
		packet.head.Is_Last = (tolNum + num == len);
		packet.head.checksum = '0';*/
		pSocket->SendTo(buffer, sizeof(buffer), to_Port + 1, to_IP, 0);
		tolNum += num;
		pSocket->ReceiveFrom(ack, 64, (SOCKADDR*)&addr_receive, &addr_len_rece, 0);
		receive_count = atoi(ack);
		if (receive_count == count + 1)
		{
			count++;
		}
		else
		{
			tolNum = receive_count * sizeof(buffer);
		}
		m_percent = (int)((double)tolNum / len * 100);
		m_progress.SetPos(m_percent);
		UpdateData(FALSE);
	}
	FileIn.Close();
	AfxMessageBox(_T("发送完成"));
	pSocket->Close();
}

void FileTransDlg::Rec_ReciverMsg(HEADER head, char *buf) {
	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buf);
	if (json_root == NULL) {
		return;
	}
	char *type = cJSON_GetObjectItem(json_root, _TYPE)->valuestring;
	CString Ctype(type);
	if (Ctype.Compare(_T("reciver")) == 0) {
		start_send = cJSON_GetObjectItem(json_root, "start_send")->valueint;
	}
}
