
// SocketServerDlg.h : ͷ�ļ�
//
#pragma once

#include "ListenSocket.h"
#include "ChatSocket.h"
#include "Data.h"
#include "DataDlg.h"


// CSocketServerDlg �Ի���
class CSocketServerDlg : public CDialogEx
{
// ����
public:
	CSocketServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SOCKETSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	CListenSocket *m_pSocketListen; //���ڼ���
	
	
	CList<CChatSocket *, CChatSocket *> m_listSocketChat; // ������û�ͨ�ŵ�Socket������
	CList<struct MSG_TRANSPOND*, struct MSG_TRANSPOND*> m_listChatMsg; // ���δ������Ϣ������
	CList<struct MSG_SYS*, struct MSG_SYS*> m_listSystemMsg; // ���δ���͵�ϵͳ��Ϣ

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int OpenServer(void); // ��ʼ��������
	int AcceptClient(void); // ������������
	int RecvMsg(CChatSocket * pSocket); // ������Ϣ
	
	int Login(struct MSG_LOGIN * msg, CChatSocket * pSocket); // ��¼
	int LoginOut(struct MSG_LOGIN * msg, CChatSocket * pSocket); // ����

	int Register(struct MSG_REGISTER * msg, CChatSocket * pSocket); // ע��

	int RelayChatMsg(struct MSG_TRANSPOND * msg, CChatSocket * pSocket); // ת��������Ϣ
	int AddFriend(struct MSG_TRANSPOND * msg, CChatSocket * pSocket); // ��Ӻ���
	int DeleteFriend(struct MSG_TRANSPOND * msg, CChatSocket * pSocket); // ɾ������

	int GetFriendInfo(struct MSG_USERINFO * msg, CChatSocket * pSocket); // ��ú�����Ϣ
	int GetFriendList(struct MSG_USERINFO * msg, CChatSocket * pSocket); // ��ú����б�
	int GetStrangerInfo(struct MSG_USERINFO * msg, CChatSocket * pSocket); // ���İ������Ϣ
	int SetUserStatus(struct MSG_USERINFO * msg, CChatSocket * pSocket); // ��������״̬
	int GetAllFriendInfo(struct MSG_FRND_INFO* msg_info, CChatSocket* pSocket);

	CString m_csOutMsg;
	CString m_csSendMsg;
	CListCtrl m_lstClient;
	CData m_data; // �˺�����
	CDataDlg *m_pDataDlg; // ���ݹ�����

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


