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

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();		//重载OnCancel函数，用来销毁父窗口

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
