// IMServerDlg.h : header file
//
#ifndef CIMSERVERDLG_H
#define CIMSERVERDLG_H

#pragma once
#include "afxwin.h"
#include "ServerSocket.h"
#include "DataFunction.h"
#include "Resource.h"

class CServerSocket;
class DataFunction;


// CIMServerDlg dialog
class CIMServerDlg : public CDialog
{
// Construction
public:
	CIMServerDlg(CWnd* pParent = NULL);	// standard constructor

public:
	//��Ӻ��������ͱ�������
	CServerSocket* listenSocket;								//���ڴ򿪷����
	CPtrList g_clientList;										//�������ڴ洢�û�
	bool g_bConnect;											//���ڱ�Ƿ�����״̬
	void AddClient();											//�����û�����Ӧ�û�����
	void RemoveClient(CServerSocket* pSocket);					//�Ƴ����ߵ��û�
	void RecvData(CServerSocket* pSocket);						//��ȡ����
	void UpdateEvent(CString str);								//�����¼���־
	BOOL WChar2MByte(LPCWSTR strBuff,LPSTR destBuff,int nLen);	//�ַ�ת��
	void SendCStringMsgToClient(CString str,CString strIp = _T(""),CString strPort = _T(""));	//����CString��Ϣ�������ͻ���
	void SendMsgToClient(void* pMsg,int nMsgLen,CString strIp = _T(""),CString strPort = _T(""));	//ͨ��ָ�뷢����Ϣ���ͻ���
	virtual BOOL PreTranslateMessage(MSG* pMsg);				//��д����ֹ����enter��escʱ�˳�����
	void AddControlText(CString str);							//��ӿ���̨��Ϣ
	DataFunction* g_pDataFunction;								//����������ݵ�ָ��

// Dialog Data
	enum { IDD = IDD_IMSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	UINT g_uPort;
	CEdit g_strEvent;
	UINT g_uUserCount;
	afx_msg void OnBnClickedStartserver();
	CString g_strControl;
	afx_msg void OnBnClickedDb();
	CEdit g_cEditControl;
};

#endif