#pragma once

#include "SocketServer.h"

struct USERINFO{
	char nID[ID_MAX]; // 账号 最多12位 纯数字 最后一位是‘\0’
	char PassWord[KEY_MAX]; // 密码 最多16位 数字、字母、标点组合
	char Email[EMAIL_MAX]; // 邮件 最多32位 ANSI编码 无中文 最后一位是‘\0’
	char Name[NAME_MAX]; // 昵称 最多24个汉字或49个字母
	char Sex; // 性别 一位 m男性 f女性
	int  nAge; // 年龄
	int  nStatus; // 状态
	char FriendList[FRIEND_MAX * ID_MAX]; // 最多50个好友 以|分割
	char RegDate[15]; // 注册时间xxxx-xx-xx xx:xx:xx 14位 不含分隔符 再加'\0' 共计15位
};

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

	
	// 保存数据 
	BOOL SaveData(CString csFilePath = L"")const;
	// 加载数据
	BOOL LoadData(CString csFilePath = L"");

	void ShowData(CListCtrl * pListCtrl)const;



protected:

	BOOL IsExist(struct USERINFO **pUserInfo, const char * nID)const; // 检查该ID是否存在	成功返回指向该ID的指针

protected:

	CList<struct USERINFO*, struct USERINFO*> m_userlist;

	//CList<struct friendlist*, struct friendlist*> m_friendlist;

	int m_MaxID;

public:
	int GetAllFriendInfo(struct MSG_FRND_INFO* msg_info, char* nID);
	int SetAllUserStatus(int nStatus);
};



