// MyWindowsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyWindowsDlg.h"
#include "afxdialogex.h"
#include "MyHead.h"

// MyWindowsDlg �Ի���
IMPLEMENT_DYNAMIC(MyWindowsDlg, CDialogEx)

MyWindowsDlg::MyWindowsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WINDOWS_DIALOG, pParent)
{
	Create(IDD_WINDOWS_DIALOG);
}

MyWindowsDlg::~MyWindowsDlg()
{
}

void MyWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WINDOWS, list_windows_);
}

BEGIN_MESSAGE_MAP(MyWindowsDlg, CDialogEx)
END_MESSAGE_MAP()
// MyWindowsDlg ��Ϣ�������

BOOL WINAPI WindowsProc(HWND nHwnd, LPARAM nLparam)
{
	ClistCtrlEx *m_List_Windows = (ClistCtrlEx*)nLparam;

	TCHAR buff[200];
	memset(buff, 0, sizeof(buff) / sizeof(buff[0]));
	GetWindowText(nHwnd, buff,_countof(buff));

	if (IsWindowVisible(nHwnd) && _tcslen(buff) && _tcscmp(buff,TEXT("��ʼ")))
	{
		DWORD nIndex = m_List_Windows->GetItemCount();
		DWORD nPid = 0;
		m_List_Windows->InsertItem(nIndex, buff);
		GetWindowThreadProcessId(nHwnd, &nPid);
		m_List_Windows->SetItemData(nIndex, nPid);
	}
	return true;
}

void MyWindowsDlg::PrintWindowsList()
{
	list_windows_.DeleteAllItems();

	EnumWindows(WindowsProc, (LPARAM)&list_windows_);

	static HIMAGELIST staic_tmp_nHiml = { 0 };//ͼ���б�
	static SHFILEINFO staic_temp_nPsfi = { 0 };//�ļ���Ϣ
									//ImageList_Destroy(nHiml);//���ͼ���б�
	static bool nOn = false;
	if (!nOn)
	{
		nOn = !nOn;
		staic_tmp_nHiml = ImageList_Create(24, 24, ILC_COLOR32, 0, 0);//����ͼ����
		ImageList_SetBkColor(staic_tmp_nHiml, list_windows_.GetBkColor());//����ͼ���б��ɫ
		list_windows_.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)staic_tmp_nHiml);//���ó����б���ʾͼ��
	}

	DWORD nListNum = list_windows_.GetItemCount();
	ImageList_SetImageCount(staic_tmp_nHiml, nListNum);

	for (DWORD i = 0; i < nListNum; i++)
	{
		CString nFileRoute = list_windows_.GetItemText(i, 0);
		DWORD_PTR nPid = list_windows_.GetItemData(i);
		HANDLE nHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)nPid);
		GetProcessFilePath(nHandle, nFileRoute);
		CloseHandle(nHandle);

		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//��ȡ�ļ���Ϣ
		if (!SHGetFileInfo(nFileRoute, nFileAttributes, &staic_temp_nPsfi, sizeof(SHFILEINFO), SHGFI_ICON)) {
			SHGetFileInfo(nFileRoute, nFileAttributes, &staic_temp_nPsfi, 
				sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		}

		ImageList_ReplaceIcon(staic_tmp_nHiml, i, staic_temp_nPsfi.hIcon);
		DestroyIcon(staic_temp_nPsfi.hIcon);

		list_windows_.SetItem(i, 0, 2, TEXT(""), i, 0, 0, 0);
	}
}

BOOL MyWindowsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	list_windows_.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	list_windows_.InsertColumn(0, TEXT("��ǰ����"), 0, 880);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	PrintWindowsList();

	return true;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
