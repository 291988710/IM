#pragma once
#include "IMServerDlg.h"	//主对话框头文件
class CIMServerDlg;
// CServerSocket command target

class CServerSocket : public CSocket
{
public:
	CIMServerDlg* g_pDlg;	//主对话框指针对象
public:
	CServerSocket();
	virtual ~CServerSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
};