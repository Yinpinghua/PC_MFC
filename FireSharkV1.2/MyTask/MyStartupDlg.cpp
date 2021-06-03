// MyStartupDlg.cpp : 实现文件

#include "stdafx.h"
#include "MyTask.h"
#include "MyHead.h"
#include "MyStartupDlg.h"
#include "afxdialogex.h"

// MyStartupDlg 对话框
IMPLEMENT_DYNAMIC(MyStartupDlg, CDialogEx)

MyStartupDlg::MyStartupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STARTUP_DIALOG, pParent)
{
	Create(IDD_STARTUP_DIALOG);
}

MyStartupDlg::~MyStartupDlg()
{
}

void MyStartupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STARTUP, list_startup_);
}

BEGIN_MESSAGE_MAP(MyStartupDlg, CDialogEx)
	ON_COMMAND(ID_DELETESTATTUP, &MyStartupDlg::on_delete_stattup)
	ON_COMMAND(ID_REFERSHSTATRUP, &MyStartupDlg::on_refersh_statrup)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_STARTUP, &MyStartupDlg::on_rclick_list_startup)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_STARTUP, &MyStartupDlg::on_keydown_list_startup)
END_MESSAGE_MAP()


// MyStartupDlg 消息处理程序
BOOL MyStartupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	menu_.LoadMenu(IDR_MENU8);
	SetMenu(&menu_);

	list_startup_.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	list_startup_.InsertColumn(0, TEXT("软件名称"), 0, 200);
	list_startup_.InsertColumn(1, TEXT("命令行"), 0, 600);
	list_startup_.InsertColumn(2, TEXT("注册表位置"), 0, 0);

	print_startup();

	return true;
}

void MyStartupDlg::print_startup()
{
	list_startup_.DeleteAllItems();

	static HIMAGELIST nHiml = { 0 };//图标列表
	static SHFILEINFO nPsfi = { 0 };//文件信息
									//ImageList_Destroy(nHiml);//清除图标列表
	static bool nOn = false;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(32, 32, ILC_COLOR32, 0, 0);//订制图标框架
		ImageList_SetBkColor(nHiml, list_startup_.GetBkColor());//设置图标列表底色
		list_startup_.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//设置超级列表显示图标
	}

	TCHAR szValueName[MAXBYTE] = { 0 };
	TCHAR szValueKey[MAXBYTE] = { 0 };

	TCHAR *reg_run[2] = { L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run" };
	HKEY HKEY_RUN[2] = { HKEY_CURRENT_USER ,HKEY_LOCAL_MACHINE };

	HKEY hKey = NULL;
	LONG nError;
	CString nPath;

	for (DWORD i = 0; i < _countof(reg_run); i++)
	{
		nError = RegOpenKey(HKEY_RUN[i], reg_run[i], &hKey);

		if (nError != ERROR_SUCCESS) {
			MessageBox(TEXT("打开注册表失败"), TEXT("提示"), MB_ICONWARNING);
			return;
		}

		DWORD nIndex = 0;
		DWORD dwBufferSize = MAXBYTE;
		while (true)
		{
			nError = RegEnumValue(hKey, nIndex++, szValueKey,
				&dwBufferSize, 0, 0, 0, &dwBufferSize);

			if (nError == ERROR_NO_MORE_ITEMS || nError != 0) {
				break;
			}

			RegQueryValueEx(hKey, szValueKey, 0, 0, (LPBYTE)szValueName, &dwBufferSize);

			list_startup_.InsertItem(list_startup_.GetItemCount(), szValueKey);
			list_startup_.SetItemText(list_startup_.GetItemCount() - 1, 1,get_path_ex(szValueName));
			list_startup_.SetItemText(list_startup_.GetItemCount() - 1, 2, reg_run[i]);
			list_startup_.SetItemData(list_startup_.GetItemCount() - 1, i);

			dwBufferSize = MAXBYTE;
		}

		RegCloseKey(hKey);
	}

	DWORD nListNum = list_startup_.GetItemCount();

	ImageList_SetImageCount(nHiml, nListNum);

	for (DWORD i = 0; i < nListNum; i++)
	{
		nPath = get_path_ex(list_startup_.GetItemText(i, 1));

		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//获取文件信息
		if (!SHGetFileInfo(nPath, nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON)) {
			SHGetFileInfo(nPath, nFileAttributes, &nPsfi, 
				sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		}

		//DWORD nIco = ImageList_AddIcon(nHiml, nPsfi.hIcon);//置入图标返回图标下标
		ImageList_ReplaceIcon(nHiml, i, nPsfi.hIcon);
		DestroyIcon(nPsfi.hIcon);

		list_startup_.SetItemImageEx(i, 0, list_startup_.GetItemText(i, 0), i);
	}
}

//关闭开启启动项
void MyStartupDlg::on_delete_stattup()
{
	HKEY hkey_run[2] = { HKEY_CURRENT_USER ,HKEY_LOCAL_MACHINE };
	HKEY hKey = NULL;
	CString nHkeyPath;
	CString nValueKey;
		
	DWORD nIndex = list_startup_.GetItemData(list_startup_.GetSelectionEx());
	nValueKey = list_startup_.GetItemText(list_startup_.GetSelectionEx(), 0);
	nHkeyPath = list_startup_.GetItemText(list_startup_.GetSelectionEx(), 2);

	LONG nError = RegOpenKey(hkey_run[nIndex], nHkeyPath, &hKey);
	if (nError != ERROR_SUCCESS) {
		MessageBox(TEXT("打开注册表失败"), TEXT("提示"), MB_ICONWARNING);
		return;
	}

	//删掉选中的该项的键和值
	RegDeleteValue(hKey, nValueKey);
	RegCloseKey(hKey);
	print_startup();
}

void MyStartupDlg::on_refersh_statrup()
{
	print_startup();
}

void MyStartupDlg::on_rclick_list_startup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate == nullptr){
		return;
	}

	*pResult = 0;

	CMenu *nMenu = menu_.GetSubMenu(0);
	POINT pos;
	GetCursorPos(&pos);

	if (nMenu == nullptr){
		return;
	}

	if (pNMItemActivate->iItem == -1) {
		nMenu->EnableMenuItem(ID_DELETESTATTUP,true);
	}else {
		nMenu->EnableMenuItem(ID_DELETESTATTUP,false);
	}

	nMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);
}

void MyStartupDlg::on_keydown_list_startup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	*pResult = 0;

	if (pLVKeyDow->wVKey != 116) {
		return;
	}

	print_startup();
}
