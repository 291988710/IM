// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "IMClient.h"
#include "ClientSocket.h"
#include "IMClientDlg.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket member functions
void CClientSocket::OnReceive(int nErrorCode)
{
	char* pData = NULL;
	pData = new char[1024];
	memset(pData,0,sizeof(char)*1024);
	UCHAR leng = 0;
	CString str;
	leng = Receive(pData,1024,0);
	str = pData;
	g_pDlg->ReceiveMsgFromServer(str);
	//((CIMClientDlg*)theApp.GetMainWnd())->ReceiveMsgFromServer(str);
	delete pData;
	pData = NULL;
	CSocket::OnReceive(nErrorCode);
}

BOOL CClientSocket::SendMsg(LPSTR lpBuff,int nLen)
{
	//����Э��ͷ
	if(Send(lpBuff,nLen) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("���ʹ���"));
		return FALSE;
	}
	return TRUE;
}