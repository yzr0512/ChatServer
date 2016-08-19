
// SocketServerDlg.h : 头文件
//
#pragma once

#include "ListenSocket.h"
#include "ChatSocket.h"
#include "Data.h"
#include "DataDlg.h"


// CSocketServerDlg 对话框
class CSocketServerDlg : public CDialogEx
{
// 构造
public:
	CSocketServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SOCKETSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CListenSocket *m_pSocketListen; //用于监听
	
	
	CList<CChatSocket *, CChatSocket *> m_listSocketChat; // 存放与用户通信的Socket的链表
	CList<struct MSG_TRANSPOND*, struct MSG_TRANSPOND*> m_listChatMsg; // 存放未接收消息的链表
	CList<struct MSG_SYS*, struct MSG_SYS*> m_listSystemMsg; // 存放未发送的系统消息

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int OpenServer(void); // 初始化服务器
	int AcceptClient(void); // 接受连接请求
	int RecvMsg(CChatSocket * pSocket); // 接收消息
	
	int Login(struct MSG_LOGIN * msg, CChatSocket * pSocket); // 登录
	int LoginOut(struct MSG_LOGIN * msg, CChatSocket * pSocket); // 下线

	int Register(struct MSG_REGISTER * msg, CChatSocket * pSocket); // 注册

	int RelayChatMsg(struct MSG_TRANSPOND * msg, CChatSocket * pSocket); // 转发聊天消息
	int AddFriend(struct MSG_TRANSPOND * msg, CChatSocket * pSocket); // 添加好友
	int DeleteFriend(struct MSG_TRANSPOND * msg, CChatSocket * pSocket); // 删除好友

	int GetFriendInfo(struct MSG_USERINFO * msg, CChatSocket * pSocket); // 获得好友信息
	int GetFriendList(struct MSG_USERINFO * msg, CChatSocket * pSocket); // 获得好友列表
	int GetStrangerInfo(struct MSG_USERINFO * msg, CChatSocket * pSocket); // 获得陌生人信息
	int SetUserStatus(struct MSG_USERINFO * msg, CChatSocket * pSocket); // 设置在线状态
	int GetAllFriendInfo(struct MSG_FRND_INFO* msg_info, CChatSocket* pSocket);

	CString m_csOutMsg;
	CString m_csSendMsg;
	CListCtrl m_lstClient;
	CData m_data; // 账号数据
	CDataDlg *m_pDataDlg; // 数据管理窗口

public:
	int RefreshListCtrlData(void);

	CString m_csID;
	
	afx_msg void OnLvnItemchangedListClient(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonOpen();
	
	int OutputInfo(const CString& csOutMsg);
	int OutputInfo(UINT nIDPrompt);

	int SendSystemMsg(struct MSG_SYS * msg);
	afx_msg void OnDataDialog();

//	afx_msg void OnBnClickedBtTest();
	afx_msg void OnBnClickedBtTest();
	afx_msg void OnClose();
};


