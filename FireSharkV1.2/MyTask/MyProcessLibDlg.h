#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"
#include "load_virus_lib.h"
#include "afxwin.h"


// MyProcessLibDlg 对话框

class MyProcessLibDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyProcessLibDlg)

public:
	MyProcessLibDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyProcessLibDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSLIB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx list_process_;
	ClistCtrlEx list_lib_;
	afx_msg void OnDblclkListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	void UpDataProcessLib();
	void PrintProcessList();
	afx_msg void OnDblclkListLib(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtn7();
	CButton btn_add_all_;
};
