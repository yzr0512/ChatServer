
/************************************************
文 件 名：ListenSocket.h
作    者：余志荣
创建日期：2016-08-29
用    途：监听Socket的头文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

#pragma once

// CListenSocket 命令目标

class CListenSocket : public CSocket
{
public:
	CListenSocket(CWnd *pWnd);
	virtual ~CListenSocket();
	virtual void OnAccept(int nErrorCode);

protected:
	CWnd* m_pParentWnd;
};


