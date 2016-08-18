// DataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketServer.h"
#include "DataDlg.h"
#include "afxdialogex.h"
#include "SocketServerDlg.h"

// CDataDlg 对话框

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
	ON_BN_CLICKED(IDC_BT_SAVE_DATA, &CDataDlg::OnBnClickedBtSaveData)
	ON_BN_CLICKED(IDC_BT_LOAD_DATA, &CDataDlg::OnBnClickedBtLoadData)
END_MESSAGE_MAP()


// CDataDlg 消息处理程序


BOOL CDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDataDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	((CSocketServerDlg *)m_pParentWnd)->m_pDataDlg = NULL;
	DestroyWindow();

	//CDialogEx::OnCancel();
}
void CDataDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	delete this;

	CDialogEx::PostNcDestroy();

}


void CDataDlg::RefreshListCtrlData(const CData * pData)
{
	pData->ShowData(&m_lstctlData);

	UpdateData(FALSE);


}


void CDataDlg::OnBnClickedBtSaveData()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	((CSocketServerDlg*)m_pParentWnd)->m_data.SaveData();
}


void CDataDlg::OnBnClickedBtLoadData()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	((CSocketServerDlg*)m_pParentWnd)->m_data.LoadData();
	((CSocketServerDlg*)m_pParentWnd)->RefreshListCtrlData();
}
