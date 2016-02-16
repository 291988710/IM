// IMClientDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CIMClientDlg �Ի���




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


// CIMClientDlg ��Ϣ�������

BOOL CIMClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//����Login�����С����ɫ
	CFont cLoginTextFont;
	cLoginTextFont.CreatePointFont(280,L"����");
	GetDlgItem(IDC_LOGIN)->SetFont(&cLoginTextFont);
	GetDlgItem(IDC_CONTROL)->SetWindowText(_T("Control Text"));
	g_bConnect = false;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CIMClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CIMClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////����������///////////////////////////////////////////

void CIMClientDlg::OnStnClickedLogin()
{
	// TODO: Add your control notification handler code here
}

void CIMClientDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (g_bConnect)//��������ӣ���Ͽ�������
	{
		g_bConnect = false;
		pSocket->Close();
		delete pSocket;
		pSocket = NULL;
		AddControlText(_T("�Ͽ�������"));
		return;
	}
	else//δ���ӣ������ӷ�����
	{
		pSocket = new CClientSocket();
		if (!pSocket->Create())
		{
			AfxMessageBox(_T("�����׽���ʧ��"));
			return;
		}
	}
	if (!pSocket->Connect(_T("127.0.0.1"),port))
	{
		AfxMessageBox(_T("���ӷ�����ʧ��"));
		return;
	}
	else
	{
		g_bConnect = true;
		AddControlText(_T("���ӷ�����"));
		pSocket->g_pDlg = this;
	}
	//���������˷���ˣ���ʼ�����û�����������Ϣ
	AddControlText(_T("��ʼ�����û�������"));
	UpdateData(TRUE);
	CString strAcc = g_strAccount;
	CString strPwd = g_strPassword;
	if (strAcc.IsEmpty())
	{
		AfxMessageBox(_T("�������˺�"));
		return;
	}
	else if (strPwd.IsEmpty())
	{
		AfxMessageBox(_T("����������"));
		return;
	}
	CString strMsgType;
	strMsgType.Format(_T("%d"),eC2S_ValidateAccountAndPassword);
	CString strSendMsg = strMsgType + _T("^") + strAcc + _T("^") + strPwd;
	SendMsgToServer(strSendMsg);
	//OnOK();
}

////////////////////////////////////////////////���ܺ���////////////////////////////////////////////////////////

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
		AfxMessageBox(_T("�ַ�ת��ʧ��"));
		delete pBuff;
		return;
	}
	pSocket->SendMsg(pBuff,str.GetLength()*sizeof(char));
}

void CIMClientDlg::ReceiveMsgFromServer(CString str)
{
	AddControlText(str);
	//Unicode��CStringתchar*
	char *szTemp=new char[str.GetLength()+1];
	WideCharToMultiByte(CP_ACP,0,str.GetBuffer(),-1,szTemp,str.GetLength()+1,NULL,NULL);

	Json::Reader reader;  
	Json::Value root;  
	CString strJsonTest;
	if (reader.parse(szTemp, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
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
	//�Զ�����������
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
			//���ݲ���
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
			AfxMessageBox(_T("�˺Ŵ���"));
			break;
		}
	case eS2C_ValidateAccountAndPassword_ErrorPassword:
		{
			AfxMessageBox(_T("�������"));
			break;
		}
	case eS2C_ValidateAccountAndPassword_AccountOnline:
		{
			AfxMessageBox(_T("���˺��ѵ�¼"));
			break;
		}
	case eS2C_ValidateAccountAndPassword_Error:
		{
			AfxMessageBox(_T("δ֪����"));
			break;
		}
	default:
		break;
	}
}