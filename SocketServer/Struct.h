
/************************************************
�� �� ����Struct.h
��    �ߣ���־��
�������ڣ�2016-08-29
��    ;����ͷ�ļ����������õ��Ĵ󲿷ֽṹ���Լ���������
�޸ļ�¼��2016-08-29 ��־�� ����
************************************************/ 

#pragma once

// �����ַ��������޴�С
#define ID_MAX                          13
#define KEY_MAX                         17
#define NAME_MAX						50
#define EMAIL_MAX						33
#define FRIEND_MAX						50
#define MSG_SIZE_MAX					8300


// ������Ϣ δ�õ�
#define RESET_PASSWORD                  5020
#define RESET_PASSWORD_SUCCESS          5021
#define RESET_PASSWORD_FAILED           5022
#define RESET_USERNAME                  5023
#define RESET_USERNAME_SUCCESS          5024
#define RESET_USERNAME_FAILED           5025


// ��ȡ������Ϣ
#define GET_FRIEND_INFO                 5040	// ����
#define GET_ALL_FRIEND_INFO             5041	// ȫ��
//#define UPDATE_FRIEND_INFO_BEGIN		5044
//#define UPDATE_FRIEND_INFO_FINISH		5045
#define GET_FRIEND_LIST                 5042
#define GET_STRANGER_INFO               5043

// ��ӻ�ɾ������


// �˺�״̬
#define SET_USER_STATUS					5064
#define STATUS_ONLINE                   5060
#define STATUS_BUSY                     5061
#define STATUS_HIDE                     5062
#define STATUS_OFFLINE                  5063

#define HEARTBEAT						5070

//#define MESSAGE_SYSTEM					5070

//***������ͨ���õĽṹ��***

// ��¼��Ϣ�ṹ��
#define LOGIN                           5010
#define LOGIN_SUCCESS                   5011
//#define LOGIN_FAILED                    5012
//#define LOGIN_FORCE                     5013
#define LOGIN_OUT                       5014

struct MSG_LOGIN
{
	int  nType; // LOGIN
	char nID[ID_MAX]; // �˺����12λ
	char Key[KEY_MAX]; // �������16λ
	int  nStatus;
};

// ����
#define CHATING_TEXT_MSG                5030
// ת����Ϣ�ṹ��
struct MSG_TRANSPOND
{
	int  nType; // CHATING_TEXT_MSG
	char ToID[ID_MAX]; // ���շ�
	char FromID[ID_MAX]; // ���ͷ�
	char Date[11]; // yyyy-mm-dd
	char Time[9]; // hh-mm-ss
	char Data[200]; // ������Ϣ
	int nReturn; // ������Ϣ
};


// ��ӻ�ɾ������
#define ADD_FRIEND_REQUEST				5050
#define ADD_FRIEND_ANSWER				5051
#define ADD_FRIEND_AGREE				5055
#define ADD_FRIEND_REFUSE				5056
#define ADD_FRIEND_FAILED               5053
#define DELETE_FRIEND                   5054
struct MSG_MAKE_FRIEND
{
	int  nType; // CHATING_TEXT_MSG
	char ToID[ID_MAX]; // ���շ�
	char FromID[ID_MAX]; // ���ͷ�
	char Date[11]; // yyyy-mm-dd
	char Time[9]; // hh-mm-ss
	char Data[200]; // ������Ϣ
	int nReturn; // ������Ϣ
};


// ������Ϣ

typedef struct MSG_USERINFO
{
	int  nType; // GET_FRIEND_LIST
	int  nStatus; // ����״̬
	char nID[ID_MAX]; // ID
	char Name[NAME_MAX]; // �ǳ�
	char Email[EMAIL_MAX]; // �ʼ� ���32λ ANSI���� ������ ���һλ�ǡ�\0��
	char Sex; // �Ա� һλ m���� fŮ��
	int  nAge; // ����
	char FriendList[FRIEND_MAX * ID_MAX]; // ���50������ ��|�ָ�
}MSG_USERINFO;

// ע��
#define REGISTER                        5000
#define REGISTER_SUCCESS                5001
#define REGISTER_FAILED                 5002
struct MSG_REGISTER
{
	int  nType; // REGISTER
	char nID[ID_MAX]; // ID ֻ���ڷ���
	char PassWord[KEY_MAX]; // ����
	char Name[NAME_MAX]; // �ǳ�	
	char Email[EMAIL_MAX]; // �ʼ�
	char Sex; // �Ա�
	int  nAge; // ����

};


// ϵͳ��Ϣ
#define FILE_TRANS_SOCKET 6090 // ���߷��������socket���������ļ���
struct MSG_SYS
{
	int nType;
	char nID[ID_MAX]; // ������
	UINT nIDPrompt; // �ַ����������ID
};


// ͨ�����ݽṹ�� ���ڽ���
struct MSG_T
{
	int  nType;
	char Data[MSG_SIZE_MAX - 4];
};

/*
#define MESSAGE_HEAD  1000
typedef struct MSG_HEAD
{
	int nType; // 
	int nMsgSize; // 
}MSG_HEAD;
*/

struct MSG_ADDFRIEND
{
	int nType;
	char ID1[ID_MAX];
	char ID2[ID_MAX];
	int nReturn; // ���ظ��ͻ�����Ϣ
};

typedef struct MSG_FRND_INFO
{
	int nType;
	int nNum; // ������Ŀ
	int nStatus[FRIEND_MAX];
	char ListID[FRIEND_MAX][ID_MAX];
	char ListName[FRIEND_MAX][NAME_MAX];
}MSG_FRND_INFO;



//�ļ�����ģ��
// ��������
#define MESSAGE_FILE_REQUEST		6000
#define MESSAGE_FILE_AGREE			6010
#define MESSAGE_FILE_REFUSE			6020
#define MESSAGE_FILE_ERROR			6021 // �Ǿܾ�ԭ������Ĵ�������ʧ��

typedef struct MSG_FILE_REQUEST
{
	int  nType;
	char ToID[ID_MAX];
	char FromID[ID_MAX];
	char FileName[512]; // �ļ�·��+�ļ���
	ULONGLONG nFileSize; // �ļ���С
	int  nFileID; // �����ļ���̫�� ����������������
}MSG_FILE_REQUEST;

// �������
#define MESSAGE_FILE_TRANS_BEGIN	6030
#define MESSAGE_FILE_TRANS_END		6031 // �����з������ر�����
#define MESSAGE_FILE_TRANS_PAUSE	6032	// ��ͣ���� δʵ��
typedef struct MSG_FILE_OPERA
{
	int nType;
	char ToID[ID_MAX];
	char FromID[ID_MAX];

}MSG_FILE_OPERA;

// �ļ����ݰ�
#define MESSAGE_FILE_CONTENT	6040
#define FILE_PACKET_SIZE		8192
typedef struct MSG_FILE_CONTENT
{
	int  nType;
	char ToID[ID_MAX];	// ������ID
	char FromID[ID_MAX]; // ������ID
	int  nFileID; // �����ļ���̫�� ����������������
	int  nPacketID; // ���ݰ����
	char FileData[FILE_PACKET_SIZE]; // �ļ�����
	int nPacketSize; // �����е�����
}MSG_FILE_CONTENT;

// ���ؼ�¼
typedef struct FILE_RECORD
{
	bool bType; // true -- ����  false -- ����
	char FromID[ID_MAX]; // ������
	char ToID[ID_MAX]; // ������
	char FilePath[512];	// �ļ�·��
	char FileName[512]; // �ļ���
	int  nFileID; // �����ļ���̫�� ����������������
	int  nTransState; // ����״̬ 0-�ȴ���Ӧ 1-������ 2-��ͣ 3-����� 4-ȡ�� 5-���� 6-�ܾ�
	ULONGLONG nFileSize; // �ļ��ܴ�С
	ULONGLONG nRecvSize; // ���յĴ�С
	int  nRecvPacket; // �ļ�������� �����Ѿ����ܵ�������Ŀ
}FILE_RECORD;

