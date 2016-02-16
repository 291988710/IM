// IMClientDlg.h : 头文件
//

#pragma once
#include <vector>
#include "afxwin.h"
#include "ClientSocket.h"
#include "MsgProtocol.h"
#include "ClientDlg.h"

class CClientSocket;
class CClientDlg;


// CIMClientDlg 对话框
class CIMClientDlg : public CDialog
{
// 构造
public:
	CIMClientDlg(CWnd* pParent = NULL);	// 标准构造函数

public:
	bool g_bConnect;
	CClientSocket* pSocket;
	std::vector<CString> g_vecSeperatedData;
	CClientDlg* pClientDlg;
public:
	BOOL WChar2MByte(LPCWSTR lpSrc,LPSTR lpDest,int nLen);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SendMsgToServer(CString str);
	void AddControlText(CString str);
	void ReceiveMsgFromServer(CString str);
	void SeperateMsgFromServer(CString str);
	void Func_OnValidateAccountAndPassword(CString strResult);

// 对话框数据
	enum { IDD = IDD_LOGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString g_strControl;
	CString g_strAccount;
	CString g_strPassword;
	afx_msg void OnStnClickedLogin();
	afx_msg void OnBnClickedOk();
	CEdit g_cEditControl;
};
