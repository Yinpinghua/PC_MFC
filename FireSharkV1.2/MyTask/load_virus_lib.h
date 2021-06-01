#pragma once
#include "MyHead.h"


class load_virus_lib
{
public:

	load_virus_lib();
	~load_virus_lib();

	PVIRUSINFO m_VirusLib = nullptr;
	DWORD m_VirusCount = 0;
	PVIRUSINFO m_ProcessLib = nullptr;
	DWORD m_ProcessCount = 0;

	CString SEG = TEXT("|#|");
	BOOL m_IsLine = FALSE;

	DWORD LoadVirusLibData();
	void GetVirusLib(PVIRUSINFO &nVIRUSINFO);
	void clear_virus_lib_data();
	void SetVirusLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount);

	DWORD LoadProcessLibData();
	void GetProcessLib(PVIRUSINFO &nVIRUSINFO);
	void ClearProcessLibData();
	void SetProcessLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount);


};

