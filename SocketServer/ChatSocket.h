#pragma once


// CChatSocket ÃüÁîÄ¿±ê

class CChatSocket : public CSocket
{
public:
	CChatSocket(CWnd *pWnd);
	virtual ~CChatSocket();
	
	virtual void OnReceive(int nErrorCode);

	CString ID;
	char m_userID[ID_MAX];

protected:
	CWnd *m_pParentWnd;

};


