#pragma once

// CListenSocket ÃüÁîÄ¿±ê

class CListenSocket : public CSocket
{
public:
	CListenSocket(CWnd *pWnd);
	virtual ~CListenSocket();
	virtual void OnAccept(int nErrorCode);

protected:
	CWnd* m_pParentWnd;
};


