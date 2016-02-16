// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IMClient.h"
#include "ClientDlg.h"


// CClientDlg dialog

IMPLEMENT_DYNAMIC(CClientDlg, CDialog)

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
	, g_strAccount(_T(""))
	, g_strPassword(_T(""))
	, g_strIp(_T(""))
	, g_strPort(_T(""))
{
}

CClientDlg::~CClientDlg()
{
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_ACCOUNT, g_strAccount);
	DDX_Text(pDX, IDC_STATIC_PASSWORD, g_strPassword);
	DDX_Text(pDX, IDC_STATIC_IP, g_strIp);
	DDX_Text(pDX, IDC_STATIC_PORT, g_strPort);
	DDX_Control(pDX, IDC_LIST_FRIEND, g_listCtlFriend);
}


BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
END_MESSAGE_MAP()

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//设置子窗口在任务栏也能够显示图标
	ModifyStyleEx(0,WS_EX_APPWINDOW);
	ShowWindow(SW_SHOW);
	//发送获取好友的信息
	CString strMsgType;
	strMsgType.Format(_T("%d"),eC2S_QueryFriendList);
	CString strSendMsg = strMsgType + _T("^") + g_strAccount;
	SendMsgToServer(strSendMsg);

	//显示好友栏的内容
	CRect rect;
	//获取好友列表视图控件的位置和大小
	g_listCtlFriend.GetClientRect(&rect);
	//为列表视图控件添加全行选中和栅格风格
	g_listCtlFriend.SetExtendedStyle(g_listCtlFriend.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//为列表视图添加三列
	g_listCtlFriend.InsertColumn(0,_T("Language"),LVCFMT_CENTER,rect.Width()/3,0);
	g_listCtlFriend.InsertColumn(1,_T("Rank1"),LVCFMT_CENTER,rect.Width()/3,1);
	g_listCtlFriend.InsertColumn(2,_T("Rank2"),LVCFMT_CENTER,rect.Width()/3,2);
	//在列表视图控件中插入列表项，并设置列表子项文本
	g_listCtlFriend.InsertItem(0,_T("Java"));
	g_listCtlFriend.SetItemText(0,1,_T("1"));
	g_listCtlFriend.SetItemText(0,2,_T("1"));
	g_listCtlFriend.InsertItem(1,_T("C"));
	g_listCtlFriend.SetItemText(1,1,_T("1"));
	g_listCtlFriend.SetItemText(1,2,_T("1"));
	g_listCtlFriend.InsertItem(2,_T("A"));
	g_listCtlFriend.SetItemText(2,1,_T("1"));
	g_listCtlFriend.SetItemText(2,2,_T("1"));
	g_listCtlFriend.InsertItem(3,_T("B"));
	g_listCtlFriend.SetItemText(3,1,_T("1"));
	g_listCtlFriend.SetItemText(3,2,_T("1"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnCancel()
{
	exit(0);	//这样关闭子窗口时，父窗口也会随之销毁
}

// CClientDlg message handlers

////////////////////////////////////////////////////////功能函数/////////////////////////////////////////////////////////
void CClientDlg::SendMsgToServer(CString str)
{
	if (!g_bConnect || str == "")
	{
		return;
	}
	char* pBuff = new char[str.GetLength()];
	memset(pBuff,0,str.GetLength()*sizeof(char));
	if (!WChar2MByte(str.GetBuffer(0),pBuff,str.GetLength()))
	{
		AfxMessageBox(_T("字符转换失败"));
		delete pBuff;
		return;
	}
	g_pSocket->SendMsg(pBuff,str.GetLength()*sizeof(char));
}

void CClientDlg::ReceiveMsgFromServer(CString str)
{
	/*AddControlText(str);
	SeperateMsgFromServer(str);
	CString strMsgType = g_vecSeperatedData[0];
	int nMsgType = _ttoi(strMsgType);
	switch(nMsgType)
	{
	case eValidateAccountAndPassword:
		{
			CString strResult = g_vecSeperatedData[1];
			Func_OnValidateAccountAndPassword(strResult);
			break;
		}
	default:
		break;
	}*/
}

BOOL CClientDlg::WChar2MByte(LPCWSTR lpSrc,LPSTR lpDest,int nLen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP,0,lpSrc,-1,lpDest,0,0,FALSE);
	if (n < nLen)
	{
		return FALSE;
	}
	WideCharToMultiByte(CP_OEMCP,0,lpSrc,-1,lpDest,nLen,0,FALSE);
	return TRUE;
}