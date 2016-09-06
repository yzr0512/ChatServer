
/************************************************
文 件 名：Data.h
作    者：余志荣
创建日期：2016-08-03
用    途：CData类的头文件
修改记录：2016-08-03 余志荣 创建
************************************************/ 

#pragma once

#include "SocketServer.h"

typedef struct USERINFO{
	char nID[ID_MAX];			// 账号 最多12位 纯数字 最后一位是‘\0’
	char PassWord[KEY_MAX];		// 密码 最多16位 数字、字母、标点组合
	char Email[EMAIL_MAX];		// 邮件 最多32位 ANSI编码 无中文 最后一位是‘\0’
	char Name[NAME_MAX];		// 昵称 最多24个汉字或49个字母
	char Sex;					// 性别 一位 m男性 f女性
	int  nAge;					// 年龄 0-150
	int  nStatus;				// 状态
	char FriendList[FRIEND_MAX * ID_MAX];// 最多50个好友 以|分割
	char RegDate[15];			// 注册时间 年月日时分秒 14位 再加'\0' 共计15位
}USER_INFO;


/************************************************
Class name: CData
Create by：余志荣
CreateDate：2016-08-03
Explain：这个类用来记录每个用户的信息
Change by: 2016-08-03 by 余志荣 创建此类
************************************************/ 
class CData
{
public:
	CData();
	~CData();
	// 读取数据	
	int IsCorrect(const char * nID, const char * password)const; // 检查密码与账号是否对应
	int GetUserInfo(struct USERINFO &user, const char * nID)const; 	// 获得某个ID的信息
	int GetUserInfo(struct MSG_USERINFO &user, const char * nID)const; 	// 获得某个ID的全部信息
	int GetUserPublicInfo(struct MSG_USERINFO &user, const char * nID)const; // 获得某个ID的公开信息
	int GetUserStatus(const char * nID)const; // 获得该ID的状态 
	int GetUserFriendList(char * friendlist, const char * nID)const; // 获得该ID的好友列表

	// 修改数据	
	BOOL AddUser(char* nID, char* password, char* email = NULL, char* name = NULL,
		char sex = 'n',	int age = 0, int Flag = STATUS_OFFLINE); // 添加用户 
	BOOL AddUser(char* nID, struct MSG_REGISTER* msg_reg); // 添加用户 	
	int DeleteUser(const char* nID); // 删除用户
	int SetUserStatus(const char * nID, int flag); // 设置该ID的状态	
	int MakeFriend(const char * nID1, const char * nID2); // ID1与ID2建立好友关系
	int BreakFriend(const char * nID1, const char * nID2); // ID1与ID2断绝好友关系
	int GetAllFriendInfo(struct MSG_FRND_INFO* msg_info, char* nID); // 获得nID所有好友的ID、名字、状态

	// 保存数据 
	BOOL SaveData(CString csFilePath = L"")const;
	// 加载数据
	BOOL LoadData(CString csFilePath = L"");

	//临时函数
	void ShowData(CListCtrl * pListCtrl)const; // 展示数据 DataDlg用
	int SetAllUserStatus(int nStatus); // 设置所有用户的状态

protected:

	BOOL IsExist(struct USERINFO **pUserInfo, const char * nID)const; // 检查该ID是否存在	成功返回指向该ID的指针

	CList<struct USERINFO*, struct USERINFO*> m_userlist;

	int m_MaxID; // 当前最大的ID

};



