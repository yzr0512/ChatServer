/************************************************
�� �� ����SocketServerDlg.h
��    �ߣ���־��
�������ڣ�2016-08-03
��    ;�������ڵ�ͷ�ļ�
�޸ļ�¼��2016-08-03 ��־�� ����
************************************************/ 

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
	
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog(); // ��ʼ������
	afx_msg void OnPaint(); // ���ƴ���
	afx_msg HCURSOR OnQueryDragIcon(); 
	afx_msg void OnLvnItemchangedListClient(NMHDR *pNMHDR, LRESULT *pResult); // �б�ѡ����ı�
	afx_msg void OnBnClickedButtonSend(); // ������Ϣ
	afx_msg void OnBnClickedButtonOpen(); // �򿪷�����	
	afx_msg void OnBnClickedBtTest(); // ������
	afx_msg void OnClose(); // �رմ���ʱִ��
	afx_msg void OnDataDialog(); // ���˺����ݴ���
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent); // ��ʱ��

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
	int DeleteFriend(MSG_MAKE_FRIEND * msg, CChatSocket * pSocket); // ɾ������
	int GetFriendInfo(struct MSG_USERINFO * msg, CChatSocket * pSocket); // ��ú�����Ϣ
	int GetFriendList(struct MSG_USERINFO * msg, CChatSocket * pSocket); // ��ú����б�
	int GetStrangerInfo(struct MSG_USERINFO * msg, CChatSocket * pSocket); // ���İ������Ϣ
	int SetUserStatus(struct MSG_USERINFO * msg, CChatSocket * pSocket); // ��������״̬
	int GetAllFriendInfo(struct MSG_FRND_INFO* msg_info, CChatSocket* pSocket);
	int RefreshListCtrlData(void); // ˢ�������б�ؼ�������
	int OutputInfo(const CString& csOutMsg); // �ڱ༭�������Ϣ
	int OutputInfo(UINT nIDPrompt); // �ڱ༭�������Ϣ
	int SendSystemMsg(struct MSG_SYS * msg); // ����ϵͳ��Ϣ
	int CheckSocketStatus(void); // ���ÿ���ͻ��˵�����״̬
	int FileTransRequest(MSG_FILE_REQUEST *msg_req); // ת���ļ���������
	int FileTransAnswer(MSG_FILE_REQUEST *msg_ans); // ת���ļ������Ӧ
	int SendToID(char* cID, void* msg, int nMsgSize); // ������Ϣ���ƶ�ID
	int AddToFileSocketList(CChatSocket* pFileSocket); // ��socket��ӵ��ļ�����socket�б�
	int FileTransRelay(MSG_FILE_CONTENT* msg_file_content); // �ļ�����ת��
	int FileTransRelay(MSG_FILE_REQUEST* msg_file); // 	
	int FileTransOpera(MSG_FILE_OPERA* msg_opera, CChatSocket *pSocket);
	int CloseServer(void); // �رշ�����
	BOOL RemoveSocketFromList(CChatSocket* pSocket); // ���б���ɾ��socket
	

	CEdit m_EditOut; // �༭��ؼ�����
	CString m_csOutMsg; // �����Ϣ�༭��
	
	CData m_data; // �˺�����
	CDataDlg *m_pDataDlg; // ���ݹ�����

protected:
	CListenSocket *m_pSocketListen; //���ڼ���
	
	CList<CChatSocket*, CChatSocket*> m_FileSocketList;	// ����û������ļ��õ�Socket������
	CList<CChatSocket *, CChatSocket *> m_listSocketChat; // ������û�ͨ�ŵ�Socket������
	CList<struct MSG_TRANSPOND*, struct MSG_TRANSPOND*> m_listChatMsg; // ���δ������Ϣ������
	CList<struct MSG_SYS*, struct MSG_SYS*> m_listSystemMsg; // ���δ���͵�ϵͳ��Ϣ

};


