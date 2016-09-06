
/************************************************
文 件 名：ChatSocket.h
作    者：余志荣
创建日期：2016-08-29
用    途：通信用的Socket的头文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

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

public:
	virtual void OnClose(int nErrorCode);
};


