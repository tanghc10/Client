// ChatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "Header.h"
#include "ChatDlg.h"
#include "cJSON.h"
#include "FileSocket.h"
#include "afxdialogex.h"
#include "FileTransDlg.h"

// LogInDlg �Ի���

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
	, m_strSend(_T(""))
	, m_strName(_T(""))
{
	is_connect = FALSE;
	to_IP = _T("");
}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ET_SEND, m_strSend);
	DDX_Control(pDX, IDC_ET_TEXT, m_edit_text);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CChatDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_FILETRANS, &CChatDlg::to_TransFile)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ChatDlg ��Ϣ�������
BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(m_caption);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChatDlg::OnBnClickedBtnSend()
{
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
	{
		AfxMessageBox(_T("�������ݲ���Ϊ�գ�"));
		return;
	}
	CTime time;
	time = CTime::GetCurrentTime();  //��ȡ����ʱ��
	CString strTime = time.Format("%Y-%m-%d %H:%M:%S ");
	m_strSend = strTime + m_strName + _T("��\r\n") + m_strSend + _T("\r\n");
	int len = WideCharToMultiByte(CP_ACP, 0, m_strSend, -1, NULL, 0, NULL, NULL);
	char *data = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_strSend, -1, data, len, NULL, NULL);
	
	/*���û�н��������ӵĻ� �Ƚ�������*/
	if (is_connect == FALSE) {
		pChatSocket = new CSessionSocket();
		if (!pChatSocket)
		{
			AfxMessageBox(_T("�ڴ治�㣡"));
			return;
		}
		if (pChatSocket->Create(0, SOCK_STREAM) == FALSE)
		{
			AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
			return;
		}
		pChatSocket->m_strUserName = m_strName;
		pChatSocket->Connect(to_IP, to_Port);
		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_port = to_Port;
		_bstr_t Temp(to_IP);
		char* toIPaddr = Temp;
		addr.sin_addr.S_un.S_addr = inet_addr(toIPaddr);
		pChatSocket->addr = addr;
		is_connect = true;
	}
	/*����ͼ��ʾ��Ϣ�Ŀ�������µ���Ϣ*/
	m_edit_text.ReplaceSel(m_strSend);
	HEADER head;
	head.type = MSG_SEND;
	head.nContentLen = len + 1;
	memset(head.from_user, 0, sizeof(head.from_user));
	_bstr_t b(m_strName);
	char *name = b;
	strcpy(head.from_user, name);
	memset(head.to_user, 0, sizeof(head.to_user));
	_bstr_t c(to_user);
	char *toUser = c;
	strcpy(head.to_user, toUser);

	pChatSocket->SendMSG(head, data);

 	m_strSend.Empty();
	UpdateData(FALSE);
}

void CChatDlg::OnGetNewMsg(char *buf) {
	to_IP = inet_ntoa(pChatSocket->addr.sin_addr);
	CString Msg(buf);
	m_edit_text.ReplaceSel(Msg);
}

void CChatDlg::SetCaption(CString newCaption) {
	SetWindowText(newCaption);
}

void CChatDlg::to_TransFile()
{
	FileTransDlg *fileDlg;
	fileDlg = new FileTransDlg(pChatSocket);
	fileDlg->to_userName = to_user;
	fileDlg->to_IP = to_IP;
	fileDlg->to_Port = to_Port;
	fileDlg->local_IP = local_IP;
	if (fileDlg->DoModal()) {
		delete fileDlg;
		fileDlg = NULL;
	}
}

void CChatDlg::On_RcvFileMsg(HEADER head, char *buf) {
	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buf);
	if (json_root == NULL) {
		return;
	}
	char *type = cJSON_GetObjectItem(json_root, _TYPE)->valuestring;
	CString Ctype(type);
	if (Ctype.Compare(_T("sender")) == 0) {
		/*�����ļ���Ϣ*/
		int tolNum = cJSON_GetObjectItem(json_root, "filesize")->valueint;
		char* fileName = cJSON_GetObjectItem(json_root, "filename")->valuestring;
		int toPort = cJSON_GetObjectItem(json_root, "myPort")->valueint;
		char* toIP = cJSON_GetObjectItem(json_root, "myIP")->valuestring;
		CString str(fileName);
		CString CtoIP(toIP);
		CString filenum;
		filenum.Format(_T(" ��СΪ %d �ֽ�"), tolNum);
		CString strTmp = _T("�Է����㷢����һ���ļ� ") + str + filenum;
		AfxMessageBox(strTmp);
		/*ѡ���ļ�����λ��*/
		/*
		wchar_t temp[256];
		CString Dir = temp;
		CFolderPickerDialog dlg(Dir, 0, NULL, 0);
		if (dlg.DoModal()) {
			str = dlg.GetPathName() + _T("\\") + str;
			AfxMessageBox(str);
		}*/

		/*��ʼ׼�������ļ�*/
		CFileSocket* pSocket = new CFileSocket();
		u_long* p;
		p = new u_long;
		*p = 0;
		ioctlsocket(pSocket->m_hSocket, FIONBIO, p);
		CString Str = _T("{\"type\":\"reciver\",\"start_send\":\"1\"}");
		if(!pSocket->Create(theApp.ListenPort+1, SOCK_DGRAM))
		{
			AfxMessageBox(_T("Create error"));
			Str = _T("{\"type\":\"reciver\",\"start_send\":\"0\"}");
		}
		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_port = toPort;
		addr.sin_addr.S_un.S_addr = inet_addr(toIP);
		int addr_len = sizeof(SOCKADDR_IN);

		/*֪ͨ���ͷ���ʼ����*/
		/*
		int lenTemp = WideCharToMultiByte(CP_ACP, 0, Str, -1, NULL, 0, NULL, NULL);
		char *data = new char[lenTemp + 1];
		WideCharToMultiByte(CP_ACP, 0, Str, -1, data, lenTemp, NULL, NULL);
		HEADER head;
		head.type = MSG_FILEINFO;
		head.nContentLen = lenTemp + 1;
		memset(head.to_user, 0, sizeof(head.to_user));
		memset(head.from_user, 0, sizeof(head.from_user));
		pChatSocket->SendMSG(head, data);*/

		/*׼��д���ļ�����*/
		CFile FileOut;
		FileOut.Open(str, CFile::modeCreate | CFile::modeWrite);

		/*packet��������, buffer_head�������������*/
		filePacket packet;
		memset(packet.buffer, 0, sizeof(packet.buffer));
		packet.head.SegIndex = 0;
		fileBuffer *buffer_head = NULL;
		fileBuffer *buffer_tail = NULL;
		fileBuffer *buffer_pre = NULL;

		int acceptNum = 0;
		char ack[64];
		memset(ack, 0, 64);
		int rcv_base = 0;
		SetTimer(1, 200, NULL);
		while (acceptNum < tolNum) {
			int num = pSocket->ReceiveFrom((char *)&packet, sizeof(packet), (SOCKADDR*)&addr, &addr_len, 0);
			if (rcv_base <= packet.head.SegIndex) {
				if (rcv_base == packet.head.SegIndex) {	//��������Ҫ�İ���ֱ��д���ļ�
					rcv_base++;
					FileOut.Write(packet.buffer, num);
					acceptNum += num;
					if (buffer_head != NULL) {	//��黺��������Ƿ�����Ҫ�İ�
						fileBuffer* temp = buffer_head;
						while (temp != NULL && rcv_base == temp->buffer.head.SegIndex)
						{
							FileOut.Write(temp->buffer.buffer, temp->num);
							acceptNum += temp->num;
							temp = temp->next;
							free(buffer_head);
							buffer_head = temp;
							rcv_base++;
						}
					}
				}
				else {
					if (buffer_head == NULL) {
						fileBuffer *temp = (fileBuffer*)malloc(sizeof(fileBuffer));
						memset(temp->buffer.buffer, 0, sizeof(temp->buffer.buffer));
						strcpy(temp->buffer.buffer, packet.buffer);
						temp->num = num;
						temp->next = NULL;
						temp->buffer.head.SegIndex = packet.head.SegIndex;
						buffer_head = temp;
					}
					else{
						fileBuffer *temp = (fileBuffer*)malloc(sizeof(fileBuffer));
						memset(temp->buffer.buffer, 0, sizeof(temp->buffer.buffer));
						strcpy(temp->buffer.buffer, packet.buffer);
						temp->num = num;
						temp->next = NULL;
						temp->buffer.head.SegIndex = packet.head.SegIndex;

						if (temp->buffer.head.SegIndex < buffer_head->buffer.head.SegIndex) {
							temp->next = buffer_head;
							buffer_head = temp;
						}
						else {
							buffer_pre = buffer_head;
							buffer_tail = buffer_pre->next;
							while (buffer_pre->buffer.head.SegIndex < temp->buffer.head.SegIndex && buffer_tail != NULL) {
								buffer_pre = buffer_tail;
								buffer_tail = buffer_tail->next;
							}
							temp->next = buffer_tail;
							buffer_pre->next = temp;
						}
					}
				}
				sprintf(ack, "%d", rcv_base);
				pSocket->SendTo(ack, sizeof(ack), toPort, CtoIP, 0);
			}
		}
		FileOut.Flush();
		FileOut.Close();
		AfxMessageBox(_T("�������"));
		pSocket->Close();
	}
	cJSON_Delete(json_root);
}

void CChatDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(nIDEvent);
	AfxMessageBox(_T("��ʱ��"));
	CDialogEx::OnTimer(nIDEvent);
}
