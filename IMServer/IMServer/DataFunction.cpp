#include "StdAfx.h"
#include "DataFunction.h"

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
	//获取数据库的数据
	int nResult = g_pAdo->Ado_ValidateAccountAndPassword(strAccount,strPassword);
	if (nResult == eS2C_ValidateAccountAndPassword_OK)
	{
		//将Account和ip、port绑定在一起
		int nBindResult = g_pAdo->Ado_BindAccountWithIpPort(strAccount,strIp,strPort);
		if (nBindResult == 0)
		{
			g_pDlg->AddControlText(_T("绑定ip/port成功"));
		}
		else
		{
			g_pDlg->AddControlText(_T("绑定ip/port失败"));
		}
	}
	CString strResult;
	strResult.Format(_T("%d^%d"),eC2S_ValidateAccountAndPassword,nResult);
	
	g_pDlg->SendCStringMsgToClient(strResult,strIp,strPort);
}

void DataFunction::Func_RefreshOnlineState(CString strIp,CString strPort,CString strOnlineState)
{
	int nResult = g_pAdo->Ado_ClearOnlineState(strIp,strPort,strOnlineState);
	if (nResult == eS2C_ClearOnlineState_OK)
	{
		g_pDlg->AddControlText(_T("清空在线状态成功"));
	}
	else
	{
		g_pDlg->AddControlText(_T("清空在线状态失败"));
	}
}

void DataFunction::Func_QueryFriendList(CString strIp,CString strPort,CString strAccount)
{
	std::vector<CString> vecAccount;
	int nResult = g_pAdo->Ado_QueryFriendList(strAccount,vecAccount);
	if (nResult == eS2C_QueryFriendList_OK)
	{
		g_pDlg->AddControlText(_T("请求好友列表成功"));
	}
	else
	{
		g_pDlg->AddControlText(_T("请求好友列表失败"));
	}
	//用socket发送vector，socket只能发送原始的byte数据，而直接发送指针过去是无用的，因此只能先把vector序列化，用某种标识符将其分割开来，再发送，
	//分批次发送,全部转为cstring类型发送。
	void* pMsg = &vecAccount;
	int nMsgLen = 0;
	for (vector<CString>::iterator iter_vecAccount = vecAccount.begin();iter_vecAccount != vecAccount.end();iter_vecAccount++)
	{
		//用'~'来区分每个数据
		if (iter_vecAccount == vecAccount.end())
		{
		}
		*iter_vecAccount = *iter_vecAccount + _T("~");
		CString strTemp = *iter_vecAccount;
		nMsgLen = nMsgLen + strTemp.GetLength();
	}
	g_pDlg->SendMsgToClient(pMsg,nMsgLen);
}