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
	//添加函数声明和变量定义
	CServerSocket* listenSocket;								//用于打开服务端
	CPtrList g_clientList;										//链表用于存储用户
	bool g_bConnect;											//用于标记服务器状态
	void AddClient();											//增加用户，响应用户请求
	void RemoveClient(CServerSocket* pSocket);					//移除下线的用户
	void RecvData(CServerSocket* pSocket);						//获取数据
	void UpdateEvent(CString str);								//更新事件日志
	BOOL WChar2MByte(LPCWSTR strBuff,LPSTR destBuff,int nLen);	//字符转换
	void SendCStringMsgToClient(CString str,CString strIp = _T(""),CString strPort = _T(""));	//发送CString消息给各个客户端
	void SendMsgToClient(void* pMsg,int nMsgLen,CString strIp = _T(""),CString strPort = _T(""));	//通过指针发送消息给客户端
	virtual BOOL PreTranslateMessage(MSG* pMsg);				//重写，防止按下enter或esc时退出程序
	void AddControlText(CString str);							//添加控制台信息
	DataFunction* g_pDataFunction;								//处理各种数据的指针

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