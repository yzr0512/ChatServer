
// SocketServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CSocketServerApp:
// �йش����ʵ�֣������ SocketServer.cpp
//

class CSocketServerApp : public CWinApp
{
public:
	CSocketServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSocketServerApp theApp;

#define ID_MAX                          13
#define KEY_MAX                         17
#define NAME_MAX						50
#define EMAIL_MAX						33
#define FRIEND_MAX						50
#define MSG_SIZE_MAX					3358


// ע��
#define REGISTER                        5000
#define REGISTER_SUCCESS                5001
#define REGISTER_FAILED                 5002

// ��¼
#define LOGIN                           5010
#define LOGIN_SUCCESS                   5011
//#define LOGIN_FAILED                    5012
#define LOGIN_FORCE                     5013
#define LOGIN_OUT						5014

// ������Ϣ δ�õ�
#define RESET_PASSWORD                  5020
#define RESET_PASSWORD_SUCCESS          5021
#define RESET_PASSWORD_FAILED           5022
#define RESET_USERNAME                  5023
#define RESET_USERNAME_SUCCESS          5024
#define RESET_USERNAME_FAILED           5025

// ������Ϣ
#define CHATING_TEXT_MSG                5030

// ��ȡ������Ϣ
#define GET_FRIEND_INFO                 5040	// ����
#define GET_ALL_FRIEND_INFO             5041	// ȫ��
#define GET_FRIEND_LIST                 5042
#define GET_STRANGER_INFO               5043

// ��ӻ�ɾ������
#define ADD_FRIEND_REQUEST				5050
#define ADD_FRIEND_ANSWER				5051
#define ADD_FRIEND_AGREE				5055
#define ADD_FRIEND_REFUSE				5056
#define ADD_FRIEND_FAILED               5053
#define DELETE_FRIEND                   5054

// �˺�״̬
#define SET_USER_STATUS					5064
#define STATUS_ONLINE                   5060
#define STATUS_BUSY                     5061
#define STATUS_HIDE                     5062
#define STATUS_OFFLINE                  5063

//#define MESSAGE_SYSTEM					5070

//***������ͨ���õĽṹ��***
struct MSG_LOGIN // ��¼��Ϣ
{
	int  nType; // LOGIN
	char nID[ID_MAX]; // �˺����12λ
	char Key[KEY_MAX]; // �������16λ
	int nStatus;
};
struct MSG_TRANSPOND // ת����Ϣ
{
	int  nType; // CHATING_TEXT_MSG
	char ToID[ID_MAX]; // ���շ�
	char FromID[ID_MAX]; // ���ͷ�
	char Date[11]; // yyyy-mm-dd
	char Time[9]; // hh-mm-ss
	char Data[200]; // ������Ϣ
	int nReturn; // ������Ϣ
};
struct MSG_USERINFO // �û���Ϣ
{
	int  nType; // GET_FRIEND_LIST
	int  nStatus; // ����״̬
	char nID[ID_MAX]; // ID
	char Name[NAME_MAX]; // �ǳ�
	char Email[EMAIL_MAX]; // �ʼ� ���32λ ANSI���� ������ ���һλ�ǡ�\0��
	char Sex; // �Ա� һλ m���� fŮ��
	int  nAge; // ����
	char FriendList[FRIEND_MAX * ID_MAX]; // ���50������ ��,�ָ�
};
struct MSG_REGISTER // ע����Ϣ
{
	int  nType; // REGISTER
	char nID[ID_MAX]; // ID ֻ���ڷ���
	char PassWord[KEY_MAX]; // ����
	char Name[NAME_MAX]; // �ǳ�	
	char Email[EMAIL_MAX]; // �ʼ�
	char Sex; // �Ա�
	int  nAge; // ����

};
struct MSG_SYS // ϵͳ��Ϣ
{
	int nType;
	char nID[ID_MAX];
	UINT nIDPrompt;
};
struct MSG_T // ͨ�����ݽṹ�� ���ڽ���
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
	int nReturn; // ���ظ��ͻ�����Ϣ
};
*/
struct MSG_FRND_INFO
{
	int nType;
	int nNum; // ������Ŀ
	int nStatus[FRIEND_MAX];
	char ListID[FRIEND_MAX][ID_MAX];
	char ListName[FRIEND_MAX][NAME_MAX];
};

