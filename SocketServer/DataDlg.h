/************************************************
文 件 名：DataDlg.h
作    者：余志荣
创建日期：2016-08-29
用    途：账号管理对话框的头文件
修改记录：2016-08-29 余志荣 创建
************************************************/ 

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
	
	void RefreshListCtrlData(); // 刷新数据
	void SetData(CData* pData); // 设置数据
	int RefreshCtrlPosition(void); // 重设控件位置
	
	CListCtrl m_lstctlData;

protected:
	CWnd* m_pParentWnd; // 主窗口
	CData* m_pData;

public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};

