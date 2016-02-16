#include "StdAfx.h"
#include "DataFunction.h"

#include "json/json.h"

#ifdef _DEBUG
#pragma comment(lib."json_vc71_libmtd.lib")
#else
#pragma comment(lib."json_vc71_libmt.lib")
#endif

DataFunction::DataFunction(void)
{
	g_pAdo = new ado();
	g_pDlg = new CIMServerDlg();
}

DataFunction::~DataFunction(void)
{
}

void DataFunction::SeperateMsgFormClient(CString str)
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
	DealWithDiffAction();
}

void DataFunction::DealWithDiffAction()
{
	CString strActionType = g_vecSeperatedData[0];
	int nActionType = _ttoi(strActionType);
	switch(nActionType)
	{
	case eC2S_ValidateAccountAndPassword:
		{
			CString strAccount = g_vecSeperatedData[1];
			CString strPassword = g_vecSeperatedData[2];
			CString strIp = g_vecSeperatedData[3];
			CString strPort = g_vecSeperatedData[4];
			Func_ValidateAccountAndPassword(strAccount,strPassword,strIp,strPort);
		}
		break;
	case eC2S_QueryFriendList:
		{
			CString strAccount = g_vecSeperatedData[1];
			CString strIp = g_vecSeperatedData[2];
			CString strPort = g_vecSeperatedData[3];
			Func_QueryFriendList(strIp,strPort,strAccount);
		}
	default:
		break;
	}
}

void DataFunction::Func_ValidateAccountAndPassword(CString strAccount,CString strPassword,CString strIp,CString strPort)
{
	//��ȡ���ݿ������
	Json::Value jsonRoot;
	jsonRoot["MsgType"] = eC2S_ValidateAccountAndPassword;

	int nResult = g_pAdo->Ado_ValidateAccountAndPassword(strAccount,strPassword);
	jsonRoot["Result"] = nResult;
	if (nResult == eS2C_ValidateAccountAndPassword_OK)
	{
		//��Account��ip��port����һ��
		int nBindResult = g_pAdo->Ado_BindAccountWithIpPort(strAccount,strIp,strPort);
		if (nBindResult == 0)
		{
			g_pDlg->AddControlText(_T("��ip/port�ɹ�"));
		}
		else
		{
			g_pDlg->AddControlText(_T("��ip/portʧ��"));
		}
	}
	//CString strResult;
	//strResult.Format(_T("%d^%d"),ES2C_ValidateAccountAndPassword,nResult);
	string msg = jsonRoot.toStyledString();
	CString strMsg;
	strMsg = msg.c_str();
	
	g_pDlg->SendMsgToClient(strMsg,strIp,strPort);
}

void DataFunction::Func_RefreshOnlineState(CString strIp,CString strPort,CString strOnlineState)
{
	int nResult = g_pAdo->Ado_ClearOnlineState(strIp,strPort,strOnlineState);
	if (nResult == eS2C_ClearOnlineState_OK)
	{
		g_pDlg->AddControlText(_T("�������״̬�ɹ�"));
	}
	else
	{
		g_pDlg->AddControlText(_T("�������״̬ʧ��"));
	}
}

void DataFunction::Func_QueryFriendList(CString strIp,CString strPort,CString strAccount)
{
	std::vector<CString> vecAccount;
	int nResult = g_pAdo->Ado_QueryFriendList(strAccount,vecAccount);
	Json::Value jsonRoot;
	jsonRoot["MsgType"] = eC2S_QueryFriendList;
	jsonRoot["Result"] = nResult;

	if (nResult == eS2C_QueryFriendList_OK)
	{
		g_pDlg->AddControlText(_T("��������б�ɹ�"));
	}
	else
	{
		g_pDlg->AddControlText(_T("��������б�ʧ��"));
	}
	//��socket����vector��socketֻ�ܷ���ԭʼ��byte���ݣ���ֱ�ӷ���ָ���ȥ�����õģ�����json����
	Json::Value jsonAccountArray;
	for (vector<CString>::iterator iter_vecAccount = vecAccount.begin();iter_vecAccount != vecAccount.end();iter_vecAccount++)
	{
		Json::Value jsonAccount;
		CString strTemp = *iter_vecAccount;

		//Unicode��CStringתstring
		char *szTemp=new char[strTemp.GetLength()+1];
		WideCharToMultiByte(CP_ACP,0,strTemp.GetBuffer(),-1,szTemp,strTemp.GetLength()+1,NULL,NULL);
		string sTemp = szTemp;
		jsonAccount = sTemp;

		jsonAccountArray.append(jsonAccount);
	}
	jsonRoot["Account"] = jsonAccountArray;
	string msg = jsonRoot.toStyledString();
	CString strMsg;
	strMsg = msg.c_str();

	g_pDlg->SendMsgToClient(strMsg,strIp,strPort);
}