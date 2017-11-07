#pragma once

// CSessionSocket ÃüÁîÄ¿±ê
#include "Header.h"

class CSessionSocket : public CSocket{
public:
	CSessionSocket();
	virtual ~CSessionSocket();
	virtual void OnReceive(int nErrorCode);
public:
	BOOL SendMSG(HEADER head, char* data);
	BOOL Is_Connect;
	SOCKADDR_IN addr;
	CString m_strUserName;
	virtual void OnClose(int nErrorCode);
};


