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
	g_pDlg->RecvData(this);		//��������
	CSocket::OnReceive(nErrorCode);
}

void CServerSocket::OnClose(int nErrorCode)
{
	g_pDlg->RemoveClient(this);		//ɾ�������û�
	CSocket::OnClose(nErrorCode);
}

void CServerSocket::OnAccept(int nErrorCode)
{
	g_pDlg->AddClient();	//��������û�
	CSocket::OnAccept(nErrorCode);
}