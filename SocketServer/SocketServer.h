
// SocketServer.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CSocketServerApp:
// 有关此类的实现，请参阅 SocketServer.cpp
//

class CSocketServerApp : public CWinApp
{
public:
	CSocketServerApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CSocketServerApp theApp;

#define ID_MAX                          13
#define KEY_MAX                         17
#define NAME_MAX						50
#define EMAIL_MAX						33
#define FRIEND_MAX						50
#define MSG_SIZE_MAX					3358


// 注册
#define REGISTER                        5000
#define REGISTER_SUCCESS                5001
#define REGISTER_FAILED                 5002

// 登录
#define LOGIN                           5010
#define LOGIN_SUCCESS                   5011
//#define LOGIN_FAILED                    5012
#define LOGIN_FORCE                     5013
#define LOGIN_OUT						5014

// 更改信息 未用到
#define RESET_PASSWORD                  5020
#define RESET_PASSWORD_SUCCESS          5021
#define RESET_PASSWORD_FAILED           5022
#define RESET_USERNAME                  5023
#define RESET_USERNAME_SUCCESS          5024
#define RESET_USERNAME_FAILED           5025

// 聊天消息
#define CHATING_TEXT_MSG                5030

// 获取好友信息
#define GET_FRIEND_INFO                 5040	// 单个
#define GET_ALL_FRIEND_INFO             5041	// 全部
#define GET_FRIEND_LIST                 5042
#define GET_STRANGER_INFO               5043

// 添加或删除好友
#define ADD_FRIEND_REQUEST				5050
#define ADD_FRIEND_ANSWER				5051
#define ADD_FRIEND_AGREE				5055
#define ADD_FRIEND_REFUSE				5056
#define ADD_FRIEND_FAILED               5053
#define DELETE_FRIEND                   5054

// 账号状态
#define SET_USER_STATUS					5064
#define STATUS_ONLINE                   5060
#define STATUS_BUSY                     5061
#define STATUS_HIDE                     5062
#define STATUS_OFFLINE                  5063

//#define MESSAGE_SYSTEM					5070

//***下面是通信用的结构体***
struct MSG_LOGIN // 登录消息
{
	int  nType; // LOGIN
	char nID[ID_MAX]; // 账号最多12位
	char Key[KEY_MAX]; // 密码最多16位
	int nStatus;
};
struct MSG_TRANSPOND // 转发消息
{
	int  nType; // CHATING_TEXT_MSG
	char ToID[ID_MAX]; // 接收方
	char FromID[ID_MAX]; // 发送方
	char Date[11]; // yyyy-mm-dd
	char Time[9]; // hh-mm-ss
	char Data[200]; // 聊天消息
	int nReturn; // 附加信息
};
struct MSG_USERINFO // 用户信息
{
	int  nType; // GET_FRIEND_LIST
	int  nStatus; // 在线状态
	char nID[ID_MAX]; // ID
	char Name[NAME_MAX]; // 昵称
	char Email[EMAIL_MAX]; // 邮件 最多32位 ANSI编码 无中文 最后一位是‘\0’
	char Sex; // 性别 一位 m男性 f女性
	int  nAge; // 年龄
	char FriendList[FRIEND_MAX * ID_MAX]; // 最多50个好友 以,分割
};
struct MSG_REGISTER // 注册消息
{
	int  nType; // REGISTER
	char nID[ID_MAX]; // ID 只用于返回
	char PassWord[KEY_MAX]; // 密码
	char Name[NAME_MAX]; // 昵称	
	char Email[EMAIL_MAX]; // 邮件
	char Sex; // 性别
	int  nAge; // 年龄

};
struct MSG_SYS // 系统消息
{
	int nType;
	char nID[ID_MAX];
	UINT nIDPrompt;
};
struct MSG_T // 通用数据结构体 用于接收
{
	int  nType; // 
	char Data[4092]; // 
};
/*
struct MSG_ADDFRIEND
{
	int nType;
	char ID1[ID_MAX];
	char ID2[ID_MAX];
	int nReturn; // 返回给客户端消息
};
*/
struct MSG_FRND_INFO
{
	int nType;
	int nNum; // 朋友数目
	int nStatus[FRIEND_MAX];
	char ListID[FRIEND_MAX][ID_MAX];
	char ListName[FRIEND_MAX][NAME_MAX];
};

