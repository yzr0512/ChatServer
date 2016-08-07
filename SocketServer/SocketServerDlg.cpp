
// SocketServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSocketServerDlg �Ի���


/*********************************************************
�������ƣ�CSocketServerDlg
�������������캯��

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


// CSocketServerDlg ��Ϣ�������

BOOL CSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	//��ʼ���ͻ����б�
	
	// ��ȡ�б�ؼ��Ŀ��
	CRect rect;
	m_lstClient.GetClientRect(&rect);
	int nColInterval = rect.Width();

	// ����ListCtrl����ʽ
	//  LVS_EX_GRIDLINES ������
	//  LVS_EX_FULLROWSELECT ����ѡ��
	m_lstClient.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// �����ͷ
	// �����ֱ�Ϊ�����������ڼ��У����������е����ָ�ʽ���п�������������������
	//  LVCFMT_CENTER ����
	//  LVCFMT_LEFT   �����
	//  LVCFMT_RIGHT  �Ҷ���
	m_lstClient.InsertColumn(0, _T("�ͻ����б�"), LVCFMT_CENTER, int(nColInterval));   
   
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSocketServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*********************************************************
�������ƣ�OpenServer
������������ʼ��������
�� �� ֵ���ɹ�����IDS_SERVER_OPEN_SUCCESS
*********************************************************/
int CSocketServerDlg::OpenServer(void)
{
	m_pSocketListen = new CListenSocket(this);

	if(!AfxSocketInit())
	{// �׽��ֳ�ʼ��ʧ��		
		OutputInfo(IDP_SOCKETS_INIT_FAILED); // �����Ϣ
		delete m_pSocketListen; // ���Socket
		m_pSocketListen = NULL; // ָ���ÿ�
		return IDP_SOCKETS_INIT_FAILED;
	}

	if(m_pSocketListen->Create(8080) == NULL) 
	{// �󶨶˿�ʧ��
		OutputInfo(IDP_SOCKETS_BIND_FAILED); // �����Ϣ
		delete m_pSocketListen; //���Socket
		m_pSocketListen = NULL;
		return IDP_SOCKETS_BIND_FAILED;
	}
	
	m_pSocketListen->Listen(); //��������
	OutputInfo(IDS_SERVER_OPEN_SUCCESS); // �����Ϣ

	return IDS_SERVER_OPEN_SUCCESS;
}


/*********************************************************
�������ƣ�AcceptClient
����������������������
�� �� ֵ��������
*********************************************************/
int CSocketServerDlg::AcceptClient(void)
{
	//����һ�����ڽ�����Socket
	CChatSocket *pSocket = new CChatSocket(this);

	if(!m_pSocketListen->Accept(*pSocket))
	{// ���ӿͻ���ʧ��
		delete pSocket;
		pSocket = NULL;
	}
	else
	{// ���ӳɹ� ��pSocketָ��ӵ�m_listSocketChatĩβ
		m_listSocketChat.AddTail(pSocket);			
	}
	return 0;
}


/*********************************************************
�������ƣ�RecvMsg
����������������Ϣ
����˵����pSocket-���յ���Ϣ��pSocket
�� �� ֵ��������
*********************************************************/
int CSocketServerDlg::RecvMsg(CChatSocket * pSocket)
{
	int iBufSize = 1024; // ���յ�����ֽ���
	struct MSG_T *msg = new struct MSG_T;
	int iRes = pSocket->Receive((char *)msg, iBufSize);	
	if(iRes == SOCKET_ERROR) // ����ʧ��
	{
		//MessageBox(_T("SOCKET_ERROR"));
		OutputInfo(_T("SOCKET_ERROR"));
		return 0;
	}
	switch(msg->nType)
	{
	case LOGIN: // ��¼��Ϣ
		Login((struct MSG_LOGIN *)msg, pSocket);break;
	case LOGIN_OUT: // ������Ϣ
		LoginOut((struct MSG_LOGIN *)msg, pSocket);break;

	case REGISTER: // ע����Ϣ
		Register((struct MSG_REGISTER *)msg, pSocket);break;

	case GET_FRIEND_LIST: // �����ú����б�
		GetFriendList((struct MSG_USERINFO *)msg, pSocket);break;
	case GET_FRIEND_INFO: // �����ú�����Ϣ
		GetFriendInfo((struct MSG_USERINFO *)msg, pSocket);break;
	case GET_ALL_FRIEND_INFO:
		
		break;
	case GET_STRANGER_INFO: // ������İ������Ϣ
		GetStrangerInfo((struct MSG_USERINFO *)msg, pSocket);break;
	case STATUS_ONLINE: // ��������״̬
		SetUserStatus((struct MSG_USERINFO *)msg, pSocket);break;

	case CHATING_TEXT_MSG: // ������Ϣ
		RelayChatMsg((struct MSG_TRANSPOND *)msg, pSocket);break;
	case ADD_FRIEND_REQUEST: // ��Ӻ�������
		RelayChatMsg((struct MSG_TRANSPOND *)msg, pSocket);break;
	case ADD_FRIEND_ANSWER: // ��������Ӧ
		AddFriend((struct MSG_TRANSPOND *)msg, pSocket);break;
	case DELETE_FRIEND: // ɾ������
		DeleteFriend((struct MSG_TRANSPOND *)msg, pSocket);break;


	}
	return 0;
}


//�����Ǹ�����Ϣ�Ĵ�����
//

/*********************************************************
�������ƣ�Login
�����������û���¼
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
*********************************************************/
int CSocketServerDlg::Login(struct MSG_LOGIN * msg, CChatSocket * pSocket)
{
	int nRes = m_data.IsCorrect(msg->nID, msg->Key);

	if(nRes == TRUE)// ����˺�������ȷ��
	{// ������ȷ
		if(m_data.GetUserStatus(msg->nID) == STATUS_OFFLINE)
		{// �����¼
			// ���ͻ��˷����˺���Ϣ
			struct MSG_USERINFO msg_info;
			msg_info.nType = LOGIN_SUCCESS;
			m_data.GetUserInfo(msg_info, msg->nID);
			if(pSocket->Send((void *)&msg_info, sizeof(msg_info)) == 0)
			{//����ʧ��
				OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
				return IDS_ERR_SOCKETS_SEND_FAILED;
			}

			strcpy_s(pSocket->m_userID, msg->nID); // ����pSocket��nID
			m_data.SetUserStatus(msg->nID, msg->nStatus);		
			RefreshListCtrlData(); // �����б�ؼ��е�����
			
			// �ڱ༭�������Ϣ		
			CString csOutMsg;
			CString csID;
			csID = msg->nID;
			csOutMsg.Format(_T("�û�%s������"), csID);
			OutputInfo(csOutMsg);
		}
		else
		{// �ܾ���¼ ԭ���ظ���¼
			// ������������Ϣ ���߿ͻ��˵�¼ʧ��
			struct MSG_SYS msg_sys;
			msg_sys.nType = IDS_SYSTEM_MESSAGE;
			msg_sys.nIDPrompt = IDS_ERR_ID_HAD_LOGIN;
			if(pSocket->Send((void *)&msg_sys, sizeof(msg_sys)) == 0)
			{//����ʧ��
				OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
				return IDS_ERR_SOCKETS_SEND_FAILED;
			}
			/*struct MSG_T msg_reply;
			msg_reply.nType = LOGIN_FAILED;
			memset(msg_reply.Data, '\0', sizeof(msg_reply.Data));	
			memcpy(msg_reply.Data, _T("���˺��ѵ�¼"), sizeof(_T("���˺��ѵ�¼")));			
			if(pSocket->Send((void *)&msg_reply, sizeof(msg_reply)) == 0)
			{//����ʧ��
				OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
				return IDS_ERR_SOCKETS_SEND_FAILED;
			}*/			
		}
	}
	else if(nRes == IDS_ERR_PASSWD_INCORRECT)
	{// �ܾ���½ ԭ���������
		// ������������Ϣ ���߿ͻ��˵�¼ʧ��
		struct MSG_SYS msg_sys;
		msg_sys.nType = IDS_SYSTEM_MESSAGE;
		msg_sys.nIDPrompt = IDS_ERR_PASSWD_INCORRECT;
		if(pSocket->Send((void *)&msg_sys, sizeof(msg_sys)) == 0)
		{//����ʧ��
			OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);				
			return IDS_ERR_SOCKETS_SEND_FAILED;
		}	
		/*struct MSG_T msg_reply;
		msg_reply.nType = LOGIN_FAILED;
		memset(msg_reply.Data, '\0', sizeof(msg_reply.Data));	
		memcpy(msg_reply.Data, _T("�˺Ż����벻��ȷ"), sizeof(_T("�˺Ż����벻��ȷ")));			
		if(pSocket->Send((void *)&msg_reply, sizeof(msg_reply)) == 0)
		{//����ʧ��
			OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);				
			return IDS_ERR_SOCKETS_SEND_FAILED;
		}*/
	}
	else if(nRes == IDS_ERR_USER_NOT_EXIST)
	{// �û�������
		struct MSG_SYS msg_sys;
		msg_sys.nType = IDS_SYSTEM_MESSAGE;
		msg_sys.nIDPrompt = IDS_ERR_USER_NOT_EXIST;
		pSocket->Send(&msg_sys, sizeof(msg_sys));
	}
	return TRUE;
}


/*********************************************************
�������ƣ�LoginOut
�����������û�����
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
*********************************************************/
int CSocketServerDlg::LoginOut(struct MSG_LOGIN * msg, CChatSocket * pSocket)
{
	// ��״̬����Ϊ����
	m_data.SetUserStatus(pSocket->m_userID, STATUS_OFFLINE);

	CString csOutMsg;
	CString csID;
	csID = msg->nID;
	csOutMsg.Format(_T("�û�%s���˳�"), csID);
	OutputInfo(csOutMsg);
	//pSocket->ID = "";
	memset(pSocket->m_userID, 0, ID_MAX);
	// ˢ���û��б�
	RefreshListCtrlData();
	return 0;
}


/*********************************************************
�������ƣ�Register
�����������û�ע��
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
*********************************************************/
int CSocketServerDlg::Register(struct MSG_REGISTER * msg_reg, CChatSocket * pSocket)
{
	char nID[ID_MAX];
	m_data.AddUser(nID, msg_reg);

	msg_reg->nType = REGISTER_SUCCESS;
	strcpy_s(msg_reg->nID, nID);
	if(pSocket->Send(msg_reg, sizeof(*msg_reg)) == 0)
	{// ����ʧ��
		OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
		return IDS_ERR_SOCKETS_SEND_FAILED;
	}
	delete msg_reg; // �ͷ��ڴ�
	return 0;
}


/*********************************************************
�������ƣ�RelayChatMsg
����������ת��������Ϣ
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
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
	// ����Է������� ���Ƚ���Ϣ������
	m_listChatMsg.AddTail(msg_tran);

	return 0;
}


/*********************************************************
�������ƣ�GetFriendList
������������ú����б�
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
*********************************************************/
int CSocketServerDlg::GetFriendList(struct MSG_USERINFO * msg_userinfo, CChatSocket * pSocket)
{	
	struct MSG_USERINFO UserInfo; // �����ظ��Ľṹ��
	UserInfo.nType = GET_FRIEND_LIST;
	if(m_data.GetUserFriendList(UserInfo.FriendList, msg_userinfo->nID))
	{
		pSocket->Send(&UserInfo, sizeof(UserInfo));
	}
	return 0;
}


/*********************************************************
�������ƣ�GetFriendInfo
������������ú�����Ϣ
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
*********************************************************/
int CSocketServerDlg::GetFriendInfo(struct MSG_USERINFO * msg_userinfo, CChatSocket * pSocket)
{
	struct MSG_USERINFO info;	
	if(m_data.GetUserInfo(info, msg_userinfo->nID) != IDS_ERR_USER_NOT_EXIST)
	{
		info.nType = GET_FRIEND_INFO;	
		if(pSocket->Send(&info, sizeof(info)) == 0)
		{//����ʧ��
			OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
			return IDS_ERR_SOCKETS_SEND_FAILED;
		}
		
	}
	else
	{// ��ѯ���û�������
		
	
	}
	return 0;
}


/*********************************************************
�������ƣ�GetStrangerInfo
�������������İ������Ϣ
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
*********************************************************/
int CSocketServerDlg::GetStrangerInfo(struct MSG_USERINFO * msg_userinfo, CChatSocket * pSocket)
{
	struct MSG_USERINFO info;
	if(m_data.GetUserPublicInfo(info, msg_userinfo->nID) != IDS_ERR_USER_NOT_EXIST)
	{
		info.nType = GET_STRANGER_INFO;	
		if(pSocket->Send(&info, sizeof(info)) == 0)
		{//����ʧ��
			OutputInfo(IDS_ERR_SOCKETS_SEND_FAILED);
			return IDS_ERR_SOCKETS_SEND_FAILED;
		}
	}
	else
	{// ��ѯ���û�������
		struct MSG_SYS msg_sys;
		msg_sys.nType = IDS_SYSTEM_MESSAGE;
		msg_sys.nIDPrompt = IDS_ERR_USER_NOT_EXIST;
		pSocket->Send(&msg_sys, sizeof(msg_sys));
	}
	return 0;
}


/*********************************************************
�������ƣ�AddFriend
������������Ӻ���
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
��    ע��
*********************************************************/
int CSocketServerDlg::AddFriend(struct MSG_TRANSPOND * msg_addfrd, CChatSocket * pSocket) 
{
	struct MSG_SYS *msg_sys = new struct MSG_SYS;
	msg_sys->nType = IDS_SYSTEM_MESSAGE;

	if(msg_addfrd->nReturn == ADD_FRIEND_AGREE)
	{// ͬ���������
		int nRes;
		nRes = m_data.MakeFriend(msg_addfrd->FromID, msg_addfrd->ToID);
		if(nRes == TRUE)
		{// ��ӳɹ�		
			msg_sys->nIDPrompt = IDS_ADD_FRIEND_SUCCESS;
		}
		else if(nRes == IDS_ERR_FRIEND_ADD_SELF)
		{// ��������Լ�Ϊ����
			msg_sys->nIDPrompt = IDS_ERR_FRIEND_ADD_SELF;			
		}
		else if(nRes == IDS_ERR_FRIEND_HAD_EXIST)
		{// �����Ѿ�����
			msg_sys->nIDPrompt = IDS_ERR_FRIEND_HAD_EXIST;
		}

		// �ֱ���˫��������Ϣ
		strcpy_s(msg_sys->nID, pSocket->m_userID);
		pSocket->Send(msg_sys, sizeof(*msg_sys));

		strcpy_s(msg_sys->nID, msg_addfrd->FromID);
		SendSystemMsg(msg_sys); // �������������Ὣmsg_sys�ͷ�
	}
	else
	{		
		msg_sys->nIDPrompt = IDS_ADD_FRIEND_REFUSE;
		strcpy_s(msg_sys->nID, msg_addfrd->FromID);
		SendSystemMsg(msg_sys); // �������������Ὣmsg_sys�ͷ�
	}
	return TRUE;
}


/*********************************************************
�������ƣ�DeleteFriend
����������ɾ������
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
��    ע��
*********************************************************/
int CSocketServerDlg::DeleteFriend(struct MSG_TRANSPOND * msg_del, CChatSocket * pSocket)
{
	if(m_data.BreakFriend(msg_del->FromID, msg_del->ToID) == TRUE)
	{// ɾ�����
		struct MSG_SYS msg_sys;
		msg_sys.nType = IDS_SYSTEM_MESSAGE;
		strcpy_s(msg_sys.nID, msg_del->FromID);
		msg_sys.nIDPrompt = IDS_DEL_FRIEND_SUCCESS;
		pSocket->Send(&msg_sys, sizeof(msg_sys));
	}
	return 0;
}


/*********************************************************
�������ƣ�SetUserStatus
������������������״̬
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
��    ע��
*********************************************************/
int CSocketServerDlg::SetUserStatus(struct MSG_USERINFO * msg_userinfo, CChatSocket * pSocket)
{
	m_data.SetUserStatus(msg_userinfo->nID, msg_userinfo->nStatus);
	return 0;
}




// ����δ���͵���Ϣ

// ����socket �����Ƿ񱣳�����



// ˢ���б�ؼ�������
int CSocketServerDlg::RefreshListCtrlData(void)
{
	// ɾ����������
	m_lstClient.DeleteAllItems();
	if(m_listSocketChat.IsEmpty())
	{
		return 1;
	}

	// ����m_listSocketChat ����������в�Ϊ�յ�ID
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	
	UpdateData(TRUE);
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if(-1 != pNMListView->iItem) // ���iItem����-1����˵�����б��ѡ�� 
	{
		m_csID = m_lstClient.GetItemText(pNMListView->iItem, 0);
		UpdateData(FALSE);
	}

}

// ������������Ϣ��ĳ��
void CSocketServerDlg::OnBnClickedButtonSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);
	if(m_csID == "") // ���ѡ��Ŀͻ���Ϊ��
	{
		MessageBox(_T("��ѡ��һ���ͻ��ˣ�"));
		return;
	}

	// ����m_listSocketChat �ҵ���ӦID��socket
	POSITION pos = m_listSocketChat.GetHeadPosition();
	CChatSocket *pChatSocket;
	int nFlag = 0; // 0��ʾ�����ڴ˿ͻ���
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
				//����ʧ��
				MessageBox(_T("����ʧ��"));
				return;
			}
			// �ڱ��ر༭���������Ϣ
			CString strTemp;
			strTemp.Format(_T("���û�%s��%s\r\n"), pChatSocket->ID, m_csSendMsg);
			m_csOutMsg += strTemp;
			m_csSendMsg = "";

			UpdateData(FALSE);
			break;
		}	
	}
	if(nFlag == 0) // �����ڴ�ID��socket
	{
		MessageBox(_T("�˿ͻ��˲����ڣ�"));
		return;	
	}

}

// ��������
void CSocketServerDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OpenServer();

}

// �����Ϣ
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


// ����ϵͳ��Ϣ
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
	// ����Է������� ���Ƚ���Ϣ������

	m_listSystemMsg.AddTail(msg_sys);

	return 0;
}


/*********************************************************
�������ƣ�OnDataDialog
���������������ݹ�����
����˵����msg-��Ϣ�ṹ�� pSocket-���յ���Ϣ��pSocket
�� �� ֵ��
��    ע��
*********************************************************/
void CSocketServerDlg::OnDataDialog()
{
	// TODO: �ڴ���������������
	if(m_pDataDlg == NULL)
	{
		m_pDataDlg = new CDataDlg(this);
		m_pDataDlg->Create(IDD_DATA_DLG);
		m_pDataDlg->ShowWindow(SW_SHOW);
		m_pDataDlg->RefreshListCtrlData(&m_data);
	}

}
