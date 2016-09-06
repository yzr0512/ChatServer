
/************************************************
�� �� ����Data.h
��    �ߣ���־��
�������ڣ�2016-08-03
��    ;��CData���ͷ�ļ�
�޸ļ�¼��2016-08-03 ��־�� ����
************************************************/ 

#pragma once

#include "SocketServer.h"

typedef struct USERINFO{
	char nID[ID_MAX];			// �˺� ���12λ ������ ���һλ�ǡ�\0��
	char PassWord[KEY_MAX];		// ���� ���16λ ���֡���ĸ��������
	char Email[EMAIL_MAX];		// �ʼ� ���32λ ANSI���� ������ ���һλ�ǡ�\0��
	char Name[NAME_MAX];		// �ǳ� ���24�����ֻ�49����ĸ
	char Sex;					// �Ա� һλ m���� fŮ��
	int  nAge;					// ���� 0-150
	int  nStatus;				// ״̬
	char FriendList[FRIEND_MAX * ID_MAX];// ���50������ ��|�ָ�
	char RegDate[15];			// ע��ʱ�� ������ʱ���� 14λ �ټ�'\0' ����15λ
}USER_INFO;


/************************************************
Class name: CData
Create by����־��
CreateDate��2016-08-03
Explain�������������¼ÿ���û�����Ϣ
Change by: 2016-08-03 by ��־�� ��������
************************************************/ 
class CData
{
public:
	CData();
	~CData();
	// ��ȡ����	
	int IsCorrect(const char * nID, const char * password)const; // ����������˺��Ƿ��Ӧ
	int GetUserInfo(struct USERINFO &user, const char * nID)const; 	// ���ĳ��ID����Ϣ
	int GetUserInfo(struct MSG_USERINFO &user, const char * nID)const; 	// ���ĳ��ID��ȫ����Ϣ
	int GetUserPublicInfo(struct MSG_USERINFO &user, const char * nID)const; // ���ĳ��ID�Ĺ�����Ϣ
	int GetUserStatus(const char * nID)const; // ��ø�ID��״̬ 
	int GetUserFriendList(char * friendlist, const char * nID)const; // ��ø�ID�ĺ����б�

	// �޸�����	
	BOOL AddUser(char* nID, char* password, char* email = NULL, char* name = NULL,
		char sex = 'n',	int age = 0, int Flag = STATUS_OFFLINE); // ����û� 
	BOOL AddUser(char* nID, struct MSG_REGISTER* msg_reg); // ����û� 	
	int DeleteUser(const char* nID); // ɾ���û�
	int SetUserStatus(const char * nID, int flag); // ���ø�ID��״̬	
	int MakeFriend(const char * nID1, const char * nID2); // ID1��ID2�������ѹ�ϵ
	int BreakFriend(const char * nID1, const char * nID2); // ID1��ID2�Ͼ����ѹ�ϵ
	int GetAllFriendInfo(struct MSG_FRND_INFO* msg_info, char* nID); // ���nID���к��ѵ�ID�����֡�״̬

	// �������� 
	BOOL SaveData(CString csFilePath = L"")const;
	// ��������
	BOOL LoadData(CString csFilePath = L"");

	//��ʱ����
	void ShowData(CListCtrl * pListCtrl)const; // չʾ���� DataDlg��
	int SetAllUserStatus(int nStatus); // ���������û���״̬

protected:

	BOOL IsExist(struct USERINFO **pUserInfo, const char * nID)const; // ����ID�Ƿ����	�ɹ�����ָ���ID��ָ��

	CList<struct USERINFO*, struct USERINFO*> m_userlist;

	int m_MaxID; // ��ǰ����ID

};



