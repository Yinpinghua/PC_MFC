#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"


// MyMoudleDlg �Ի���

class MyMoudleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyMoudleDlg)

public:
	MyMoudleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyMoudleDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYMOUDLEDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Module;
	afx_msg void on_dblclk_list_module(NMHDR *pNMHDR, LRESULT *pResult);
};
