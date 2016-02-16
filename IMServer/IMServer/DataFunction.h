//DataFunction.h
//用于处理服务端接收到的各种数据的各种功能性函数
//2016.1.11
#pragma once
#include <vector>
#include "MsgProtocol.h"
#include "ado.h"
#include "IMServerDlg.h"

class ado;
class CIMServerDlg;

class DataFunction
{
public:
	DataFunction(void);
	~DataFunction(void);
public:
	std::vector<CString> g_vecSeperatedData;			//存储分割之后的数据
	std::vector<CString>::iterator g_iterVecSeperatedData;
	ado* g_pAdo;
	CIMServerDlg* g_pDlg;
public:
	void SeperateMsgFormClient(CString);		//将客户端传递过来的数据根据"^"标识符来进行分割
	void DealWithDiffAction();					//根据不同的数值，对得到的数据进行不同的操作

	void Func_ValidateAccountAndPassword(CString,CString,CString,CString);		//验证用户名密码是否正确
	void Func_RefreshOnlineState(CString,CString,CString);						//刷新用户在线状态
	void Func_QueryFriendList(CString,CString,CString);
};
