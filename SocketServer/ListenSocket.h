
/************************************************
�� �� ����ListenSocket.h
��    �ߣ���־��
�������ڣ�2016-08-29
��    ;������Socket��ͷ�ļ�
�޸ļ�¼��2016-08-29 ��־�� ����
************************************************/ 

#pragma once

// CListenSocket ����Ŀ��

class CListenSocket : public CSocket
{
public:
	CListenSocket(CWnd *pWnd);
	virtual ~CListenSocket();
	virtual void OnAccept(int nErrorCode);

protected:
	CWnd* m_pParentWnd;
};


