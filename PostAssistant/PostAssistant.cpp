
// PostAssistant.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "PostAssistant.h"
#include "PostAssistantDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPostAssistantApp

BEGIN_MESSAGE_MAP(CPostAssistantApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPostAssistantApp ����

CPostAssistantApp::CPostAssistantApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPostAssistantApp ����

CPostAssistantApp theApp;


// CPostAssistantApp ��ʼ��
#include "Global.h"
BOOL CPostAssistantApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();


	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	// ���࿪
	if (OpenMutex(MUTEX_ALL_ACCESS, TRUE, _T("PostAssistant")) == NULL)
		CreateMutex(NULL, TRUE, _T("PostAssistant"));
	else
	{
		AfxMessageBox(_T("�࿪ʲô���������ˣ�"), MB_ICONWARNING);
		return FALSE;
	}

	// ��ʼ�����������
	srand(GetTickCount());

	// ��ֹIE11 KB3025390������ִֹ��JS
#pragma region
	static const LPCTSTR regPath[2] = { _T("SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_DISABLE_INTERNAL_SECURITY_MANAGER"),
		_T("SOFTWARE\\Wow6432Node\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_DISABLE_INTERNAL_SECURITY_MANAGER") };
	::CRegKey reg;
	for (int i = 0; i < 2; i++)
		if (reg.Open(HKEY_LOCAL_MACHINE, regPath[i], KEY_ALL_ACCESS) == ERROR_SUCCESS)
		{
			DWORD value;
			LSTATUS status = reg.QueryDWORDValue(_T("iexplore.exe"), value);
			if ((status == ERROR_SUCCESS && value == 0) || status == ERROR_FILE_NOT_FOUND)
			{
				reg.SetDWORDValue(_T("iexplore.exe"), 1);
				AfxMessageBox(_T("������IEʹ����������Ч"), MB_ICONWARNING);
			}
			reg.Close();
		}
#pragma endregion

	// ��ʼ�������ļ�·��
	TCHAR cd[MAX_PATH];
	GetCurrentDirectory(_countof(cd), cd);
	PROFILE_PATH = cd;
	PROFILE_PATH += _T("\\myini.ini");

	// ��ʼ��COM��
	CoInitialize(NULL);

	// ����������
	CPostAssistantDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// ж��COM��
	CoUninitialize();


	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

