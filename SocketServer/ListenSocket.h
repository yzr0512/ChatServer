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


