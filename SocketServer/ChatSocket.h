
/************************************************
�� �� ����ChatSocket.h
��    �ߣ���־��
�������ڣ�2016-08-29
��    ;��ͨ���õ�Socket��ͷ�ļ�
�޸ļ�¼��2016-08-29 ��־�� ����
************************************************/ 

#pragma once


// CChatSocket ����Ŀ��

class CChatSocket : public CSocket
{
public:
	CChatSocket(CWnd *pWnd);
	virtual ~CChatSocket();
	
	virtual void OnReceive(int nErrorCode);

	CString ID;
	char m_userID[ID_MAX];

	CTime m_tmLastMsg; // ���ͨ��ʱ��

protected:
	CWnd *m_pParentWnd;

public:
	virtual void OnClose(int nErrorCode);
};


