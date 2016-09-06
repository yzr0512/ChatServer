/************************************************
文 件 名：DataDlg.cpp
作    者：余志荣
创建日期：2016-08-29
用    途：账号管理窗口的实现文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

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
	m_pParentWnd(pParent), m_pData(NULL)
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
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CDataDlg 消息处理程序


BOOL CDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RefreshCtrlPosition();

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

/*********************************************************
函数名称：RefreshListCtrlData
功能描述：刷新列表控件的内容
作    者：余志荣
创建时间：2016-08-29
*********************************************************/
void CDataDlg::RefreshListCtrlData()
{
	m_pData->ShowData(&m_lstctlData);

	UpdateData(FALSE);
}

/*********************************************************
函数名称：OnBnClickedBtSaveData
功能描述：保存账号数据
作    者：余志荣
创建时间：2016-08-29
*********************************************************/
void CDataDlg::OnBnClickedBtSaveData()
{
	((CSocketServerDlg*)m_pParentWnd)->m_data.SaveData();
}

/*********************************************************
函数名称：OnBnClickedBtLoadData
功能描述：加载账号数据
作    者：余志荣
创建时间：2016-08-29
*********************************************************/
void CDataDlg::OnBnClickedBtLoadData()
{
	((CSocketServerDlg*)m_pParentWnd)->m_data.LoadData();
	((CSocketServerDlg*)m_pParentWnd)->RefreshListCtrlData();
}

// 改变窗口尺寸
void CDataDlg::OnSize(UINT nType, int cx, int cy)
{

	CDialogEx::OnSize(nType, cx, cy);
	
	if(RefreshCtrlPosition())
		RefreshListCtrlData();
	
}

// 重设控件位置
int CDataDlg::RefreshCtrlPosition(void)
{
	if(m_lstctlData.GetSafeHwnd() == NULL)
		return 0;
	CRect rcClient;
	GetClientRect(&rcClient);
	CPoint pot = rcClient.TopLeft();
	m_lstctlData.SetWindowPos(NULL, pot.x + 3, pot.y +3, 
		rcClient.Width() - 6, rcClient.Height() - 63, NULL);
	
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_BT_ADD_USER);
	pWnd->SetWindowPos(NULL, pot.x + 10, rcClient.Height() - 40, 
		0, 0, SWP_NOZORDER | SWP_NOSIZE);
	pWnd = GetDlgItem( IDC_BT_DEL_USER );
	pWnd->SetWindowPos(NULL, pot.x + 110, rcClient.Height() - 40, 
		0, 0, SWP_NOZORDER | SWP_NOSIZE);
	pWnd = GetDlgItem( IDC_BT_SAVE_DATA );
	pWnd->SetWindowPos(NULL, pot.x + 210, rcClient.Height() - 40, 
		0, 0, SWP_NOZORDER | SWP_NOSIZE);
	pWnd = GetDlgItem( IDC_BT_LOAD_DATA );
	pWnd->SetWindowPos(NULL, pot.x + 310, rcClient.Height() - 40, 
		0, 0, SWP_NOZORDER | SWP_NOSIZE);


	return 1;
}

/*********************************************************
函数名称：SetData
功能描述：设置数据
作    者：余志荣
创建时间：2016-08-29
参数说明：pData -- 数据的指针
*********************************************************/
void CDataDlg::SetData(CData* pData)
{
	ASSERT(pData);
	m_pData = pData;
}

// 设置窗口的最小尺寸
void CDataDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{	
	//调整最小高度与宽度
	lpMMI->ptMinTrackSize.x = 740;
	lpMMI->ptMinTrackSize.y = 497;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
