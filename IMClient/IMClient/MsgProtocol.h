//MsgProtocol.h
//消息协议文件，封装各种不同类型的消息
//2016.1.8
#ifndef MSGPROTOCOL_H
#define MSGPROTOCOL_H

/////////////////////////////////////接收消息的类型////////////////////////////////////
enum EC2S_MsgType
{
	eC2S_ValidateAccountAndPassword,
	eC2S_QueryFriendList,
};

enum ES2C_ValidateAccountAndPassword
{
	eS2C_ValidateAccountAndPassword_OK,
	eS2C_ValidateAccountAndPassword_NoAccount,
	eS2C_ValidateAccountAndPassword_ErrorPassword,
	eS2C_ValidateAccountAndPassword_AccountOnline,
	eS2C_ValidateAccountAndPassword_Error,
};

enum ES2C_ClearOnlineState
{
	eS2C_ClearOnlineState_OK,
	eS2C_ClearOnlineState_Error,
};

enum ES2C_QueryFriendList
{
	eS2C_QueryFriendList_OK,
	eS2C_QueryFriendList_Error,
};

#endif