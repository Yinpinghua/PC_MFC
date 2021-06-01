// MyProcessLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyProcessLibDlg.h"
#include "afxdialogex.h"
#include "MyHead.h"
#include "MD5_FILE.h"


// MyProcessLibDlg 对话框

IMPLEMENT_DYNAMIC(MyProcessLibDlg, CDialogEx)

MyProcessLibDlg::MyProcessLibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESSLIB_DIALOG, pParent)
{

}

MyProcessLibDlg::~MyProcessLibDlg()
{
}

void MyProcessLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, list_process_);
	DDX_Control(pDX, IDC_LIST_LIB, list_lib_);
	DDX_Control(pDX, IDC_BUTTON1, btn_add_all_);
}


BEGIN_MESSAGE_MAP(MyProcessLibDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROCESS, &MyProcessLibDlg::OnDblclkListProcess)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LIB, &MyProcessLibDlg::OnDblclkListLib)
	ON_BN_CLICKED(IDC_BUTTON1, &MyProcessLibDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &MyProcessLibDlg::OnBnClickedBtn7)
END_MESSAGE_MAP()


// MyProcessLibDlg 消息处理程序


BOOL MyProcessLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	list_process_.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	list_lib_.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	list_process_.InsertColumn(0, TEXT("进程名"), 0, 100);
	list_process_.InsertColumn(1, TEXT("进程路径"), 0, 100);
	//m_List_Process.InsertColumn(2, TEXT("文件MD5"), 0, 100);

	list_lib_.InsertColumn(0, TEXT("进程名"), 0, 100);
	list_lib_.InsertColumn(1, TEXT("文件MD5"), 0, 100);

	TASKLIST nTaskList = {};
	DWORD nNum = EnumTaskList(nTaskList);//枚举进程列表
	CStringA nFileMd5;
	DWORD nIndex;

	for (DWORD i = 0; i < nNum; i++){
		if (nTaskList.szExeFilePath[i].IsEmpty()) {
			continue;
		}

		nIndex = list_process_.GetItemCount();
		list_process_.InsertItem(nIndex, nTaskList.szExeFile[i], 0);
		list_process_.SetItemText(nIndex, 1, nTaskList.szExeFilePath[i]);
	}

	ClearTaskList(nTaskList);
	PrintProcessList();

	return true;
}

void MyProcessLibDlg::PrintProcessList()
{
	list_lib_.DeleteAllItems();

	load_virus_lib nLoadProcessLib;
	DWORD nCount = nLoadProcessLib.LoadProcessLibData();
	PVIRUSINFO m_VirusInfo = new VIRUSINFO[nCount];
	nLoadProcessLib.GetProcessLib(m_VirusInfo);
	nLoadProcessLib.ClearProcessLibData();


	for (DWORD i = 0; i < nCount; i++)
	{
		list_lib_.InsertItem(i, CString(m_VirusInfo[i].FileName), 0);
		list_lib_.SetItemText(i, 1, CString(m_VirusInfo[i].FileMd5));
	}

	delete[]m_VirusInfo;
	m_VirusInfo = nullptr;
}

void MyProcessLibDlg::OnDblclkListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	if (pNMItemActivate->iItem == -1) {
		return;
	}

	CString nFileName;
	CString nFilePath;
	CString nFileMd5;

	nFileName = list_process_.GetItemText(pNMItemActivate->iItem, 0);
	nFilePath = list_process_.GetItemText(pNMItemActivate->iItem, 1);

	nFileMd5 = get_file_md5(nFilePath);
	if (nFileMd5.IsEmpty()) {
		return;
	}

	CString nLibName;
	CString nLibMd5;

	size_t data_count = list_lib_.GetItemCount();
	for (size_t i = 0 ; i < data_count;i++)
	{
		nLibName = list_lib_.GetItemText(i, 0);
		nLibMd5 = list_lib_.GetItemText(i, 1);

		if (strcmp(CStringA(nFileName), CStringA(nLibName)) == 0){
			return;
		}
	}

	list_lib_.InsertItem(list_lib_.GetItemCount(), nFileName);
	list_lib_.SetItemText(list_lib_.GetItemCount() - 1, 1, nFileMd5);

	UpDataProcessLib();
}

void MyProcessLibDlg::UpDataProcessLib()
{
	DWORD nCount = list_lib_.GetItemCount();
	PVIRUSINFO nVirusInfo = new VIRUSINFO[nCount];
	for (DWORD i = 0; i < nCount; i++){
		strcpy_s(nVirusInfo[i].FileName, MAX_PATH, CStringA(list_lib_.GetItemText(i, 0)));
		strcpy_s(nVirusInfo[i].FileMd5, MAX_PATH, CStringA(list_lib_.GetItemText(i, 1)));
	}

	load_virus_lib nLoadVirusLib;
	nLoadVirusLib.SetProcessLib(nVirusInfo, nCount);

	delete[]nVirusInfo;
	nVirusInfo = nullptr;
}

void MyProcessLibDlg::OnDblclkListLib(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	if (pNMItemActivate == nullptr){
		return;
	}

	if (pNMItemActivate->iItem == -1) {
		return;
	}

	list_lib_.DeleteItem(pNMItemActivate->iItem);

	UpDataProcessLib();
}

void MyProcessLibDlg::OnBnClickedButton1()
{
	btn_add_all_.EnableWindow(FALSE);
	CString file_name;
	CString nFilePath;
	CString nFileMd5;
	bool nHave = false;

	DWORD nCount = list_process_.GetItemCount();
	for (DWORD i = 0; i < nCount; i++)
	{
		file_name = list_process_.GetItemText(i, 0);
		nFilePath = list_process_.GetItemText(i, 1);

		nFileMd5 = get_file_md5(nFilePath);
		if (nFileMd5.IsEmpty())continue;;

		CString nLibName;
		CString nLibMd5;
		nHave = false;

		for (INT x = 0; x < list_lib_.GetItemCount(); x++)
		{
			nLibName = list_lib_.GetItemText(x, 0);
			nLibMd5 = list_lib_.GetItemText(x, 1);

			if (strcmp(CStringA(file_name), CStringA(nLibName)) == 0)
			{
				nHave = true;
				break;
			}
		}

		if (!nHave)
		{
			list_lib_.InsertItem(list_lib_.GetItemCount(), file_name);
			list_lib_.SetItemText(list_lib_.GetItemCount() - 1, 1, nFileMd5);
		}
	}

	UpDataProcessLib();
	btn_add_all_.EnableWindow(true);
}


void MyProcessLibDlg::OnBnClickedBtn7()
{
	list_lib_.DeleteAllItems();
	UpDataProcessLib();
}
