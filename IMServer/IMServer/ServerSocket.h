#pragma once
#include "IMServerDlg.h"	//���Ի���ͷ�ļ�
class CIMServerDlg;
// CServerSocket command target

class CServerSocket : public CSocket
{
public:
	CIMServerDlg* g_pDlg;	//���Ի���ָ�����
public:
	CServerSocket();
	virtual ~CServerSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
};