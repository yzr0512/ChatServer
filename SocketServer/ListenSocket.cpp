

/************************************************
�� �� ����ListenSocket.cpp
��    �ߣ���־��
�������ڣ�2016-08-29
��    ;������Socket��ʵ���ļ�
�޸ļ�¼��2016-08-29 ��־�� ����
************************************************/ 

// ListenSocket.cpp : ʵ���ļ�
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


// CListenSocket ��Ա����


void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	((CSocketServerDlg*)m_pParentWnd)->AcceptClient();
		
	CSocket::OnAccept(nErrorCode);


}
