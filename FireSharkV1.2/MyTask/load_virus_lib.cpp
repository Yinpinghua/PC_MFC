#include "stdafx.h"
#include "load_virus_lib.h"


load_virus_lib::load_virus_lib()
{

}


load_virus_lib::~load_virus_lib()
{
	clear_virus_lib_data();
}

//��ȡ���ز�����
DWORD load_virus_lib::LoadVirusLibData()
{
	HANDLE  pFile = nullptr;
	//�򿪱������.dat�Ĳ������ļ�
	pFile = CreateFile(GetProgramDir()+ TEXT("VirusLib.dat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == (HANDLE)-1) {
		return 0;
	}

	DWORD nSize = GetFileSize((PHANDLE)pFile, 0);
	DWORD nReadSize = 0;

	m_VirusCount = nSize / sizeof(VIRUSINFO);
	m_VirusLib = new VIRUSINFO[m_VirusCount];
	ReadFile(pFile, (LPVOID)m_VirusLib, nSize, &nReadSize,NULL);
	CloseHandle(pFile);
	pFile = nullptr;
	return m_VirusCount;
}

//ȡ����������Ϣ
void load_virus_lib::GetVirusLib(PVIRUSINFO &nVIRUSINFO)
{
	memcpy_s(nVIRUSINFO, sizeof(VIRUSINFO)*m_VirusCount,
		m_VirusLib, sizeof(VIRUSINFO)*m_VirusCount);
}

//�򿪲�����
void load_virus_lib::SetVirusLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount)
{
	FILE *pFile = nullptr;
	fopen_s(&pFile, GetProgramDir() + "VirusLib.dat", "wb");
	m_VirusCount = fwrite(nVIRUSINFO, sizeof(VIRUSINFO), nCount, pFile);
	fclose(pFile);
}

//�رղ�����
void load_virus_lib::clear_virus_lib_data()
{
	if (m_VirusLib !=nullptr){
		delete[] m_VirusLib;
	}

	m_VirusLib = nullptr;
	m_VirusCount = 0;
}

//��ȡ������
DWORD load_virus_lib::LoadProcessLibData()
{

	HANDLE  pFile = nullptr;

	pFile = CreateFile(GetProgramDir() + TEXT("ProcessLib.dat"), GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (pFile == (HANDLE)-1) {
		return 0;
	}

	ClearProcessLibData();
	DWORD nSize = GetFileSize((PHANDLE)pFile, 0);
	DWORD nReadSize = 0;

	m_ProcessCount = nSize / sizeof(VIRUSINFO);
	m_ProcessLib = new VIRUSINFO[m_ProcessCount];
	ReadFile(pFile, (LPVOID)m_ProcessLib, nSize, &nReadSize, NULL);
	CloseHandle(pFile);
	return m_ProcessCount;
}

//ȡ����������Ϣ
void load_virus_lib::GetProcessLib(PVIRUSINFO &nVIRUSINFO)
{
	memcpy_s(nVIRUSINFO, sizeof(VIRUSINFO)*m_ProcessCount, m_ProcessLib, sizeof(VIRUSINFO)*m_ProcessCount);
}

//��ӽ�����������
void load_virus_lib::SetProcessLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount)
{
	//�򿪲�д��ProcessLib���л��ļ�
	FILE *pFile = nullptr;
	fopen_s(&pFile, GetProgramDir() + "ProcessLib.dat", "wb");
	m_ProcessCount = fwrite(nVIRUSINFO, sizeof(VIRUSINFO), nCount, pFile);
	fclose(pFile);
}

//�رհ�����
void load_virus_lib::ClearProcessLibData()
{
	if (m_ProcessLib != nullptr){
		delete[] m_ProcessLib;
	}

	m_ProcessLib = nullptr;
	m_ProcessCount = 0;
}

