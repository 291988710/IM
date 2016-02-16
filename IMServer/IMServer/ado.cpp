#include "StdAfx.h"
#include "ado.h"

ado::ado(void)
{
	CoInitialize(NULL);
	try
	{
		g_pConn.CreateInstance(__uuidof(Connection));
		_bstr_t strConnect="Provider=Microsoft.jet.OLEDB.4.0;Data Source=AccountID.mdb";
		g_pConn->Open(strConnect,"","",0);
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

ado::~ado(void)
{
}

void ado::GetAllAccountIDInfo()
{
	CString strSql = _T("select * from tb_ActID");
	_bstr_t sqlToExcute = strSql;

	_RecordsetPtr pRec = NULL;

	try
	{
		pRec.CreateInstance(__uuidof(Recordset));
		pRec = g_pConn->Execute(sqlToExcute,NULL,adCmdText);
		_variant_t roader;
		while(!pRec->adoEOF)
		{
			st_AccID temp_stActID;
			roader = pRec->GetCollect(_T("AccountID"));
			if (roader.vt != VT_NULL)
			{
				temp_stActID.strAccountID = roader;
			}
			roader = pRec->GetCollect(_T("Password"));
			if (roader.vt != VT_NULL)
			{
				temp_stActID.strPassword = roader;
			}
			vec_AccID.push_back(temp_stActID);
			pRec->MoveNext();
		}
		pRec->Close();
	}
	catch (_com_error& e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Error msg is: %s"),e.ErrorMessage());
		AfxMessageBox(strErrorMsg);
		return;
	}
}

int ado::Ado_ValidateAccountAndPassword(CString strAccount,CString strPassword)
{
	CString strSql;
	strSql.Format(_T("select * from tb_ActID where AccountID = '%s'"),strAccount);
	_bstr_t sqlToExcute = strSql;
	CString strAdoPassword;
	CString strOnline;

	_RecordsetPtr pRec = NULL;

	try
	{
		pRec.CreateInstance(__uuidof(Recordset));
		pRec = g_pConn->Execute(sqlToExcute,NULL,adCmdText);
		_variant_t roader;
		while(!pRec->adoEOF)
		{
			roader = pRec->GetCollect(_T("Password"));
			if (roader.vt != VT_NULL)
			{
				strAdoPassword = roader;
			}
			roader = pRec->GetCollect(_T("Online"));
			if (roader.vt != VT_NULL)
			{
				strOnline = roader;
			}
			pRec->MoveNext();
		}
		pRec->Close();
	}
	catch (_com_error& e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Error msg is: %s"),e.ErrorMessage());
		AfxMessageBox(strErrorMsg);
		return eS2C_ValidateAccountAndPassword_Error;
	}

	if (strAdoPassword == "")	//没有该账号
	{
		return eS2C_ValidateAccountAndPassword_NoAccount;
	}
	else if (strOnline == _T("1"))	//该账号已登录
	{
		return eS2C_ValidateAccountAndPassword_AccountOnline;
	}
	else if (strAdoPassword != strPassword)	//密码不正确
	{
		return eS2C_ValidateAccountAndPassword_ErrorPassword;
	}
	else if (strAdoPassword == strPassword)	//该账号存在并且密码正确
	{
		return eS2C_ValidateAccountAndPassword_OK;
	}
}

int ado::Ado_BindAccountWithIpPort(CString strAccount,CString strIp,CString strPort)
{
	CString strSql;
	CString strOnline = _T("1");
	strSql.Format(_T("update tb_ActID set IP = '%s',Port = '%s',Online = '%s' where AccountID = '%s'"),strIp,strPort,strOnline,strAccount);
	_bstr_t sqlToExcute = strSql;

	_RecordsetPtr pRec = NULL;

	try
	{
		pRec.CreateInstance(__uuidof(Recordset));
		pRec = g_pConn->Execute(sqlToExcute,NULL,adCmdText);
		return 0;
	}
	catch (_com_error& e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Error msg is: %s"),e.ErrorMessage());
		AfxMessageBox(strErrorMsg);
		return 1;
	}
}

int ado::Ado_ClearOnlineState(CString strIp,CString strPort,CString strOnlineState)
{
	CString strSql;
	CString strIpEmpty = _T("");
	CString strPortEmpty = _T("");
	strSql.Format(_T("update tb_ActID set IP = '%s',Port = '%s',Online = '%s' where IP = '%s' and Port = '%s'"),strIpEmpty,strPortEmpty,strOnlineState,strIp,strPort);
	_bstr_t sqlToExcute = strSql;

	_RecordsetPtr pRec = NULL;

	try
	{
		pRec.CreateInstance(__uuidof(Recordset));
		pRec = g_pConn->Execute(sqlToExcute,NULL,adCmdText);
		return eS2C_ClearOnlineState_OK;
	}
	catch (_com_error& e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Error msg is: %s"),e.ErrorMessage());
		AfxMessageBox(strErrorMsg);
		return eS2C_ClearOnlineState_Error;
	}
}

int ado::Ado_QueryFriendList(CString strAccount,vector<CString>& vecAccount)
{
	CString strSql;
	strSql.Format(_T("select * from tb_ActID where AccountID <> '%s'"),strAccount);
	_bstr_t sqlToExcute = strSql;

	_RecordsetPtr pRec = NULL;

	try
	{
		pRec.CreateInstance(__uuidof(Recordset));
		pRec = g_pConn->Execute(sqlToExcute,NULL,adCmdText);
		_variant_t roader;
		while(!pRec->adoEOF)
		{
			roader = pRec->GetCollect(_T("AccountID"));
			if (roader.vt != VT_NULL)
			{
				vecAccount.push_back(roader);
			}
			pRec->MoveNext();
		}
		pRec->Close();
		return eS2C_QueryFriendList_OK;
	}
	catch (_com_error& e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("Error msg is: %s"),e.ErrorMessage());
		AfxMessageBox(strErrorMsg);
		return eS2C_QueryFriendList_Error;
	}
}