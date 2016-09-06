

/************************************************
文 件 名：ListenSocket.cpp
作    者：余志荣
创建日期：2016-08-29
用    途：监听Socket的实现文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

// ListenSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketServer.h"
#include "ListenSocket.h"
#include "SocketServerDlg.h"

// CListenSocket

CListenSocket::CListenSocket(CWnd *pWnd)
{
	m_pParentWnd = pWnd;
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket 成员函数


void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	((CSocketServerDlg*)m_pParentWnd)->AcceptClient();
		
	CSocket::OnAccept(nErrorCode);


}
