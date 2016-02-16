//DataFunction.h
//���ڴ������˽��յ��ĸ������ݵĸ��ֹ����Ժ���
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
	std::vector<CString> g_vecSeperatedData;			//�洢�ָ�֮�������
	std::vector<CString>::iterator g_iterVecSeperatedData;
	ado* g_pAdo;
	CIMServerDlg* g_pDlg;
public:
	void SeperateMsgFormClient(CString);		//���ͻ��˴��ݹ��������ݸ���"^"��ʶ�������зָ�
	void DealWithDiffAction();					//���ݲ�ͬ����ֵ���Եõ������ݽ��в�ͬ�Ĳ���

	void Func_ValidateAccountAndPassword(CString,CString,CString,CString);		//��֤�û��������Ƿ���ȷ
	void Func_RefreshOnlineState(CString,CString,CString);						//ˢ���û�����״̬
	void Func_QueryFriendList(CString,CString,CString);
};
