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
}

CChatSocket::~CChatSocket()
{
}


// CChatSocket 成员函数


void CChatSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	((CSocketServerDlg*)m_pParentWnd)->RecvMsg(this);


	CSocket::OnReceive(nErrorCode);
}
