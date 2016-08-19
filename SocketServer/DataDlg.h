

#pragma once

#include "Data.h"


// CDataDlg 对话框

class CDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataDlg)

public:
	CDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataDlg();

// 对话框数据
	enum { IDD = IDD_DATA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual void OnCancel();
	afx_msg void OnBnClickedBtSaveData();
	afx_msg void OnBnClickedBtLoadData();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	void RefreshListCtrlData();
	void SetData(CData* pData);
	int RefreshCtrlPosition(void);
	
	CListCtrl m_lstctlData;

protected:
	CWnd* m_pParentWnd; // 主窗口
	CData* m_pData;

public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};

