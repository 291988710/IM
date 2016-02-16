#pragma once
#include <vector>
#include "MsgProtocol.h"
using namespace std;
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF" ,"adoEOF")

class ado
{
public:
	typedef struct sttTb_AccID
	{
		CString strAccountID;
		CString strPassword;
	}st_AccID;
public:
	ado(void);
	~ado(void);
	void GetAllAccountIDInfo();
	int Ado_ValidateAccountAndPassword(CString,CString);
	int Ado_BindAccountWithIpPort(CString,CString,CString);
	int Ado_ClearOnlineState(CString,CString,CString);
	int Ado_QueryFriendList(CString,vector<CString>&);
public:
	_ConnectionPtr g_pConn;
	_RecordsetPtr g_pRecSet;
	st_AccID* pst_AccID;
	vector<st_AccID> vec_AccID;
	vector<st_AccID>::iterator iter_VecAccID;
};