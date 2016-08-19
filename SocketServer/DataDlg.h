

#pragma once

#include "Data.h"


// CDataDlg �Ի���

class CDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataDlg)

public:
	CDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataDlg();

// �Ի�������
	enum { IDD = IDD_DATA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	CWnd* m_pParentWnd; // ������
	CData* m_pData;

public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};

