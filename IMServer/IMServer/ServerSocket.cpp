// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "IMServer.h"
#include "ServerSocket.h"


// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}


// CServerSocket member functions

void CServerSocket::OnReceive(int nErrorCode)
{
	g_pDlg->RecvData(this);		//接收数据
	CSocket::OnReceive(nErrorCode);
}

void CServerSocket::OnClose(int nErrorCode)
{
	g_pDlg->RemoveClient(this);		//删除下线用户
	CSocket::OnClose(nErrorCode);
}

void CServerSocket::OnAccept(int nErrorCode)
{
	g_pDlg->AddClient();	//添加上线用户
	CSocket::OnAccept(nErrorCode);
}