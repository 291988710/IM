// IMServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IMServer.h"
#include "IMServerDlg.h"

#include "ado.h"

#include "json/json.h"

#ifdef _DEBUG
#pragma comment(lib."json_vc71_libmtd.lib")
#else
#pragma comment(lib."json_vc71_libmt.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CIMServerDlg dialog




CIMServerDlg::CIMServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIMServerDlg::IDD, pParent)
	, g_uPort(8000)
	, g_uUserCount(0)
	, g_strControl(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIMServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT, g_uPort);
	DDX_Control(pDX, IDC_EVENTRECORD, g_strEvent);
	DDX_Text(pDX, IDC_USERCOUNT, g_uUserCount);
	DDX_Text(pDX, IDC_CONTROL, g_strControl);
	DDX_Control(pDX, IDC_CONTROL, g_cEditControl);
}

BEGIN_MESSAGE_MAP(CIMServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_STARTSERVER, &CIMServerDlg::OnBnClickedStartserver)
	ON_BN_CLICKED(IDC_DB, &CIMServerDlg::OnBnClickedDb)
END_MESSAGE_MAP()


// CIMServerDlg message handlers

BOOL CIMServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	g_bConnect = false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIMServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIMServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIMServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/////////////////////////////////////////////////����������/////////////////////////////////////////////////

void CIMServerDlg::OnBnClickedStartserver()
{
	// TODO: Add your control notification handler code here
	//�˺������ڴ򿪻��߹رշ�������Create��Listen�ֱ��ʴ����������ͼ����ͻ��ˣ����ж˿ںŴӱ༭���ȡ��Ӧ�ó���Ŀ��÷�Χ��1024-65535��
	if (g_bConnect)
	{
		delete listenSocket;
		listenSocket = NULL;
		g_bConnect = false;
		SetDlgItemText(IDC_STARTSERVER,_T("StartServer"));
		UpdateEvent(_T("ϵͳ�رշ�����"));
		return;
	}
	listenSocket = new CServerSocket();
	listenSocket->g_pDlg = this;
	g_pDataFunction = new DataFunction();
	g_pDataFunction->g_pDlg = this;
	if (!listenSocket)
	{
		AfxMessageBox(_T("�����׽��ֳ���"));
		return;
	}
	//ָ���Ի���Ϊ���Ի���
	UpdateData(TRUE);
	if (!listenSocket->Create(g_uPort))//�������������׽��֣�IP��ַĬ�ϱ���IP
	{
		AfxMessageBox(_T("�����׽��ִ���"));
		listenSocket->Close();
		return;
	}
	if (!listenSocket->Listen())
	{
		AfxMessageBox(_T("����ʧ��"));
		listenSocket->Close();
		return;
	}
	g_bConnect = true;
	SetDlgItemText(IDC_STARTSERVER,_T("CloseServer"));
	UpdateEvent(_T("ϵͳ�򿪷�����"));
}

void CIMServerDlg::OnBnClickedDb()
{
	// TODO: Add your control notification handler code here
	CString strDBInfo;
	strDBInfo = _T("AccID\tPwd\r\n");
	ado ado1;
	ado1.GetAllAccountIDInfo();
	for (ado1.iter_VecAccID = ado1.vec_AccID.begin();ado1.iter_VecAccID != ado1.vec_AccID.end();ado1.iter_VecAccID++)
	{
		CString strAccId,strPwd;
		strAccId = *ado1.iter_VecAccID->strAccountID;
		strPwd = *ado1.iter_VecAccID->strPassword;
		strDBInfo = strDBInfo + strAccId + _T("\t") + strPwd + _T("\r\n");
	}

	const char* str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";  
	Json::Reader reader;  
	Json::Value root;  
	if (reader.parse(str, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{  
		string upload_id = root["uploadid"].asString();  // ���ʽڵ㣬upload_id = "UP000000"  
		int code = root["code"].asInt();    // ���ʽڵ㣬code = 100 
		CString strJsonTest;
		strJsonTest.Format(_T("strJsonTest\t%s\t%d"),upload_id,code);
		AddControlText(strJsonTest);
	}  
	AddControlText(strDBInfo);
}

/////////////////////////////////////////////////Socket����/////////////////////////////////////////////////

void CIMServerDlg::AddClient()
{
	//�˺�����CServerSocket���е�OnAccept��Ϣ�е��ã�������Ӧ�û����ӷ�������������Ҫ����ΪAccpet��
	//�����ӳɹ���ͨ������g_clientList�������û���������־��
	CServerSocket* pSocket = new CServerSocket;
	pSocket->g_pDlg = this;
	if (!pSocket)
	{
		AfxMessageBox(_T("�ڴ治�㣬�ͻ������ӷ�����ʧ��"));
		delete pSocket;
		return;
	}
	listenSocket->Accept(*pSocket);
	CString strIp;
	UINT uPort;
	pSocket->GetPeerName(strIp,uPort);

	pSocket->AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
	g_clientList.AddTail(pSocket);
	g_uUserCount = g_clientList.GetCount();
	UpdateData(FALSE);
	UpdateEvent(_T("�û����ӷ�����"));
}

void CIMServerDlg::RemoveClient(CServerSocket* pSocket)
{
	//�˺�����CServerSocket���е�OnClose��Ϣ�е��ã��õ�POSITION�ṹ�����Ҵ洢�û�����λ�û������ˣ��������û��ͷţ���������ɾ������������־��
	POSITION nPos = g_clientList.GetHeadPosition();
	POSITION nTmpPos = nPos;
	while (nPos)
	{
		CServerSocket* pSocketItem = (CServerSocket*)g_clientList.GetNext(nPos);
		if (pSocketItem->m_hSocket == pSocket->m_hSocket)
		{
			CString strIp;
			UINT uPort;
			pSocket->GetPeerName(strIp,uPort);
			CString strPort;
			strPort.Format(_T("%d"),uPort);
			CString strOnlinePort = _T("0");
			//������״̬ˢΪ0
			g_pDataFunction->Func_RefreshOnlineState(strIp,strPort,strOnlinePort);

			pSocketItem->Close();
			delete pSocketItem;
			g_clientList.RemoveAt(nTmpPos);
			g_uUserCount = g_clientList.GetCount();
			UpdateData(false);
			UpdateEvent(_T("�û��뿪"));
			return;
		}
		nTmpPos = nPos;
	}
}

void CIMServerDlg::RecvData(CServerSocket* pSocket)
{
	//�˺�����CServerSocket���е�OnReceive��Ϣ�е��ã����ڴ�����յ������ݣ���������ת���������û���ͨ��CSocket���GetPeerName�������Ի�ȡ�û���IP�Ͷ˿ںš�
	char* pData = NULL;
	pData = new char[1024];
	memset(pData,0,sizeof(char)* 1024);
	UCHAR leng = 0;
	CString str;
	if (pSocket->Receive(pData,1024,0) != SOCKET_ERROR)
	{
		str = pData;
		AddControlText(str);
		CString strIp;
		UINT uPort;
		pSocket->GetPeerName(strIp,uPort);
		CString strPort;
		strPort.Format(_T("%d"),uPort);
		str = str + _T("^") + strIp + _T("^") + strPort;
		//������յ������ݣ���������֮����"^"����
		g_pDataFunction->SeperateMsgFormClient(str);
		/*for (g_pDataFunction->g_iterVecSeperatedData = g_pDataFunction->g_vecSeperatedData.begin();g_pDataFunction->g_iterVecSeperatedData != g_pDataFunction->g_vecSeperatedData.end();g_pDataFunction->g_iterVecSeperatedData++)
		{
			AddControlText(*g_pDataFunction->g_iterVecSeperatedData);
		}*/
	}
	delete pData;
	pData = NULL;
}

void CIMServerDlg::UpdateEvent(CString str)
{
	CString string;
	CTime time = CTime::GetCurrentTime();
	str += _T("\r\n");
	string = time.Format(_T("%Y/%m/%d %H:%M:%S ")) + str;
	int lastLine = g_strEvent.LineIndex(g_strEvent.GetLineCount() - 1);
	g_strEvent.SetSel(lastLine+1,lastLine+2,0);
	g_strEvent.ReplaceSel(string);
	g_strEvent.LineScroll(lastLine);
}

BOOL CIMServerDlg::WChar2MByte(LPCWSTR strBuff,LPSTR destBuff,int nLen)
{
	//�˺����ڷ��ͺ���SendCStringMsgToClient�е��ã������ַ�����ת���������ַ�ת��Ϊ���ַ���������ת���Ļ������շ�ֻ�ܽ���һ���ֽڡ�
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP,0,strBuff,-1,destBuff,0,0,FALSE);
	if (n < nLen)
	{
		return FALSE;
	}
	WideCharToMultiByte(CP_OEMCP,0,strBuff,-1,destBuff,nLen,0,FALSE);
	return TRUE;
}

void CIMServerDlg::SendCStringMsgToClient(CString str,CString strIp,CString strPort)
{
	if (strIp.IsEmpty() && strPort.IsEmpty())
	{
		char* pSend = new char[str.GetLength()];
		memset(pSend,0,str.GetLength()*sizeof(char));
		if (!WChar2MByte(str.GetBuffer(0),pSend,str.GetLength()))
		{
			AfxMessageBox(_T("�ַ�ת��ʧ��"));
			delete pSend;
			return;
		}
		POSITION nPos = g_clientList.GetHeadPosition();
		while (nPos)
		{
			CServerSocket* pTemp = (CServerSocket*)g_clientList.GetNext(nPos);
			pTemp->Send(pSend,str.GetLength());
		}
		delete pSend;
	}
	else
	{
		char* pSend = new char[str.GetLength()];
		memset(pSend,0,str.GetLength()*sizeof(char));
		if (!WChar2MByte(str.GetBuffer(0),pSend,str.GetLength()))
		{
			AfxMessageBox(_T("�ַ�ת��ʧ��"));
			delete pSend;
			return;
		}
		POSITION nPos = g_clientList.GetHeadPosition();
		while (nPos)
		{
			CServerSocket* pTemp = (CServerSocket*)g_clientList.GetNext(nPos);
			CString strIpTemp;
			UINT uPortTemp;
			pTemp->GetPeerName(strIpTemp,uPortTemp);
			CString strPortTemp;
			strPortTemp.Format(_T("%d"),uPortTemp);
			if (strIp == strIpTemp && strPort == strPortTemp)
			{
				pTemp->Send(pSend,str.GetLength());
				break;
			}
		}
		delete pSend;
	}
}

void CIMServerDlg::SendMsgToClient(void* pMsg,int nMsgLen,CString strIp /* = _T("") */,CString strPort /* = _T("") */)
{
	//pMsg�����ݵ�ָ�룬nMsgLen�����ݵĳ���
	char* pSend = new char[nMsgLen];
	memset(pSend,0,nMsgLen);
	memcpy(pSend,pMsg,nMsgLen);
	if (!WChar2MByte((LPCWSTR)pMsg,pSend,nMsgLen))
	{
		AfxMessageBox(_T("�ַ�ת��ʧ��"));
		delete pSend;
		return;
	}
	POSITION nPos = g_clientList.GetHeadPosition();
	while (nPos)
	{
		CServerSocket* pTemp = (CServerSocket*)g_clientList.GetNext(nPos);
		CString strIpTemp;
		UINT uPortTemp;
		pTemp->GetPeerName(strIpTemp,uPortTemp);
		CString strPortTemp;
		strPortTemp.Format(_T("%d"),uPortTemp);
		if (strIp == strIpTemp && strPort == strPortTemp)
		{
			pTemp->Send(pSend,nMsgLen);
			break;
		}
	}
	delete pSend;
}

BOOL CIMServerDlg::PreTranslateMessage(MSG* pMsg)
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

void CIMServerDlg::AddControlText(CString str)
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
	//�Զ�����������
	int lastLine = g_cEditControl.LineIndex(g_cEditControl.GetLineCount() - 1);
	g_cEditControl.LineScroll(lastLine);
}