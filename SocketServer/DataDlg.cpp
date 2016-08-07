// DataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketServer.h"
#include "DataDlg.h"
#include "afxdialogex.h"
#include "SocketServerDlg.h"

// CDataDlg �Ի���

IMPLEMENT_DYNAMIC(CDataDlg, CDialogEx)

CDataDlg::CDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataDlg::IDD, pParent),
	m_pParentWnd(pParent)
{

}

CDataDlg::~CDataDlg()
{
}

void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_lstctlData);
}


BEGIN_MESSAGE_MAP(CDataDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataDlg ��Ϣ�������


BOOL CDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDataDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	delete this;

	CDialogEx::PostNcDestroy();

}

void CDataDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	((CSocketServerDlg *)m_pParentWnd)->m_pDataDlg = NULL;
	DestroyWindow();

	//CDialogEx::OnCancel();
}

void CDataDlg::RefreshListCtrlData(const CData * pData)
{
	pData->ShowData(&m_lstctlData);

	UpdateData(FALSE);

}

