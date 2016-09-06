

/************************************************
文 件 名：ChatSocket.cpp
作    者：余志荣
创建日期：2016-08-29
用    途：通信用的Socket的实现文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

// ChatSocket.cpp : 实现文件
//


#include "stdafx.h"

#include "SocketServer.h"

#include "SocketServerDlg.h" //这个必须在上面两个之后include

#include "ChatSocket.h"



// CChatSocket

CChatSocket::CChatSocket(CWnd *pWnd)
{
	m_pParentWnd = pWnd;
	m_tmLastMsg = CTime::GetTickCount();
}

CChatSocket::~CChatSocket()
{
}


// CChatSocket 成员函数


void CChatSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_tmLastMsg = CTime::GetTickCount();
	((CSocketServerDlg*)m_pParentWnd)->RecvMsg(this);
	

	CSocket::OnReceive(nErrorCode);
}


void CChatSocket::OnClose(int nErrorCode)
{

	((CSocketServerDlg*)m_pParentWnd)->RemoveSocketFromList(this);

	CSocket::OnClose(nErrorCode);
}
