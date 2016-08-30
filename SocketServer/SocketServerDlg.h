
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
	virtual BOOL OnInitDialog(); // 初始化窗口
	afx_msg void OnPaint(); // 绘制窗口
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

	CString m_csOutMsg; // 编辑框
	CString m_csSendMsg; // 要发送的消息
	CListCtrl m_lstClient; // 客户端列表控件

	CData m_data; // 账号数据
	CDataDlg *m_pDataDlg; // 数据管理窗口

public:
	int RefreshListCtrlData(void);

	CString m_csID;
	
	afx_msg void OnLvnItemchangedListClient(NMHDR *pNMHDR, LRESULT *pResult); // 列表选中项改变
	afx_msg void OnBnClickedButtonSend(); // 发送消息
	afx_msg void OnBnClickedButtonOpen(); // 打开服务器
	
	int OutputInfo(const CString& csOutMsg); // 在编辑框输出信息
	int OutputInfo(UINT nIDPrompt); // 在编辑框输出信息

	int SendSystemMsg(struct MSG_SYS * msg); // 发送系统消息
	afx_msg void OnDataDialog(); // 打开账号数据窗口

	afx_msg void OnBnClickedBtTest(); // 测试用
	afx_msg void OnClose(); // 关闭窗口时执行
	int CheckSocketStatus(void); // 检查每个客户端的连接状态
	afx_msg void OnTimer(UINT_PTR nIDEvent); // 定时器
	int FileTransRequest(MSG_FILE_REQUEST *msg_req); // 转发文件传输请求
	int FileTransAnswer(MSG_FILE_REQUEST *msg_ans); // 转发文件传输回应
	int SendToID(char* cID, void* msg, int nMsgSize); // 发送消息给制定ID
};


