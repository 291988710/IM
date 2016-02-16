#pragma once
#include "afxcmn.h"
#include "ClientSocket.h"

class CClientSocket;

// CClientDlg dialog

class CClientDlg : public CDialog
{
	DECLARE_DYNAMIC(CClientDlg)

public:
	CClientDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClientDlg();

// Dialog Data
	enum { IDD = IDD_CLIENT_DIALOG };

public:
	CClientSocket *g_pSocket;
	bool g_bConnect;
public:
	void SendMsgToServer(CString str);
	void ReceiveMsgFromServer(CString str);
	BOOL WChar2MByte(LPCWSTR lpSrc,LPSTR lpDest,int nLen);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();		//����OnCancel�������������ٸ�����

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString g_strAccount;
	CString g_strPassword;
	CString g_strIp;
	CString g_strPort;
	CListCtrl g_listCtlFriend;
};
