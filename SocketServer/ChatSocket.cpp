// ChatSocket.cpp : ʵ���ļ�
//


#include "stdafx.h"

#include "SocketServer.h"

#include "SocketServerDlg.h" //�����������������֮��include

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


// CChatSocket ��Ա����


void CChatSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_tmLastMsg = CTime::GetTickCount();
	((CSocketServerDlg*)m_pParentWnd)->RecvMsg(this);
	

	CSocket::OnReceive(nErrorCode);
}
