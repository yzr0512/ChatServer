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


void CDataDlg::RefreshListCtrlData()
{
	m_pData->ShowData(&m_lstctlData);

	UpdateData(FALSE);
}


void CDataDlg::OnBnClickedBtSaveData()
{
	((CSocketServerDlg*)m_pParentWnd)->m_data.SaveData();
}


void CDataDlg::OnBnClickedBtLoadData()
{
	((CSocketServerDlg*)m_pParentWnd)->m_data.LoadData();
	((CSocketServerDlg*)m_pParentWnd)->RefreshListCtrlData();
}


void CDataDlg::OnSize(UINT nType, int cx, int cy)
{

	CDialogEx::OnSize(nType, cx, cy);
	
	if(RefreshCtrlPosition())
		RefreshListCtrlData();
	

	// TODO: ÔÚ´Ë´¦Ìí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂë
}


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


void CDataDlg::SetData(CData* pData)
{
	ASSERT(pData);
	m_pData = pData;
}


void CDataDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
	
	//调整最小高度与宽度
	lpMMI->ptMinTrackSize.x = 740;
	lpMMI->ptMinTrackSize.y = 497;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
