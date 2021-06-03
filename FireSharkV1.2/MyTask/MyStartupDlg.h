#pragma once
#include "ClistCtrlEx.h"
#include "afxcmn.h"


// MyStartupDlg 对话框

class MyStartupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyStartupDlg)

public:
	MyStartupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyStartupDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STARTUP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMenu menu_;
	virtual BOOL OnInitDialog();
	ClistCtrlEx list_startup_;
	void print_startup();
	afx_msg void on_delete_stattup();
	afx_msg void on_refersh_statrup();
	afx_msg void on_rclick_list_startup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void on_keydown_list_startup(NMHDR *pNMHDR, LRESULT *pResult);
};
