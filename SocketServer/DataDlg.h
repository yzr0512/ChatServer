

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

	void RefreshListCtrlData(const CData * pData);

protected:
	CWnd* m_pParentWnd; // ������

public:
	CListCtrl m_lstctlData;

};

