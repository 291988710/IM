// IMClientDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>
#include "afxwin.h"
#include "ClientSocket.h"
#include "MsgProtocol.h"
#include "ClientDlg.h"

class CClientSocket;
class CClientDlg;


// CIMClientDlg �Ի���
class CIMClientDlg : public CDialog
{
// ����
public:
	CIMClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

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

// �Ի�������
	enum { IDD = IDD_LOGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
