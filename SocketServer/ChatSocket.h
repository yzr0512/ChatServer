#pragma once


// CChatSocket 命令目标

class CChatSocket : public CSocket
{
public:
	CChatSocket(CWnd *pWnd);
	virtual ~CChatSocket();
	
	virtual void OnReceive(int nErrorCode);

	CString ID;
	char m_userID[ID_MAX];

	CTime m_tmLastMsg; // 最后通信时间

protected:
	CWnd *m_pParentWnd;

};


