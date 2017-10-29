#pragma once

// CSessionSocket ÃüÁîÄ¿±ê

class CSessionSocket : public CAsyncSocket
{
public:
	CSessionSocket();
	virtual ~CSessionSocket();
	virtual void OnReceive(int nErrorCode);
public:
	BOOL LogoIn(LPSTR lpBuff, int nlen, char from_user[20]);
	BOOL SendMSG(LPSTR lpBuff, int nlen, char to_user[20], char from_user[20]);
	CString m_strUserName;
};


