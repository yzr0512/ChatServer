
// SocketServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSocketServerDlg 对话框


/*********************************************************
函数名称：CSocketServerDlg
功能描述：构造函数

*********************************************************/
CSocketServerDlg::CSocketServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSocketServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_csOutMsg = _T("");
	m_csSendMsg = _T("");
	m_csID = _T("");
	m_pDataDlg = NULL;
}

void CSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OUT_MSG, m_csOutMsg);
	DDX_Text(pDX, IDC_EDIT_SEND_MSG, m_csSendMsg);
	DDX_Control(pDX, IDC_LIST_CLIENT, m_lstClient);
	DDX_Text(pDX, IDC_EDIT_ID, m_csID);
}

BEGIN_MESSAGE_MAP(CSocketServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CLIENT, &CSocketServerDlg::OnLvnItemchangedListClient)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSocketServerDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSocketServerDlg::OnBnClickedButtonOpen)
	ON_COMMAND(ID_DATA_DIALOG, &CSocketServerDlg::OnDataDialog)
END_MESSAGE_MAP()


// CSocketServerDlg 消息处理程序

BOOL CSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	//初始化客户端列表
	
	// 获取列表控件的宽度
	CRect rect;
	m_lstClient.GetClientRect(&rect);
	int nColInterval = rect.Width();

	// 设置ListCtrl的样式
	//  LVS_EX_GRIDLINES 网格线
	//  LVS_EX_FULLROWSELECT 整行选中
	m_lstClient.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// 插入表头
	// 参数分别为：列索引（第几列）、列名、列的文字格式、列宽、与列相关联子项的索引
	//  LVCFMT_CENTER 居中
	//  LVCFMT_LEFT   左对齐
	//  LVCFMT_RIGHT  右对齐
	m_lstClient.InsertColumn(0, _T("客户端列表"), LVCFMT_CENTER, int(nColInterval));   
   
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSocketServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*********************************************************
函数名称：OpenServer
功能描述：初始化服务器
返 回 值：成功返回IDS_SERVER_OPEN_SUCCESS
*********************************************************/
int CSocketServerDlg::OpenServer(void)
{
	m_pSocketListen = new CListenSocket(this);

	if(!AfxSocketInit())
	{// 套接字初始化失败		
		OutputInfo(IDP_SOCKETS_INIT_FAILED); // 输出信息
		delete m_pSocketListen; // 清除Socket
		m_pSocketListen = NULL; // 指针置空
		return IDP_SOCKETS_INIT_FAILED;
	}

	if(m_pSocketListen->Create(8080) == NULL) 
	{// 绑定端口失败
		OutputInfo(IDP_SOCKETS_BIND_FAILED); // 输出信息
		delete m_pSocketListen; //清除Socket
		m_pSocketListen = NULL;
		return IDP_SOCKETS_BIND_FAILED;
	}
	
	m_pSocketListen->Listen(); //启动监听
	OutputInfo(IDS_SERVER_OPEN_SUCCESS); // 输出信息

	return IDS_SERVER_OPEN_SUCCESS;
}


/*********************************************************
函数名称：AcceptClient
功能描述：监听连接请求
返 回 值：无意义
*********************************************************/
int CSocketServerDlg::AcceptClient(void)
{
	//定义一个用于交互的Socket
	CChatSocket *pSocket = new CChatSocket(this);

	if(!m_pSocketListen->Accept(*pSocket))
	{// 连接客户端失败
		delete pSocket;
		pSocket = NULL;
	}
	else
	{// 连接成功 将pSocket指针加到m_listSocketChat末尾
		m_listSocketChat.AddTail(pSocket);			
	}
	return 0;
}


/*********************************************************
函数名称：RecvMsg
功能描述：接收信息
参数说明：pSocket-接收到消息的pSocket
返 回 值：无意义
*********************************************************/
int CSocketServerDlg::RecvMsg(CChatSocket * pSocket)
{
	int iBufSize = 1024; // 接收的最大字节数
	struct MSG_T *msg = new struct MSG_T;
	int iRes = pSocket->Receive((char *)msg, iBufSize);	
	if(iRes == SOCKET_ERROR) // 接收失败
	{
		//MessageBox(_T("SOCKET_ERROR"));
		OutputInfo(_T("SOCKET_ERROR"));
		return 0;
	}
	switch(msg->nType)
	{
	case LOGIN: // 登录消息
		Login((struct MSG_LOGIN *)msg, pSocket);break;
	case LOGIN_OUT: // 下线消息
		LoginOut((struct MSG_LOGIN *)msg, pSocket);break;

	case REGISTER: // 注册消息
		Register((struct MSG_REGISTER *)msg, pSocket);break;

	case GET_FRIEND_LIST: // 请求获得好友列表
		GetFriendList((struct MSG_USERINFO *)msg, pSocket);break;
	case GET_FRIEND_INFO: // 请求获得好友信息
		GetFriendInfo((struct MSG_USERINFO *)msg, pSocket);break;
	case GET_ALL_FRIEND_INFO:
		
		break;
	case GET_STRANGER_INFO: // 请求获得陌生人信息
		GetStrangerInfo((struct MSG_USERINFO *)msg, pSocket);break;
	case STATUS_ONLINE: // 设置在线状态
		SetUserStatus((struct MSG_USERINFO *)msg, pSocket);break;

	case CHATING_TEXT_MSG: // 聊天消息
		RelayChatMsg((struct MSG_TRANSPOND *)msg, pSocket);break;
	case ADD_FRIEND_REQUEST: // 添加好友请求
		RelayChatMsg((struct MSG_TRANSPOND *)msg, pSocket);break;
	case ADD_FRIEND_ANSWER: // 添加请求回应
		AddFriend((struct MSG_TRANSPOND *)msg, pSocket);break;
	case DELETE_FRIEND: // 删除好友
		DeleteFriend((struct MSG_TRANSPOND *)msg, pSocket);break;


	}
	return 0;
}


//下面是各种消息的处理函数
//

/*********************************************************
函数名称：Login
功能描述：用户登录
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
*********************************************************/
int CSocketServerDlg::Login(struct MSG_LOGIN * msg, CChatSocket * pSocket)
{
	int nRes = m_data.IsCorrect(msg->nID, msg->Key);

	if(nRes == TRUE)// 检查账号密码正确性
	{// 密码正确
		if(m_data.GetUserStatus(msg->nID) == STATUS_OFFLINE)
		{// 允许登录
			// 给客户端返回账号信息
			struct MSG_USERINFO msg_info;
			msg_info.nType = LOGIN_SUCCESS;
			m_data.GetUserInfo(msg_info, msg->nID);
			if(pSocket->Send((void *)&msg_info, sizeof(msg_info)) == 0)
			{//发送失败
				OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
				return IDS_ERR_SOCKETS_SEND_FAILED;
			}

			strcpy_s(pSocket->m_userID, msg->nID); // 设置pSocket的nID
			m_data.SetUserStatus(msg->nID, msg->nStatus);		
			RefreshListCtrlData(); // 更新列表控件中的数据
			
			// 在编辑框输出信息		
			CString csOutMsg;
			CString csID;
			csID = msg->nID;
			csOutMsg.Format(_T("用户%s已连接"), csID);
			OutputInfo(csOutMsg);
		}
		else
		{// 拒绝登录 原因：重复登录
			// 服务器返回信息 告诉客户端登录失败
			struct MSG_SYS msg_sys;
			msg_sys.nType = IDS_SYSTEM_MESSAGE;
			msg_sys.nIDPrompt = IDS_ERR_ID_HAD_LOGIN;
			if(pSocket->Send((void *)&msg_sys, sizeof(msg_sys)) == 0)
			{//发送失败
				OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
				return IDS_ERR_SOCKETS_SEND_FAILED;
			}
			/*struct MSG_T msg_reply;
			msg_reply.nType = LOGIN_FAILED;
			memset(msg_reply.Data, '\0', sizeof(msg_reply.Data));	
			memcpy(msg_reply.Data, _T("此账号已登录"), sizeof(_T("此账号已登录")));			
			if(pSocket->Send((void *)&msg_reply, sizeof(msg_reply)) == 0)
			{//发送失败
				OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
				return IDS_ERR_SOCKETS_SEND_FAILED;
			}*/			
		}
	}
	else if(nRes == IDS_ERR_PASSWD_INCORRECT)
	{// 拒绝登陆 原因：密码错误
		// 服务器返回信息 告诉客户端登录失败
		struct MSG_SYS msg_sys;
		msg_sys.nType = IDS_SYSTEM_MESSAGE;
		msg_sys.nIDPrompt = IDS_ERR_PASSWD_INCORRECT;
		if(pSocket->Send((void *)&msg_sys, sizeof(msg_sys)) == 0)
		{//发送失败
			OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);				
			return IDS_ERR_SOCKETS_SEND_FAILED;
		}	
		/*struct MSG_T msg_reply;
		msg_reply.nType = LOGIN_FAILED;
		memset(msg_reply.Data, '\0', sizeof(msg_reply.Data));	
		memcpy(msg_reply.Data, _T("账号或密码不正确"), sizeof(_T("账号或密码不正确")));			
		if(pSocket->Send((void *)&msg_reply, sizeof(msg_reply)) == 0)
		{//发送失败
			OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);				
			return IDS_ERR_SOCKETS_SEND_FAILED;
		}*/
	}
	else if(nRes == IDS_ERR_USER_NOT_EXIST)
	{// 用户不存在
		struct MSG_SYS msg_sys;
		msg_sys.nType = IDS_SYSTEM_MESSAGE;
		msg_sys.nIDPrompt = IDS_ERR_USER_NOT_EXIST;
		pSocket->Send(&msg_sys, sizeof(msg_sys));
	}
	return TRUE;
}


/*********************************************************
函数名称：LoginOut
功能描述：用户下线
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
*********************************************************/
int CSocketServerDlg::LoginOut(struct MSG_LOGIN * msg, CChatSocket * pSocket)
{
	// 将状态设置为离线
	m_data.SetUserStatus(pSocket->m_userID, STATUS_OFFLINE);

	CString csOutMsg;
	CString csID;
	csID = msg->nID;
	csOutMsg.Format(_T("用户%s已退出"), csID);
	OutputInfo(csOutMsg);
	//pSocket->ID = "";
	memset(pSocket->m_userID, 0, ID_MAX);
	// 刷新用户列表
	RefreshListCtrlData();
	return 0;
}


/*********************************************************
函数名称：Register
功能描述：用户注册
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
*********************************************************/
int CSocketServerDlg::Register(struct MSG_REGISTER * msg_reg, CChatSocket * pSocket)
{
	char nID[ID_MAX];
	m_data.AddUser(nID, msg_reg);

	msg_reg->nType = REGISTER_SUCCESS;
	strcpy_s(msg_reg->nID, nID);
	if(pSocket->Send(msg_reg, sizeof(*msg_reg)) == 0)
	{// 发送失败
		OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
		return IDS_ERR_SOCKETS_SEND_FAILED;
	}
	delete msg_reg; // 释放内存
	return 0;
}


/*********************************************************
函数名称：RelayChatMsg
功能描述：转发聊天消息
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
*********************************************************/
int CSocketServerDlg::RelayChatMsg(struct MSG_TRANSPOND * msg_tran, CChatSocket * pSocket)
{
	POSITION pos = m_listSocketChat.GetHeadPosition();
	while(pos != NULL)
	{
		CChatSocket *p = m_listSocketChat.GetNext(pos);
		if(!strcmp(p->m_userID, msg_tran->ToID))
		{
			p->Send(msg_tran, sizeof(*msg_tran));
			delete msg_tran;
			return 0;
		}
	}
	// 如果对方不在线 则先将消息存起来
	m_listChatMsg.AddTail(msg_tran);

	return 0;
}


/*********************************************************
函数名称：GetFriendList
功能描述：获得好友列表
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
*********************************************************/
int CSocketServerDlg::GetFriendList(struct MSG_USERINFO * msg_userinfo, CChatSocket * pSocket)
{	
	struct MSG_USERINFO UserInfo; // 用来回复的结构体
	UserInfo.nType = GET_FRIEND_LIST;
	if(m_data.GetUserFriendList(UserInfo.FriendList, msg_userinfo->nID))
	{
		pSocket->Send(&UserInfo, sizeof(UserInfo));
	}
	return 0;
}


/*********************************************************
函数名称：GetFriendInfo
功能描述：获得好友信息
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
*********************************************************/
int CSocketServerDlg::GetFriendInfo(struct MSG_USERINFO * msg_userinfo, CChatSocket * pSocket)
{
	struct MSG_USERINFO info;	
	if(m_data.GetUserInfo(info, msg_userinfo->nID) != IDS_ERR_USER_NOT_EXIST)
	{
		info.nType = GET_FRIEND_INFO;	
		if(pSocket->Send(&info, sizeof(info)) == 0)
		{//发送失败
			OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
			return IDS_ERR_SOCKETS_SEND_FAILED;
		}
		
	}
	else
	{// 查询的用户不存在
		
	
	}
	return 0;
}


/*********************************************************
函数名称：GetStrangerInfo
功能描述：获得陌生人信息
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
*********************************************************/
int CSocketServerDlg::GetStrangerInfo(struct MSG_USERINFO * msg_userinfo, CChatSocket * pSocket)
{
	struct MSG_USERINFO info;
	if(m_data.GetUserPublicInfo(info, msg_userinfo->nID) != IDS_ERR_USER_NOT_EXIST)
	{
		info.nType = GET_STRANGER_INFO;	
		if(pSocket->Send(&info, sizeof(info)) == 0)
		{//发送失败
			OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
			return IDS_ERR_SOCKETS_SEND_FAILED;
		}
	}
	else
	{// 查询的用户不存在
		struct MSG_SYS msg_sys;
		msg_sys.nType = IDS_SYSTEM_MESSAGE;
		msg_sys.nIDPrompt = IDS_ERR_USER_NOT_EXIST;
		pSocket->Send(&msg_sys, sizeof(msg_sys));
	}
	return 0;
}


/*********************************************************
函数名称：AddFriend
功能描述：添加好友
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
备    注：
*********************************************************/
int CSocketServerDlg::AddFriend(struct MSG_TRANSPOND * msg_addfrd, CChatSocket * pSocket) 
{
	struct MSG_SYS *msg_sys = new struct MSG_SYS;
	msg_sys->nType = IDS_SYSTEM_MESSAGE;

	if(msg_addfrd->nReturn == ADD_FRIEND_AGREE)
	{// 同意好友申请
		int nRes;
		nRes = m_data.MakeFriend(msg_addfrd->FromID, msg_addfrd->ToID);
		if(nRes == TRUE)
		{// 添加成功		
			msg_sys->nIDPrompt = IDS_ADD_FRIEND_SUCCESS;
		}
		else if(nRes == IDS_ERR_FRIEND_ADD_SELF)
		{// 不能添加自己为好友
			msg_sys->nIDPrompt = IDS_ERR_FRIEND_ADD_SELF;			
		}
		else if(nRes == IDS_ERR_FRIEND_HAD_EXIST)
		{// 好友已经存在
			msg_sys->nIDPrompt = IDS_ERR_FRIEND_HAD_EXIST;
		}

		// 分别向双方发送消息
		strcpy_s(msg_sys->nID, pSocket->m_userID);
		pSocket->Send(msg_sys, sizeof(*msg_sys));

		strcpy_s(msg_sys->nID, msg_addfrd->FromID);
		SendSystemMsg(msg_sys); // 在这个函数里面会将msg_sys释放
	}
	else
	{		
		msg_sys->nIDPrompt = IDS_ADD_FRIEND_REFUSE;
		strcpy_s(msg_sys->nID, msg_addfrd->FromID);
		SendSystemMsg(msg_sys); // 在这个函数里面会将msg_sys释放
	}
	return TRUE;
}


/*********************************************************
函数名称：DeleteFriend
功能描述：删除好友
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
备    注：
*********************************************************/
int CSocketServerDlg::DeleteFriend(struct MSG_TRANSPOND * msg_del, CChatSocket * pSocket)
{
	if(m_data.BreakFriend(msg_del->FromID, msg_del->ToID) == TRUE)
	{// 删除完成
		struct MSG_SYS msg_sys;
		msg_sys.nType = IDS_SYSTEM_MESSAGE;
		strcpy_s(msg_sys.nID, msg_del->FromID);
		msg_sys.nIDPrompt = IDS_DEL_FRIEND_SUCCESS;
		pSocket->Send(&msg_sys, sizeof(msg_sys));
	}
	return 0;
}


/*********************************************************
函数名称：SetUserStatus
功能描述：设置在线状态
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
备    注：
*********************************************************/
int CSocketServerDlg::SetUserStatus(struct MSG_USERINFO * msg_userinfo, CChatSocket * pSocket)
{
	m_data.SetUserStatus(msg_userinfo->nID, msg_userinfo->nStatus);
	return 0;
}




// 遍历未发送的消息

// 遍历socket 看看是否保持连接



// 刷新列表控件的内容
int CSocketServerDlg::RefreshListCtrlData(void)
{
	// 删除所有数据
	m_lstClient.DeleteAllItems();
	if(m_listSocketChat.IsEmpty())
	{
		return 1;
	}

	// 遍历m_listSocketChat 输出里面所有不为空的ID
	POSITION pos = m_listSocketChat.GetHeadPosition();
	CChatSocket *pChatSocket;
	while(pos != NULL)
	{
		pChatSocket = m_listSocketChat.GetNext(pos);
		if(pChatSocket->ID != "")
		{
			m_lstClient.InsertItem(0, pChatSocket->ID); 
		}		
	}

	if(m_pDataDlg != NULL)
	{
		m_pDataDlg->RefreshListCtrlData(&m_data);
	}

	return 0;
}


void CSocketServerDlg::OnLvnItemchangedListClient(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	UpdateData(TRUE);
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if(-1 != pNMListView->iItem) // 如果iItem不是-1，就说明有列表项被选择 
	{
		m_csID = m_lstClient.GetItemText(pNMListView->iItem, 0);
		UpdateData(FALSE);
	}

}

// 服务器发送信息给某人
void CSocketServerDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	if(m_csID == "") // 如果选择的客户端为空
	{
		MessageBox(_T("请选择一个客户端！"));
		return;
	}

	// 遍历m_listSocketChat 找到对应ID的socket
	POSITION pos = m_listSocketChat.GetHeadPosition();
	CChatSocket *pChatSocket;
	int nFlag = 0; // 0表示不存在此客户端
	while(pos != NULL)
	{
		pChatSocket = m_listSocketChat.GetNext(pos);
		if(pChatSocket->ID == m_csID)
		{
			nFlag = 1;
			struct MSG_T msgSend;
			msgSend.nType = CHATING_TEXT_MSG;
			
			memset(msgSend.Data, '\0', sizeof(msgSend.Data));	
			memcpy(msgSend.Data, m_csSendMsg, m_csSendMsg.GetLength() * 2);
			
			if(pChatSocket->Send((void *)&msgSend, sizeof(msgSend)) == 0)
			{
				//发送失败
				MessageBox(_T("发送失败"));
				return;
			}
			// 在本地编辑框中输出信息
			CString strTemp;
			strTemp.Format(_T("给用户%s：%s\r\n"), pChatSocket->ID, m_csSendMsg);
			m_csOutMsg += strTemp;
			m_csSendMsg = "";

			UpdateData(FALSE);
			break;
		}	
	}
	if(nFlag == 0) // 不存在此ID的socket
	{
		MessageBox(_T("此客户端不存在！"));
		return;	
	}

}

// 开启服务
void CSocketServerDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	OpenServer();

}

// 输出消息
int CSocketServerDlg::OutputInfo(const CString& csOutMsg)
{
	CTime tm = CTime::GetCurrentTime();
	tm.Format(_T("%H:%M:%S"));
	m_csOutMsg += tm.Format(_T("%H:%M:%S "));
	m_csOutMsg += csOutMsg;
	m_csOutMsg += "\r\n";
	UpdateData(FALSE);

	return 0;
}

int CSocketServerDlg::OutputInfo(UINT nIDPrompt)
{
	CString csMsg;
	csMsg.LoadString(nIDPrompt);	
	CTime tm = CTime::GetCurrentTime();
	tm.Format(_T("%H:%M:%S"));
	m_csOutMsg += tm.Format(_T("%H:%M:%S "));
	m_csOutMsg += csMsg;
	m_csOutMsg += "\r\n";
	UpdateData(FALSE);
	return 0;
}


// 发送系统消息
int CSocketServerDlg::SendSystemMsg(struct MSG_SYS * msg_sys)
{
	POSITION pos = m_listSocketChat.GetHeadPosition();
	while(pos != NULL)
	{
		CChatSocket *p = m_listSocketChat.GetNext(pos);
		if(!strcmp(p->m_userID, msg_sys->nID))
		{
			p->Send(msg_sys, sizeof(struct MSG_SYS));
			delete msg_sys;
			return 0;
		}
	}
	// 如果对方不在线 则先将消息存起来

	m_listSystemMsg.AddTail(msg_sys);

	return 0;
}


/*********************************************************
函数名称：OnDataDialog
功能描述：打开数据管理窗口
参数说明：msg-消息结构体 pSocket-接收到消息的pSocket
返 回 值：
备    注：
*********************************************************/
void CSocketServerDlg::OnDataDialog()
{
	// TODO: 在此添加命令处理程序代码
	if(m_pDataDlg == NULL)
	{
		m_pDataDlg = new CDataDlg(this);
		m_pDataDlg->Create(IDD_DATA_DLG);
		m_pDataDlg->ShowWindow(SW_SHOW);
		m_pDataDlg->RefreshListCtrlData(&m_data);
	}

}
