// MyStartupDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "MyTask.h"
#include "MyHead.h"
#include "MyStartupDlg.h"
#include "afxdialogex.h"

// MyStartupDlg �Ի���
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


// MyStartupDlg ��Ϣ�������
BOOL MyStartupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	menu_.LoadMenu(IDR_MENU8);
	SetMenu(&menu_);

	list_startup_.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	list_startup_.InsertColumn(0, TEXT("�������"), 0, 200);
	list_startup_.InsertColumn(1, TEXT("������"), 0, 600);
	list_startup_.InsertColumn(2, TEXT("ע���λ��"), 0, 0);

	print_startup();

	return true;
}

void MyStartupDlg::print_startup()
{
	list_startup_.DeleteAllItems();

	static HIMAGELIST nHiml = { 0 };//ͼ���б�
	static SHFILEINFO nPsfi = { 0 };//�ļ���Ϣ
									//ImageList_Destroy(nHiml);//���ͼ���б�
	static bool nOn = false;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(32, 32, ILC_COLOR32, 0, 0);//����ͼ����
		ImageList_SetBkColor(nHiml, list_startup_.GetBkColor());//����ͼ���б��ɫ
		list_startup_.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//���ó����б���ʾͼ��
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
			MessageBox(TEXT("��ע���ʧ��"), TEXT("��ʾ"), MB_ICONWARNING);
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

		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//��ȡ�ļ���Ϣ
		if (!SHGetFileInfo(nPath, nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON)) {
			SHGetFileInfo(nPath, nFileAttributes, &nPsfi, 
				sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		}

		//DWORD nIco = ImageList_AddIcon(nHiml, nPsfi.hIcon);//����ͼ�귵��ͼ���±�
		ImageList_ReplaceIcon(nHiml, i, nPsfi.hIcon);
		DestroyIcon(nPsfi.hIcon);

		list_startup_.SetItemImageEx(i, 0, list_startup_.GetItemText(i, 0), i);
	}
}

//�رտ���������
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
		MessageBox(TEXT("��ע���ʧ��"), TEXT("��ʾ"), MB_ICONWARNING);
		return;
	}

	//ɾ��ѡ�еĸ���ļ���ֵ
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
