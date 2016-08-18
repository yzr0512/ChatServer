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
函数名称：IsExist
功能描述：检查nID是否存在 并返回指针
参数说明：pUserInfo-因为需要修改指针的指向 所以要双重指针
		nID-被检查的用户账号
返 回 值：返回假的时候pUserInfo = NULL，
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
函数名称：IsCorrect
功能描述：检查账号和密码是否匹配
参数说明：nID-用户账号 password-用户密码
返 回 值：TRUE-密码正确
          IDS_ERR_PASSWD_INCORRECT-密码错误
		  IDS_ERR_USER_NOT_EXIST-用户不存在
*********************************************************/
int CData::IsCorrect(const char * nID, const char * password)const // 检查密码与账号是否对应
{
	struct USERINFO* pUserInfo = NULL;
	//IsExist(&pUserInfo, nID);
	if(!IsExist(&pUserInfo, nID))
	{// 用户不存在
		return IDS_ERR_USER_NOT_EXIST; 
	}
	
	if(!strcmp(pUserInfo->PassWord, password))
	{// 密码正确
		return TRUE;			
	}
	else
	{// 密码错误
		return IDS_ERR_PASSWD_INCORRECT;	
	}
}

		
/*********************************************************
函数名称：GetUserStatus
功能描述：获得nID的状态信息 
参数说明：nID-账号
返 回 值：nStatus-状态(ERR_USER_NOT_EXIT表示账号不存在)
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
函数名称：GetUserInfo
功能描述：获得某个ID的全部信息 
参数说明：user-储存用户信息的结构体 nID-账号
返 回 值：
*********************************************************/
int CData::GetUserInfo(struct USERINFO &user, const char * nID)const
{
	struct USERINFO *pInfo = NULL;
	if(!IsExist(&pInfo, nID))
	{// 账号不存在
		return IDS_ERR_USER_NOT_EXIST;

	}
	// 拷贝信息
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
	{// 账号不存在
		return IDS_ERR_USER_NOT_EXIST;
	}	

	// 拷贝信息
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
函数名称：GetUserPublicInfo
功能描述：获得某个ID的公开信息 
参数说明：user-储存用户信息的结构体 nID-账号
返 回 值：
备    注：公开信息有 ID、昵称、邮箱、状态、性别
*********************************************************/
int CData::GetUserPublicInfo(struct MSG_USERINFO &user, const char * nID)const// 获得某个ID的公开信息
{
	struct USERINFO *pInfo = NULL;
	if(!IsExist(&pInfo, nID))
	{// 账号不存在
		return IDS_ERR_USER_NOT_EXIST;
	}	

	// 拷贝信息
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
函数名称：AddUser
功能描述：添加用户
参数说明：nID-账号(返回值) password-密码 email-邮箱 name-昵称 sex-性别
	age-年龄 nStatus-状态
返 回 值：TRUE-添加成功
备    注：通过nID返回注册的ID，不能自由设置ID
*********************************************************/
BOOL CData::AddUser(char* nID, char* password, char* email, char* name,
		char sex, int age, int nStatus)
{
	struct USERINFO *pUserInfo = new struct USERINFO;
	// 设置各种信息
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
	size_t i; // 已转换的字符数
	wcstombs_s(&i, pUserInfo->RegDate, strDate, strDate.GetLength() + 1); // 宽字符转多字符
	//wcstombs(pUserInfo->RegDate, strDate, strDate.GetLength() + 1); 

	// 将指针加入链表中
	m_userlist.AddTail(pUserInfo);

	// 将ID返回
	strcpy_s(nID, sizeof(pUserInfo->nID), pUserInfo->nID);

	return TRUE;
}
BOOL CData::AddUser(char* nID, struct MSG_REGISTER* msg_reg)
{
	struct USERINFO *pUserInfo = new struct USERINFO;

	strcpy_s(pUserInfo->PassWord, msg_reg->PassWord); // password
	if(msg_reg->Email) // email
		strcpy_s(pUserInfo->Email, msg_reg->Email);
	
	if(strlen(msg_reg->Name) != 0) // 如果没有设置名字则用账号代替
		memcpy(pUserInfo->Name, msg_reg->Name, NAME_MAX);
	else	
		mbstowcs((TCHAR *)pUserInfo->Name, msg_reg->nID, strlen(msg_reg->nID)); // 多字符转宽字符

	pUserInfo->Sex = msg_reg->Sex; // sex
	pUserInfo->nAge = msg_reg->nAge; // age
	pUserInfo->nStatus = IDS_STATUS_OFFLINE; // Flag
	sprintf_s(pUserInfo->nID, "%d", ++m_MaxID); // ID
	memset(pUserInfo->FriendList, '\0', sizeof(pUserInfo->FriendList));

	CTime tm = CTime::GetCurrentTime();	
	CString strDate = tm.Format("%Y%m%d%H%M%S");		
	size_t i; // 已转换的字符数
	wcstombs_s(&i, pUserInfo->RegDate, strDate, strDate.GetLength() + 1); // 宽字符转多字符
	//wcstombs(pUserInfo->RegDate, strDate, strDate.GetLength() + 1);

	// 将指针加入链表中
	m_userlist.AddTail(pUserInfo);

	strcpy_s(nID, sizeof(pUserInfo->nID), pUserInfo->nID);

	return TRUE;
}


/*********************************************************
函数名称：DeleteUser
功能描述：删除用户
参数说明：nID-账号
返 回 值：成功返回TRUE
备    注：
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
			m_userlist.GetPrev(pos); // 因为pos已经指向了下一个元素 将pos恢复
			m_userlist.RemoveAt(pos); 
			return TRUE;
		}
	}
	return IDS_ERR_USER_NOT_EXIST;
}


/*********************************************************
函数名称：GetUserFriendList
功能描述：获得用户好友列表
参数说明：friendlist-好友列表字符串 nID-账号
返 回 值：成功返回TRUE
备    注：好友列表用","分割
*********************************************************/
int CData::GetUserFriendList(char * friendlist, const char * nID)const
{
	struct USERINFO* pUserInfo = NULL;
	if(!IsExist(&pUserInfo, nID))
	{// 账号不存在
		return IDS_ERR_USER_NOT_EXIST;
	}
	// 复制信息
	strcpy_s(friendlist, sizeof(pUserInfo->FriendList), pUserInfo->FriendList);
	
	return TRUE;
}


/*********************************************************
函数名称：SetUserStatus
功能描述：设置该ID的状态
参数说明：nID-账号 nStatus-要设置成的状态
返 回 值：成功返回TRUE
备    注：好友列表用","分割
*********************************************************/
int CData::SetUserStatus(const char * nID, int nStatus)
{
	POSITION pos = m_userlist.GetHeadPosition();
	struct USERINFO* pUserInfo = NULL;
	if(!IsExist(&pUserInfo, nID))
	{// 账号不存在
		return IDS_ERR_USER_NOT_EXIST;
	}

	pUserInfo->nStatus = nStatus;

	return TRUE;
}


/*********************************************************
函数名称：MakeFriend
功能描述：将两个nID建立好友关系
参数说明：nID1-账号1  nID2-账号2
创建时间：2016-08-03
返 回 值：成功返回TRUE
*********************************************************/
int CData::MakeFriend(const char * nID1, const char * nID2)
{
		// 2016-08-04修改
		// 修改原因：降低嵌套层数
		// 原代码：
	/*POSITION pos = m_userlist.GetHeadPosition();
	struct USERINFO* pInfo = NULL;
	IsExist(pInfo, nID1);
	while(pos != NULL)
	{
		pInfo = m_userlist.GetNext(pos);
		if(!strcmp(pInfo->nID, nID1))
		{// 找到第一个ID
			char FriLst[FRIEND_MAX * ID_MAX];
			char *pToken;
			bool Flag = FALSE; // 用来标记是否存在此好友
			strcpy(FriLst, pInfo->FriendList);
			pToken = strtok(FriLst, ","); // 分割好友列表
			while(pToken != NULL)
			{
				if(!strcmp(pToken, nID2))
				{//已存在此好友
					Flag = TRUE;
					return FALSE;
				}
				pToken = strtok(NULL, ",");
			}
			
			// 查找第二个ID
			POSITION pos2 = m_userlist.GetHeadPosition();
			struct USERINFO* pInfo2;
			while(pos2 != NULL)
			{
				pInfo2 = m_userlist.GetNext(pos2);
				if(!strcmp(pInfo2->nID, nID2))
				{// 找到第二个ID也存在
					strcat_s(pInfo->FriendList, ","); // 添加分隔符
					strcat_s(pInfo->FriendList, nID2); // 添加ID

					strcat_s(pInfo2->FriendList, ","); // 添加分隔符
					strcat_s(pInfo2->FriendList, nID1); // 添加ID
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
	{// 账号一不存在
		return IDS_ERR_USER_NOT_EXIST;
	}
	if(!IsExist(&pInfo2, nID2))
	{// 账号二不存在
		return IDS_ERR_USER_NOT_EXIST;	
	}

	if(!strcmp(nID1, nID2))
	{// 两个账号相同
		return IDS_ERR_FRIEND_ADD_SELF;	
	}

	// 检查两人是否已是好友关系
	char FriLst[FRIEND_MAX * ID_MAX];
	strcpy_s(FriLst, pInfo->FriendList); // 复制好友列表		
	bool Flag = FALSE; // 用来标记是否存在此好友
	char *pToken = NULL; // 分割好友列表
	char *pTokenNext = NULL;
	pToken = strtok_s(FriLst, ",", &pTokenNext); 
	while(pToken != NULL)
	{
		if(!strcmp(pToken, nID2))
		{//已存在此好友
			Flag = TRUE;
			return IDS_ERR_FRIEND_HAD_EXIST;
		}
		//pToken = strtok(NULL, ",");
		pToken = strtok_s(NULL, ",", &pTokenNext); // 安全版本
	}						
	
	// 两人不是好友关系就在双方列表末尾加上
	if(strlen(pInfo->FriendList) == 0)
	{// 首个好友 直接添加
		strcat_s(pInfo->FriendList, nID2); // 添加ID
	}
	else
	{// 非首个好友 需要接上分隔符
		strcat_s(pInfo->FriendList, ","); // 添加分隔符
		strcat_s(pInfo->FriendList, nID2); // 添加ID
	}
	
	if(strlen(pInfo2->FriendList) == 0)
	{// 首个好友 直接添加
		strcat_s(pInfo2->FriendList, nID1); // 添加ID
	}
	else
	{// 非首个好友 需要接上分隔符
		strcat_s(pInfo2->FriendList, ","); // 添加分隔符
		strcat_s(pInfo2->FriendList, nID1); // 添加ID
	}

	return TRUE;
}


/*********************************************************
函数名称：BreakFriend
功能描述：将两个nID删除好友关系
参数说明：nID1-账号1  nID2-账号2
创建时间：2016-08-03
返 回 值：成功返回TRUE
*********************************************************/
int CData::BreakFriend(const char * nID1, const char * nID2)
{
		// 2016-08-04修改
		// 原代码	
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
			bool Flag = FALSE; // 用来标记是否存在此好友
			strcpy(FriLst, pInfo->FriendList);
			pToken = strtok(FriLst, ","); // 分割好友列表
			
			// 清空好友列表
			memset(pInfo->FriendList, 0, FRIEND_MAX * ID_MAX * sizeof(char));

			while(pToken != NULL)
			{
				if(strcmp(pToken, nID2))
				{//如果pToken不是ID2就将其复制到Friendlist
					if(strlen(pInfo->FriendList) == 0)
					{//	第一个好友
						strcat_s(pInfo->FriendList, pToken); // 添加ID						
					}
					else
					{// 非第一个好友
						strcat_s(pInfo->FriendList, ","); // 添加分隔符
						strcat_s(pInfo->FriendList, pToken); // 添加ID
					}
				}
				else
				{
					Flag = TRUE; // 存在此好友 则下面也需要继续执行
				}
				pToken = strtok(NULL, ","); // 继续分割剩余的好友ID
			}
			
			if(!Flag) // 不存在此好友 下面不需要继续执行
				return FALSE;

			// 查找第二个ID
			POSITION pos2 = m_userlist.GetHeadPosition();
			struct USERINFO* pInfo2;			
			while(pos2 != NULL)
			{
				pInfo2 = m_userlist.GetNext(pos2);
				if(!strcmp(pInfo2->nID, nID2))
				{// 找到第二个ID也存在
					strcpy(FriLst, pInfo2->FriendList); // 拷贝原好友列表
					pToken = strtok(FriLst, ","); // 分割好友列表
					// 清空好友列表
					memset(pInfo2->FriendList, 0, FRIEND_MAX * ID_MAX * sizeof(char));
					while(pToken != NULL)
					{
						if(strcmp(pToken, nID1))
						{//如果pToken不是ID1就将其复制到Friendlist
							if(strlen(pInfo2->FriendList) == 0)
							{//	第一个好友
								strcat_s(pInfo2->FriendList, pToken); // 添加ID						
							}
							else
							{// 非第一个好友
								strcat_s(pInfo2->FriendList, ","); // 添加分隔符
								strcat_s(pInfo2->FriendList, pToken); // 添加ID
							}
						}
						pToken = strtok(NULL, ","); // 继续分割剩余的好友ID
					}
				}
			}
			// 双方都删除完成
			return TRUE;
		}
	}
	return FALSE;
	*/	
	struct USERINFO* pInfo = NULL;
	struct USERINFO* pInfo2 = NULL;
	if(!IsExist(&pInfo, nID1))
	{// 账号一不存在
		return IDS_ERR_USER_NOT_EXIST;
	}
	if(!IsExist(&pInfo2, nID2))
	{// 账号二不存在
		return IDS_ERR_USER_NOT_EXIST;
	}
	char FriLst[FRIEND_MAX * ID_MAX];
	strcpy_s(FriLst, pInfo->FriendList); // 复制原好友列表
	
	memset(pInfo->FriendList, 0, FRIEND_MAX * ID_MAX * sizeof(char)); // 清空原好友列表
	
	bool Flag = FALSE; // 用来标记是否存在此好友

	char *pToken = NULL;// 分割好友列表
	char *pTokenNext = NULL;
	pToken = strtok_s(FriLst, ",", &pTokenNext); 
	while(pToken != NULL)
	{
		if(strcmp(pToken, nID2))
		{//如果pToken不是ID2就将其复制到Friendlist
			if(strlen(pInfo->FriendList) == 0)
			{//	第一个好友
				strcat_s(pInfo->FriendList, pToken); // 添加ID						
			}
			else
			{// 非第一个好友
				strcat_s(pInfo->FriendList, ","); // 添加分隔符
				strcat_s(pInfo->FriendList, pToken); // 添加ID
			}
		}
		else
		{
			Flag = TRUE; // 存在此好友 则下面也需要继续执行
		}
		//pToken = strtok(NULL, ","); // 继续分割剩余的好友ID
		pToken = strtok_s(NULL, ",", &pTokenNext);
	}

	if(!Flag) // 不存在此好友 下面不需要继续执行
		return IDS_ERR_FRIEND_NOT_EXIST;

	strcpy_s(FriLst, pInfo2->FriendList); // 拷贝原好友列表
	memset(pInfo2->FriendList, 0, FRIEND_MAX * ID_MAX * sizeof(char)); // 清空好友列表	
	pTokenNext = NULL;
	pToken = strtok_s(FriLst, ",", &pTokenNext); // 分割好友列表			
	while(pToken != NULL)
	{
		if(strcmp(pToken, nID1))
		{//如果pToken不是ID1就将其复制到Friendlist
			if(strlen(pInfo2->FriendList) == 0)
			{//	第一个好友
				strcat_s(pInfo2->FriendList, pToken); // 添加ID						
			}
			else
			{// 非第一个好友
				strcat_s(pInfo2->FriendList, ","); // 添加分隔符
				strcat_s(pInfo2->FriendList, pToken); // 添加ID
			}
		}
		//pToken = strtok(NULL, ","); // 继续分割剩余的好友ID
		pToken = strtok_s(NULL, ",", &pTokenNext);
	}
	return TRUE; // 双方都删除完成
}


/*********************************************************
函数名称：LoadData
功能描述：保存账号数据
创建时间：2016-08-18
参数说明：
返 回 值：成功返回TRUE
*********************************************************/
BOOL CData::SaveData(CString csFilePath)const
{
	CFile file;	
	//int nReadBytes = 0; //存储文件中读取的总字节数

	if(!file.Open(L".//data.txt", CFile::modeWrite | CFile::modeCreate))
	{
		return FALSE;
	}
	POSITION pos = m_userlist.GetHeadPosition();
	while(pos != NULL)
	{
		USERINFO *p = m_userlist.GetNext(pos);
		file.Write(p, sizeof(*p));
	}
	file.Close();



	return TRUE;
}


/*********************************************************
函数名称：LoadData
功能描述：加载账号数据
创建时间：2016-08-18
参数说明：msg_info-储存所有信息的结构体
返 回 值：成功返回TRUE
*********************************************************/
BOOL CData::LoadData(CString csFilePath)
{
	CFile file;
	if(!file.Open(L".//data.txt", CFile::modeReadWrite))
	{
		return FALSE;
	}
	
	POSITION pos = m_userlist.GetHeadPosition();
	while(pos != NULL)
	{
		USERINFO *p = m_userlist.GetNext(pos);
		//delete p;
		if(pos == NULL)
		{// 已经到了链表尾
			m_userlist.RemoveTail();
		}
		else
		{
			m_userlist.GetPrev(pos);
			m_userlist.RemoveAt(pos);
			pos = m_userlist.GetHeadPosition();
		}
	}
	
	int maxID;
	
	while(TRUE)
	{
		USERINFO *p = new USERINFO;
		int nRet = file.Read(p, sizeof(USERINFO));
		p->nStatus = IDS_STATUS_OFFLINE; // 状态重置	
		
		maxID = atoi(p->nID);
		if(m_MaxID <= maxID)
		{
			m_MaxID = maxID;
		}
		
		if(nRet == 0)
		{
			delete p;
			break;
		}
		else
		{
			m_userlist.AddTail(p);		
		}
	}
	file.Close();

	return TRUE;
}


/*********************************************************
函数名称：GetAllFriendInfo
功能描述：获得某个ID所有好友的基本信息
创建时间：2016-08-07
参数说明：msg_info-储存所有信息的结构体
返 回 值：成功返回TRUE
*********************************************************/
int CData::GetAllFriendInfo(struct MSG_FRND_INFO* msg_info, char* nID)
{
	struct USERINFO *pUser = NULL;
	if(!IsExist(&pUser, nID))
	{// 用户不存在
		return IDS_ERR_USER_NOT_EXIST;
	}
	
	char FriLst[FRIEND_MAX * ID_MAX];
	//memcpy(FriLst, pUser->FriendList, FRIEND_MAX * ID_MAX);
	strcpy_s(FriLst, pUser->FriendList); // 复制好友列表

	int nNum = 0; // 记录好友数目
	char *pToken = NULL; // 分割好友列表
	char *pTokenNext = NULL;
	pToken = strtok_s(FriLst, ",", &pTokenNext); 
	while(pToken != NULL)
	{
		struct USERINFO *pFriend = NULL;
		if(IsExist(&pFriend, pToken))
		{// 存在此ID
			msg_info->nStatus[nNum] = pFriend->nStatus;
			strcpy_s(msg_info->ListID[nNum], pFriend->nID);
			memcpy(msg_info->ListName[nNum], pFriend->Name, NAME_MAX);
		}
		++nNum;
		pToken = strtok_s(NULL, ",", &pTokenNext); 
	}
	msg_info->nNum = nNum;
	// 完成
	return TRUE;
}


// 下面是临时的函数 有可能会删除掉

// 设置所有用户的在线状态
int CData::SetAllUserStatus(int nStatus)
{
	POSITION pos = m_userlist.GetHeadPosition();
	struct USERINFO *puif = NULL;
	while(pos != NULL)
	{
		puif = m_userlist.GetNext(pos);
		puif->nStatus = nStatus;
	}
	return 0;
}

// 输出信息
void CData::ShowData(CListCtrl * pListCtrl)const
{
	// 删除所有行
	pListCtrl->DeleteAllItems();
	// 删除所有列
	CHeaderCtrl* pHeaderCtrl = pListCtrl->GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
	{
		int  nColumnCount = pHeaderCtrl->GetItemCount();
		for (int i=0; i<nColumnCount; i++)
		{
			pListCtrl->DeleteColumn(0);
		}
	}

	// 给m_lstctlData添加表头
	CRect rect;
	pListCtrl->GetClientRect(&rect);
	int nColInterval = rect.Width();
	pListCtrl->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	pListCtrl->InsertColumn(0, _T("ID"), LVCFMT_CENTER,			int(nColInterval / 8));   
    pListCtrl->InsertColumn(1, _T("Name"), LVCFMT_CENTER,		int(nColInterval / 8));
	pListCtrl->InsertColumn(2, _T("Email"), LVCFMT_CENTER,		int(nColInterval / 8));   
	pListCtrl->InsertColumn(3, _T("Sex"), LVCFMT_CENTER,		int(nColInterval / 8));   
	pListCtrl->InsertColumn(4, _T("Age"), LVCFMT_CENTER,		int(nColInterval / 8));    
	pListCtrl->InsertColumn(5, _T("Status"), LVCFMT_CENTER,		int(nColInterval / 8));   
	pListCtrl->InsertColumn(6, _T("RegDate"), LVCFMT_CENTER,	int(nColInterval / 8));
	pListCtrl->InsertColumn(7, _T("PassWord"), LVCFMT_CENTER,	int(nColInterval / 8));
	
	// 添加数据
	int i = 0;
	POSITION pos = m_userlist.GetHeadPosition();
	struct USERINFO *puif = NULL;
	while(pos != NULL)
	{
		puif = m_userlist.GetNext(pos);
		CString strTemp; // 用于转换多字符

		//strTemp.Format("%s", puif->nID); // nID是多字符
		strTemp = puif->nID;
		pListCtrl->InsertItem(i, strTemp);
		strTemp = (LPCTSTR)puif->Name; // 昵称是以宽字符来储存
		pListCtrl->SetItemText(i, 1, strTemp);
		strTemp = puif->Email; // Email是多字符
		pListCtrl->SetItemText(i, 2, strTemp); 
		strTemp = puif->Sex; // Sex是单字符
		pListCtrl->SetItemText(i, 3, strTemp); 
		strTemp.Format(L"%d", puif->nAge); // 年龄是数字
		pListCtrl->SetItemText(i, 4, strTemp); 
		//strTemp.Format(L"%d", puif->nStatus); // 状态是数字
		strTemp.LoadStringW(puif->nStatus);
		pListCtrl->SetItemText(i, 5, strTemp); 
		strTemp = puif->RegDate; // 注册日期是多字符
		pListCtrl->SetItemText(i, 6, strTemp);
		strTemp = puif->PassWord; // 密码是多字符
		pListCtrl->SetItemText(i, 7, strTemp);
		++i;	
	}
}
