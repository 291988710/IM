#pragma once
#include "IMClientDlg.h"
class CIMClientDlg;

// CClientSocket command target

class CClientSocket : public CSocket
{
public:
	CIMClientDlg* g_pDlg;	//���Ի���ָ�����
	CClientSocket();
	virtual ~CClientSocket();
	virtual void OnReceive(int nErrorCode);
	BOOL SendMsg(LPSTR lpBuff,int nLen);
};


