// IMClientDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "IMClient.h"
#include "IMClientDlg.h"

#include "json/json.h"

#ifdef _DEBUG
#pragma comment(lib."json_vc71_libmtd.lib")
#else
#pragma comment(lib."json_vc71_libmt.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define port 8000


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CIMClientDlg 对话框




CIMClientDlg::CIMClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIMClientDlg::IDD, pParent)
	, g_strControl(_T(""))
	, g_strAccount(_T(""))
	, g_strPassword(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIMClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONTROL, g_strControl);
	DDX_Text(pDX, IDC_ACCOUNT, g_strAccount);
	DDX_Text(pDX, IDC_PASSWORD, g_strPassword);
	DDX_Control(pDX, IDC_CONTROL, g_cEditControl);
}

BEGIN_MESSAGE_MAP(CIMClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_LOGIN, &CIMClientDlg::OnStnClickedLogin)
	ON_BN_CLICKED(IDOK, &CIMClientDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CIMClientDlg 消息处理程序

BOOL CIMClientDlg::OnInitDialog()
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
	//设置Login字体大小和颜色
	CFont cLoginTextFont;
	cLoginTextFont.CreatePointFont(280,L"楷体");
	GetDlgItem(IDC_LOGIN)->SetFont(&cLoginTextFont);
	GetDlgItem(IDC_CONTROL)->SetWindowText(_T("Control Text"));
	g_bConnect = false;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIMClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIMClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIMClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////界面点击函数///////////////////////////////////////////

void CIMClientDlg::OnStnClickedLogin()
{
	// TODO: Add your control notification handler code here
}

void CIMClientDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (g_bConnect)//如果已连接，则断开服务器
	{
		g_bConnect = false;
		pSocket->Close();
		delete pSocket;
		pSocket = NULL;
		AddControlText(_T("断开服务器"));
		return;
	}
	else//未连接，则连接服务器
	{
		pSocket = new CClientSocket();
		if (!pSocket->Create())
		{
			AfxMessageBox(_T("创建套接字失败"));
			return;
		}
	}
	if (!pSocket->Connect(_T("127.0.0.1"),port))
	{
		AfxMessageBox(_T("连接服务器失败"));
		return;
	}
	else
	{
		g_bConnect = true;
		AddControlText(_T("连接服务器"));
		pSocket->g_pDlg = this;
	}
	//已连接上了服务端，开始发送用户名和密码信息
	AddControlText(_T("开始发送用户名密码"));
	UpdateData(TRUE);
	CString strAcc = g_strAccount;
	CString strPwd = g_strPassword;
	if (strAcc.IsEmpty())
	{
		AfxMessageBox(_T("请输入账号"));
		return;
	}
	else if (strPwd.IsEmpty())
	{
		AfxMessageBox(_T("请输入密码"));
		return;
	}
	CString strMsgType;
	strMsgType.Format(_T("%d"),eC2S_ValidateAccountAndPassword);
	CString strSendMsg = strMsgType + _T("^") + strAcc + _T("^") + strPwd;
	SendMsgToServer(strSendMsg);
	//OnOK();
}

////////////////////////////////////////////////功能函数////////////////////////////////////////////////////////

BOOL CIMClientDlg::WChar2MByte(LPCWSTR strBuff,LPSTR destBuff,int nLen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP,0,strBuff,-1,destBuff,0,0,FALSE);
	if (n < nLen)
	{
		return FALSE;
	}
	WideCharToMultiByte(CP_OEMCP,0,strBuff,-1,destBuff,nLen,0,FALSE);
	return TRUE;
}

BOOL CIMClientDlg::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->wParam)
	{
	case VK_RETURN:
	case VK_ESCAPE:
		return true;
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CIMClientDlg::SendMsgToServer(CString str)
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
	pSocket->SendMsg(pBuff,str.GetLength()*sizeof(char));
}

void CIMClientDlg::ReceiveMsgFromServer(CString str)
{
	AddControlText(str);
	//Unicode下CString转char*
	char *szTemp=new char[str.GetLength()+1];
	WideCharToMultiByte(CP_ACP,0,str.GetBuffer(),-1,szTemp,str.GetLength()+1,NULL,NULL);

	Json::Reader reader;  
	Json::Value root;  
	CString strJsonTest;
	if (reader.parse(szTemp, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{  
		int nMsgType = root["MsgType"].asInt();   
		int nResult = root["Result"].asInt();    
		CString strMsgType;
		strMsgType.Format(_T("%d"),nMsgType);
		CString strResult;
		strResult.Format(_T("%d"),nResult);
		strJsonTest = strMsgType + _T("\r\n") + strResult;
		AddControlText(strJsonTest);
	}  

	/*SeperateMsgFromServer(str);
	CString strMsgType = g_vecSeperatedData[0];
	int nMsgType = _ttoi(strMsgType);
	switch(nMsgType)
	{
	case eC2S_ValidateAccountAndPassword:
		{
			CString strResult = g_vecSeperatedData[1];
			Func_OnValidateAccountAndPassword(strResult);
			break;
		}
	default:
		break;
	}*/
}

void CIMClientDlg::SeperateMsgFromServer(CString str)
{
	g_vecSeperatedData.clear();
	int nPrePos = 0;
	int nCurPos = 0;
	while (-1 != str.Find('^',nPrePos + 1))
	{
		nCurPos = str.Find('^',nPrePos + 1);
		CString strTemp = str.Mid(nPrePos,nCurPos-nPrePos);
		nPrePos = nCurPos + 1;
		g_vecSeperatedData.push_back(strTemp);
	}	
	CString strTemp = str.Right(str.GetLength() - nCurPos - 1);
	g_vecSeperatedData.push_back(strTemp);
}

void CIMClientDlg::AddControlText(CString str)
{
	if (!g_strControl.IsEmpty())
	{
		g_strControl = g_strControl + _T("\r\n") + str;
	}
	else
	{
		g_strControl = str;
	}
	SetDlgItemText(IDC_CONTROL,g_strControl);
	//自动下拉滚动条
	int lastLine = g_cEditControl.LineIndex(g_cEditControl.GetLineCount() - 1);
	g_cEditControl.LineScroll(lastLine);
}

void CIMClientDlg::Func_OnValidateAccountAndPassword(CString strResult)
{
	int nResult = _ttoi(strResult);
	switch(nResult)
	{
	case eS2C_ValidateAccountAndPassword_OK:
		{
			CDialog::ShowWindow(SW_HIDE);
			//CDialog::DestroyWindow();
			//传递参数
			pClientDlg = new CClientDlg();
			pClientDlg->g_strAccount = g_strAccount;
			pClientDlg->g_strPassword = g_strPassword;
			CString strIp;
			UINT uPort;
			pSocket->GetPeerName(strIp,uPort);
			CString strPort;
			strPort.Format(_T("%d"),uPort);
			pClientDlg->g_strIp = strIp;
			pClientDlg->g_strPort = strPort;
			pClientDlg->g_pSocket = pSocket;
			pClientDlg->g_bConnect = g_bConnect;

			pClientDlg->DoModal();
			break;
		}
	case eS2C_ValidateAccountAndPassword_NoAccount:
		{
			AfxMessageBox(_T("账号错误"));
			break;
		}
	case eS2C_ValidateAccountAndPassword_ErrorPassword:
		{
			AfxMessageBox(_T("密码错误"));
			break;
		}
	case eS2C_ValidateAccountAndPassword_AccountOnline:
		{
			AfxMessageBox(_T("该账号已登录"));
			break;
		}
	case eS2C_ValidateAccountAndPassword_Error:
		{
			AfxMessageBox(_T("未知错误"));
			break;
		}
	default:
		break;
	}
}