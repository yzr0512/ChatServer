#include "stdafx.h"
#include "Data.h"
#include "resource.h"

CData::CData()
{
	m_MaxID = 10000;
}
CData::~CData()
{
	POSITION pos = m_userlist.GetHeadPosition();
	struct USERINFO* pUserInfo;
	while(pos != NULL)
	{
		pUserInfo = m_userlist.GetNext(pos);
		delete pUserInfo;
	}
}


/*********************************************************
�������ƣ�IsExist
�������������nID�Ƿ���� ������ָ��
����˵����pUserInfo-��Ϊ��Ҫ�޸�ָ���ָ�� ����Ҫ˫��ָ��
		nID-�������û��˺�
�� �� ֵ�����ؼٵ�ʱ��pUserInfo = NULL��
*********************************************************/
BOOL CData::IsExist(struct USERINFO **pUserInfo, const char* nID)const
{
	POSITION pos = m_userlist.GetHeadPosition();
	while(pos != NULL)
	{
		*pUserInfo = m_userlist.GetNext(pos);
		if(!strcmp((*pUserInfo)->nID, nID))
		{
			return TRUE;
		}
	}
	*pUserInfo = NULL;
	return FALSE;
}


/*********************************************************
�������ƣ�IsCorrect
��������������˺ź������Ƿ�ƥ��
����˵����nID-�û��˺� password-�û�����
�� �� ֵ��TRUE-������ȷ
          IDS_ERR_PASSWD_INCORRECT-�������
		  IDS_ERR_USER_NOT_EXIST-�û�������
*********************************************************/
int CData::IsCorrect(const char * nID, const char * password)const // ����������˺��Ƿ��Ӧ
{
	struct USERINFO* pUserInfo = NULL;
	//IsExist(&pUserInfo, nID);
	if(!IsExist(&pUserInfo, nID))
	{// �û�������
		return IDS_ERR_USER_NOT_EXIST; 
	}
	
	if(!strcmp(pUserInfo->PassWord, password))
	{// ������ȷ
		return TRUE;			
	}
	else
	{// �������
		return IDS_ERR_PASSWD_INCORRECT;	
	}
}

		
/*********************************************************
�������ƣ�GetUserStatus
�������������nID��״̬��Ϣ 
����˵����nID-�˺�
�� �� ֵ��nStatus-״̬(ERR_USER_NOT_EXIT��ʾ�˺Ų�����)
*********************************************************/
int CData::GetUserStatus(const char* nID)const
{	
	struct USERINFO* pUserInfo = NULL;
	if(IsExist(&pUserInfo, nID))
	{
		return pUserInfo->nStatus;
	}
	return IDS_ERR_USER_NOT_EXIST;
}


/*********************************************************
�������ƣ�GetUserInfo
�������������ĳ��ID��ȫ����Ϣ 
����˵����user-�����û���Ϣ�Ľṹ�� nID-�˺�
�� �� ֵ��
*********************************************************/
int CData::GetUserInfo(struct USERINFO &user, const char * nID)const
{
	struct USERINFO *pInfo = NULL;
	if(!IsExist(&pInfo, nID))
	{// �˺Ų�����
		return IDS_ERR_USER_NOT_EXIST;

	}
	// ������Ϣ
	strcpy_s(user.nID, pInfo->nID);
	strcpy_s(user.Name, pInfo->Name);
	strcpy_s(user.Email, pInfo->Email);
	strcpy_s(user.FriendList, pInfo->FriendList);
	strcpy_s(user.RegDate, pInfo->RegDate);

	user.nAge = pInfo->nAge;
	user.Sex = pInfo->Sex;
	user.nStatus = pInfo->nStatus;
	return TRUE;
	
}
int CData::GetUserInfo(struct MSG_USERINFO &user, const char * nID)const
{
	struct USERINFO *pInfo = NULL;
	if(!IsExist(&pInfo, nID))
	{// �˺Ų�����
		return IDS_ERR_USER_NOT_EXIST;
	}	

	// ������Ϣ
	strcpy_s(user.nID, pInfo->nID);
	memcpy(user.Name, pInfo->Name, NAME_MAX);
	//strcpy_s(user.Name, pInfo->Name);
	strcpy_s(user.Email, pInfo->Email);
	strcpy_s(user.FriendList, pInfo->FriendList);

	user.nAge = pInfo->nAge;
	user.Sex = pInfo->Sex;
	
	if(pInfo->nStatus == STATUS_HIDE)
		user.nStatus = STATUS_OFFLINE;
	else
		user.nStatus = pInfo->nStatus;

	return TRUE;
}


/*********************************************************
�������ƣ�GetUserPublicInfo
�������������ĳ��ID�Ĺ�����Ϣ 
����˵����user-�����û���Ϣ�Ľṹ�� nID-�˺�
�� �� ֵ��
��    ע��������Ϣ�� ID���ǳơ����䡢״̬���Ա�
*********************************************************/
int CData::GetUserPublicInfo(struct MSG_USERINFO &user, const char * nID)const// ���ĳ��ID�Ĺ�����Ϣ
{
	struct USERINFO *pInfo = NULL;
	if(!IsExist(&pInfo, nID))
	{// �˺Ų�����
		return IDS_ERR_USER_NOT_EXIST;
	}	

	// ������Ϣ
	strcpy_s(user.nID, pInfo->nID);
	//strcpy_s(user.Name, pInfo->Name);
	memcpy(user.Name, pInfo->Name, NAME_MAX);
	strcpy_s(user.Email, pInfo->Email);
	memset(user.FriendList, 0, FRIEND_MAX * ID_MAX);

	user.nAge = -1;
	user.Sex = pInfo->Sex;
	if(pInfo->nStatus == STATUS_HIDE)
		user.nStatus = STATUS_OFFLINE;
	else
		user.nStatus = pInfo->nStatus;
	return TRUE;
}


/*********************************************************
�������ƣ�AddUser
��������������û�
����˵����nID-�˺�(����ֵ) password-���� email-���� name-�ǳ� sex-�Ա�
	age-���� nStatus-״̬
�� �� ֵ��TRUE-��ӳɹ�
��    ע��ͨ��nID����ע���ID��������������ID
*********************************************************/
BOOL CData::AddUser(char* nID, char* password, char* email, char* name,
		char sex, int age, int nStatus)
{
	struct USERINFO *pUserInfo = new struct USERINFO;
	// ���ø�����Ϣ
	strcpy_s(pUserInfo->PassWord, password); // password
	if(email) // email
		strcpy_s(pUserInfo->Email, email);
	if(name) // name
		strcpy_s(pUserInfo->Name, name);
	pUserInfo->Sex = sex; // sex
	pUserInfo->nAge = age; // age
	pUserInfo->nStatus = nStatus; // Flag
	sprintf_s(pUserInfo->nID, "%d", ++m_MaxID); // ID
	memset(pUserInfo->FriendList, '\0', sizeof(pUserInfo->FriendList));

	CTime tm = CTime::GetCurrentTime();	
	CString strDate = tm.Format("%Y%m%d%H%M%S");
	size_t i; // ��ת�����ַ���
	wcstombs_s(&i, pUserInfo->RegDate, strDate, strDate.GetLength() + 1); // ���ַ�ת���ַ�
	//wcstombs(pUserInfo->RegDate, strDate, strDate.GetLength() + 1); 

	// ��ָ�����������
	m_userlist.AddTail(pUserInfo);

	// ��ID����
	strcpy_s(nID, sizeof(pUserInfo->nID), pUserInfo->nID);

	return TRUE;
}
BOOL CData::AddUser(char* nID, struct MSG_REGISTER* msg_reg)
{
	struct USERINFO *pUserInfo = new struct USERINFO;

	strcpy_s(pUserInfo->PassWord, msg_reg->PassWord); // password
	if(msg_reg->Email) // email
		strcpy_s(pUserInfo->Email, msg_reg->Email);
	
	if(strlen(msg_reg->Name) != 0) // ���û���������������˺Ŵ���
		memcpy(pUserInfo->Name, msg_reg->Name, NAME_MAX);
	else	
		mbstowcs((TCHAR *)pUserInfo->Name, msg_reg->nID, strlen(msg_reg->nID)); // ���ַ�ת���ַ�

	pUserInfo->Sex = msg_reg->Sex; // sex
	pUserInfo->nAge = msg_reg->nAge; // age
	pUserInfo->nStatus = STATUS_OFFLINE; // Flag
	sprintf_s(pUserInfo->nID, "%d", ++m_MaxID); // ID
	memset(pUserInfo->FriendList, '\0', sizeof(pUserInfo->FriendList));

	CTime tm = CTime::GetCurrentTime();	
	CString strDate = tm.Format("%Y%m%d%H%M%S");		
	size_t i; // ��ת�����ַ���
	wcstombs_s(&i, pUserInfo->RegDate, strDate, strDate.GetLength() + 1); // ���ַ�ת���ַ�
	//wcstombs(pUserInfo->RegDate, strDate, strDate.GetLength() + 1);

	// ��ָ�����������
	m_userlist.AddTail(pUserInfo);

	strcpy_s(nID, sizeof(pUserInfo->nID), pUserInfo->nID);

	return TRUE;
}


/*********************************************************
�������ƣ�DeleteUser
����������ɾ���û�
����˵����nID-�˺�
�� �� ֵ���ɹ�����TRUE
��    ע��
*********************************************************/
int CData::DeleteUser(const char* nID)
{
	POSITION pos = m_userlist.GetHeadPosition();
	struct USERINFO* pUserInfo;
	while(pos != NULL)
	{
		pUserInfo = m_userlist.GetNext(pos);
		if(!strcmp(pUserInfo->nID, nID))
		{
			delete pUserInfo;
			m_userlist.GetPrev(pos); // ��Ϊpos�Ѿ�ָ������һ��Ԫ�� ��pos�ָ�
			m_userlist.RemoveAt(pos); 
			return TRUE;
		}
	}
	return IDS_ERR_USER_NOT_EXIST;
}


/*********************************************************
�������ƣ�GetUserFriendList
��������������û������б�
����˵����friendlist-�����б��ַ��� nID-�˺�
�� �� ֵ���ɹ�����TRUE
��    ע�������б���","�ָ�
*********************************************************/
int CData::GetUserFriendList(char * friendlist, const char * nID)const
{
	struct USERINFO* pUserInfo = NULL;
	if(!IsExist(&pUserInfo, nID))
	{// �˺Ų�����
		return IDS_ERR_USER_NOT_EXIST;
	}
	// ������Ϣ
	strcpy_s(friendlist, sizeof(pUserInfo->FriendList), pUserInfo->FriendList);
	
	return TRUE;
}


/*********************************************************
�������ƣ�SetUserStatus
�������������ø�ID��״̬
����˵����nID-�˺� nStatus-Ҫ���óɵ�״̬
�� �� ֵ���ɹ�����TRUE
��    ע�������б���","�ָ�
*********************************************************/
int CData::SetUserStatus(const char * nID, int nStatus)
{
	POSITION pos = m_userlist.GetHeadPosition();
	struct USERINFO* pUserInfo = NULL;
	if(!IsExist(&pUserInfo, nID))
	{// �˺Ų�����
		return IDS_ERR_USER_NOT_EXIST;
	}

	pUserInfo->nStatus = nStatus;

	return TRUE;
}


/*********************************************************
�������ƣ�MakeFriend
����������������nID�������ѹ�ϵ
����˵����nID1-�˺�1  nID2-�˺�2
����ʱ�䣺2016-08-03
�� �� ֵ���ɹ�����TRUE
*********************************************************/
int CData::MakeFriend(const char * nID1, const char * nID2)
{
		// 2016-08-04�޸�
		// �޸�ԭ�򣺽���Ƕ�ײ���
		// ԭ���룺
	/*POSITION pos = m_userlist.GetHeadPosition();
	struct USERINFO* pInfo = NULL;
	IsExist(pInfo, nID1);
	while(pos != NULL)
	{
		pInfo = m_userlist.GetNext(pos);
		if(!strcmp(pInfo->nID, nID1))
		{// �ҵ���һ��ID
			char FriLst[FRIEND_MAX * ID_MAX];
			char *pToken;
			bool Flag = FALSE; // ��������Ƿ���ڴ˺���
			strcpy(FriLst, pInfo->FriendList);
			pToken = strtok(FriLst, ","); // �ָ�����б�
			while(pToken != NULL)
			{
				if(!strcmp(pToken, nID2))
				{//�Ѵ��ڴ˺���
					Flag = TRUE;
					return FALSE;
				}
				pToken = strtok(NULL, ",");
			}
			
			// ���ҵڶ���ID
			POSITION pos2 = m_userlist.GetHeadPosition();
			struct USERINFO* pInfo2;
			while(pos2 != NULL)
			{
				pInfo2 = m_userlist.GetNext(pos2);
				if(!strcmp(pInfo2->nID, nID2))
				{// �ҵ��ڶ���IDҲ����
					strcat_s(pInfo->FriendList, ","); // ��ӷָ���
					strcat_s(pInfo->FriendList, nID2); // ���ID

					strcat_s(pInfo2->FriendList, ","); // ��ӷָ���
					strcat_s(pInfo2->FriendList, nID1); // ���ID
					return 1;
				}
			}
			break;
		}

	}
	return FALSE;*/
	
	
	struct USERINFO* pInfo = NULL;
	struct USERINFO* pInfo2 = NULL;
	if(!IsExist(&pInfo, nID1))
	{// �˺�һ������
		return IDS_ERR_USER_NOT_EXIST;
	}
	if(!IsExist(&pInfo2, nID2))
	{// �˺Ŷ�������
		return IDS_ERR_USER_NOT_EXIST;	
	}

	if(!strcmp(nID1, nID2))
	{// �����˺���ͬ
		return IDS_ERR_FRIEND_ADD_SELF;	
	}

	// ��������Ƿ����Ǻ��ѹ�ϵ
	char FriLst[FRIEND_MAX * ID_MAX];
	strcpy_s(FriLst, pInfo->FriendList); // ���ƺ����б�		
	bool Flag = FALSE; // ��������Ƿ���ڴ˺���
	char *pToken = NULL; // �ָ�����б�
	char *pTokenNext = NULL;
	pToken = strtok_s(FriLst, ",", &pTokenNext); 
	while(pToken != NULL)
	{
		if(!strcmp(pToken, nID2))
		{//�Ѵ��ڴ˺���
			Flag = TRUE;
			return IDS_ERR_FRIEND_HAD_EXIST;
		}
		//pToken = strtok(NULL, ",");
		pToken = strtok_s(NULL, ",", &pTokenNext); // ��ȫ�汾
	}						
	
	// ���˲��Ǻ��ѹ�ϵ����˫���б�ĩβ����
	if(strlen(pInfo->FriendList) == 0)
	{// �׸����� ֱ�����
		strcat_s(pInfo->FriendList, nID2); // ���ID
	}
	else
	{// ���׸����� ��Ҫ���Ϸָ���
		strcat_s(pInfo->FriendList, ","); // ��ӷָ���
		strcat_s(pInfo->FriendList, nID2); // ���ID
	}
	
	if(strlen(pInfo2->FriendList) == 0)
	{// �׸����� ֱ�����
		strcat_s(pInfo2->FriendList, nID1); // ���ID
	}
	else
	{// ���׸����� ��Ҫ���Ϸָ���
		strcat_s(pInfo2->FriendList, ","); // ��ӷָ���
		strcat_s(pInfo2->FriendList, nID1); // ���ID
	}

	return TRUE;
}


/*********************************************************
�������ƣ�BreakFriend
����������������nIDɾ�����ѹ�ϵ
����˵����nID1-�˺�1  nID2-�˺�2
����ʱ�䣺2016-08-03
�� �� ֵ���ɹ�����TRUE
*********************************************************/
int CData::BreakFriend(const char * nID1, const char * nID2)
{
		// 2016-08-04�޸�
		// ԭ����	
	/*
	POSITION pos = m_userlist.GetHeadPosition();
	struct USERINFO* pInfo;
	while(pos != NULL)
	{
		pInfo = m_userlist.GetNext(pos);
		if(!strcmp(pInfo->nID, nID1))
		{
			char FriLst[FRIEND_MAX * ID_MAX];
			char *pToken;
			bool Flag = FALSE; // ��������Ƿ���ڴ˺���
			strcpy(FriLst, pInfo->FriendList);
			pToken = strtok(FriLst, ","); // �ָ�����б�
			
			// ��պ����б�
			memset(pInfo->FriendList, 0, FRIEND_MAX * ID_MAX * sizeof(char));

			while(pToken != NULL)
			{
				if(strcmp(pToken, nID2))
				{//���pToken����ID2�ͽ��临�Ƶ�Friendlist
					if(strlen(pInfo->FriendList) == 0)
					{//	��һ������
						strcat_s(pInfo->FriendList, pToken); // ���ID						
					}
					else
					{// �ǵ�һ������
						strcat_s(pInfo->FriendList, ","); // ��ӷָ���
						strcat_s(pInfo->FriendList, pToken); // ���ID
					}
				}
				else
				{
					Flag = TRUE; // ���ڴ˺��� ������Ҳ��Ҫ����ִ��
				}
				pToken = strtok(NULL, ","); // �����ָ�ʣ��ĺ���ID
			}
			
			if(!Flag) // �����ڴ˺��� ���治��Ҫ����ִ��
				return FALSE;

			// ���ҵڶ���ID
			POSITION pos2 = m_userlist.GetHeadPosition();
			struct USERINFO* pInfo2;			
			while(pos2 != NULL)
			{
				pInfo2 = m_userlist.GetNext(pos2);
				if(!strcmp(pInfo2->nID, nID2))
				{// �ҵ��ڶ���IDҲ����
					strcpy(FriLst, pInfo2->FriendList); // ����ԭ�����б�
					pToken = strtok(FriLst, ","); // �ָ�����б�
					// ��պ����б�
					memset(pInfo2->FriendList, 0, FRIEND_MAX * ID_MAX * sizeof(char));
					while(pToken != NULL)
					{
						if(strcmp(pToken, nID1))
						{//���pToken����ID1�ͽ��临�Ƶ�Friendlist
							if(strlen(pInfo2->FriendList) == 0)
							{//	��һ������
								strcat_s(pInfo2->FriendList, pToken); // ���ID						
							}
							else
							{// �ǵ�һ������
								strcat_s(pInfo2->FriendList, ","); // ��ӷָ���
								strcat_s(pInfo2->FriendList, pToken); // ���ID
							}
						}
						pToken = strtok(NULL, ","); // �����ָ�ʣ��ĺ���ID
					}
				}
			}
			// ˫����ɾ�����
			return TRUE;
		}
	}
	return FALSE;
	*/	
	struct USERINFO* pInfo = NULL;
	struct USERINFO* pInfo2 = NULL;
	if(!IsExist(&pInfo, nID1))
	{// �˺�һ������
		return IDS_ERR_USER_NOT_EXIST;
	}
	if(!IsExist(&pInfo2, nID2))
	{// �˺Ŷ�������
		return IDS_ERR_USER_NOT_EXIST;
	}
	char FriLst[FRIEND_MAX * ID_MAX];
	strcpy_s(FriLst, pInfo->FriendList); // ����ԭ�����б�
	
	memset(pInfo->FriendList, 0, FRIEND_MAX * ID_MAX * sizeof(char)); // ���ԭ�����б�
	
	bool Flag = FALSE; // ��������Ƿ���ڴ˺���

	char *pToken = NULL;// �ָ�����б�
	char *pTokenNext = NULL;
	pToken = strtok_s(FriLst, ",", &pTokenNext); 
	while(pToken != NULL)
	{
		if(strcmp(pToken, nID2))
		{//���pToken����ID2�ͽ��临�Ƶ�Friendlist
			if(strlen(pInfo->FriendList) == 0)
			{//	��һ������
				strcat_s(pInfo->FriendList, pToken); // ���ID						
			}
			else
			{// �ǵ�һ������
				strcat_s(pInfo->FriendList, ","); // ��ӷָ���
				strcat_s(pInfo->FriendList, pToken); // ���ID
			}
		}
		else
		{
			Flag = TRUE; // ���ڴ˺��� ������Ҳ��Ҫ����ִ��
		}
		//pToken = strtok(NULL, ","); // �����ָ�ʣ��ĺ���ID
		pToken = strtok_s(NULL, ",", &pTokenNext);
	}

	if(!Flag) // �����ڴ˺��� ���治��Ҫ����ִ��
		return IDS_ERR_FRIEND_NOT_EXIST;

	strcpy_s(FriLst, pInfo2->FriendList); // ����ԭ�����б�
	memset(pInfo2->FriendList, 0, FRIEND_MAX * ID_MAX * sizeof(char)); // ��պ����б�	
	pTokenNext = NULL;
	pToken = strtok_s(FriLst, ",", &pTokenNext); // �ָ�����б�			
	while(pToken != NULL)
	{
		if(strcmp(pToken, nID1))
		{//���pToken����ID1�ͽ��临�Ƶ�Friendlist
			if(strlen(pInfo2->FriendList) == 0)
			{//	��һ������
				strcat_s(pInfo2->FriendList, pToken); // ���ID						
			}
			else
			{// �ǵ�һ������
				strcat_s(pInfo2->FriendList, ","); // ��ӷָ���
				strcat_s(pInfo2->FriendList, pToken); // ���ID
			}
		}
		//pToken = strtok(NULL, ","); // �����ָ�ʣ��ĺ���ID
		pToken = strtok_s(NULL, ",", &pTokenNext);
	}
	return TRUE; // ˫����ɾ�����
}


// �������� 
//   ������Ϊ����������Ŀ¼�ڱ���
//   ����ɹ�������
BOOL CData::SaveData(CString csFilePath)const
{

	return TRUE;
}

// ��������
// 
BOOL LoadData(CString csFilePath = L"")
{

	return FALSE;
}

void CData::ShowData(CListCtrl * pListCtrl)const
{
	// ɾ��������
	pListCtrl->DeleteAllItems();
	// ɾ��������
	CHeaderCtrl* pHeaderCtrl = pListCtrl->GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
	{
		int  nColumnCount = pHeaderCtrl->GetItemCount();
		for (int i=0; i<nColumnCount; i++)
		{
			pListCtrl->DeleteColumn(0);
		}
	}

	// ��m_lstctlData��ӱ�ͷ
	CRect rect;
	pListCtrl->GetClientRect(&rect);
	int nColInterval = rect.Width();
	pListCtrl->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	pListCtrl->InsertColumn(0, _T("ID"), LVCFMT_CENTER,			int(nColInterval / 7));   
    pListCtrl->InsertColumn(1, _T("Name"), LVCFMT_CENTER,		int(nColInterval / 7));
	pListCtrl->InsertColumn(2, _T("Email"), LVCFMT_CENTER,		int(nColInterval / 7));   
	pListCtrl->InsertColumn(3, _T("Sex"), LVCFMT_CENTER,		int(nColInterval / 7));   
	pListCtrl->InsertColumn(4, _T("Age"), LVCFMT_CENTER,		int(nColInterval / 7));    
	pListCtrl->InsertColumn(5, _T("Status"), LVCFMT_CENTER,		int(nColInterval / 7));   
	pListCtrl->InsertColumn(6, _T("RegDate"), LVCFMT_CENTER,	int(nColInterval / 7));
	//pListCtrl->InsertColumn(7, _T("PassWord"), LVCFMT_CENTER,	int(nColInterval / 7));
	// �������
	int i = 0;
	POSITION pos = m_userlist.GetHeadPosition();
	struct USERINFO *puif = NULL;
	while(pos != NULL)
	{
		puif = m_userlist.GetNext(pos);
		CString strTemp; // ����ת�����ַ�

		//strTemp.Format("%s", puif->nID); // nID�Ƕ��ַ�
		strTemp = puif->nID;
		pListCtrl->InsertItem(i, strTemp);
		strTemp = (LPCTSTR)puif->Name; // �ǳ����Կ��ַ�������
		pListCtrl->SetItemText(i, 1, strTemp);
		strTemp = puif->Email; // Email�Ƕ��ַ�
		pListCtrl->SetItemText(i, 2, strTemp); 
		strTemp = puif->Sex; // Sex�ǵ��ַ�
		pListCtrl->SetItemText(i, 3, strTemp); 
		strTemp.Format(L"%d", puif->nAge); // ����������
		pListCtrl->SetItemText(i, 4, strTemp); 
		strTemp.Format(L"%d", puif->nStatus); // ״̬������
		pListCtrl->SetItemText(i, 5, strTemp); 
		strTemp = puif->RegDate; // ע�������Ƕ��ַ�
		pListCtrl->SetItemText(i, 6, strTemp);
		//strTemp = puif->PassWord; // �����Ƕ��ַ�
		//pListCtrl->SetItemText(i, 7, strTemp);
		++i;	
	}

}
