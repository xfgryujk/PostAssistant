// SettingPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PostAssistant.h"
#include "SettingPage.h"
#include "afxdialogex.h"
#include "Global.h"


// CSettingPage �Ի���

IMPLEMENT_DYNAMIC(CSettingPage, CDialog)

CSettingPage::CSettingPage(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingPage::IDD, pParent)
{

}

#pragma region MFC
CSettingPage::~CSettingPage()
{
}

void CSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_textSignCheck);
	DDX_Control(pDX, IDC_CHECK2, m_richTextSignCheck);
	DDX_Control(pDX, IDC_CHECK3, m_autoRunCheck);
	DDX_Control(pDX, IDC_COMBO1, m_postHotkeyCombo);
	DDX_Control(pDX, IDC_COMBO2, m_unicodePostHotkeyCombo);
	DDX_Control(pDX, IDC_CHECK4, m_keepFormatCheck);
}


BEGIN_MESSAGE_MAP(CSettingPage, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK1, &CSettingPage::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CSettingPage::OnBnClickedCheck2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSettingPage::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CSettingPage::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_CHECK3, &CSettingPage::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CSettingPage::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CSettingPage ��Ϣ�������
#pragma endregion

#pragma region UI
// ����Esc�رմ���
void CSettingPage::OnCancel()
{
}

// ���λس��رմ���
void CSettingPage::OnOK()
{
}

// ���ٴ���
void CSettingPage::OnClose()
{
	DestroyWindow();

	CDialog::OnClose();
}
#pragma endregion


BOOL CSettingPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��ʼ���ȼ���Ͽ�
	for (TCHAR text[] = _T("A"); text[0] <= _T('Z'); text[0]++)
		m_postHotkeyCombo.AddString(text);
	for (TCHAR text[] = _T("A"); text[0] <= _T('Z'); text[0]++)
		m_unicodePostHotkeyCombo.AddString(text);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

// ʹ������ǩ��
void CSettingPage::OnBnClickedCheck1()
{
	WritePrivateProfileString(_T("����"), _T("text"), m_textSignCheck.GetCheck() ? _T("1") : _T("0"), PROFILE_PATH);
}

// ʹ�ø��ı�ǩ��
void CSettingPage::OnBnClickedCheck2()
{
	WritePrivateProfileString(_T("����"), _T("richtext"), m_richTextSignCheck.GetCheck() ? _T("1") : _T("0"), PROFILE_PATH);
}

// ���ָ�ʽ
void CSettingPage::OnBnClickedCheck4()
{
	WritePrivateProfileString(_T("����"), _T("KeepFormat"), m_keepFormatCheck.GetCheck() ? _T("1") : _T("0"), PROFILE_PATH);
}

// �����ȼ�
void CSettingPage::OnCbnSelchangeCombo1()
{
	int index = m_postHotkeyCombo.GetCurSel();
	if (index != CB_ERR)
	{
		if (index == m_unicodePostHotkeyCombo.GetCurSel())
		{
			UnregisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_UNICODE_POST);
			m_unicodePostHotkeyCombo.SetCurSel(-1);
			WritePrivateProfileString(_T("����"), _T("ftz"), _T("-1"), PROFILE_PATH);
		}
		UnregisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_POST);
		RegisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_POST, MOD_CONTROL, 'A' + index);
		CString text;
		text.Format(_T("%d"), index);
		WritePrivateProfileString(_T("����"), _T("ft"), text, PROFILE_PATH);
	}
}

// ��Unicode���ȼ�
void CSettingPage::OnCbnSelchangeCombo2()
{
	/*int index = m_unicodePostHotkeyCombo.GetCurSel();
	if (index != CB_ERR)
	{
		if (index == m_postHotkeyCombo.GetCurSel())
		{
			UnregisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_POST);
			m_postHotkeyCombo.SetCurSel(-1);
			WritePrivateProfileString(_T("����"), _T("ft"), _T("-1"), PROFILE_PATH);
		}
		UnregisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_UNICODE_POST);
		RegisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_UNICODE_POST, MOD_CONTROL, 'A' + index);
		CString text;
		text.Format(_T("%d"), index);
		WritePrivateProfileString(_T("����"), _T("ftz"), text, PROFILE_PATH);
	}*/
}

// ��������
void CSettingPage::OnBnClickedCheck3()
{
	::CRegKey reg;
	if (reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), KEY_SET_VALUE) != ERROR_SUCCESS)
	{
		m_autoRunCheck.SetCheck(!m_autoRunCheck.GetCheck());
		return;
	}
	if (m_autoRunCheck.GetCheck())
	{
		CString value;
		GetModuleFileName(NULL, value.GetBuffer(MAX_PATH), MAX_PATH);
		value.ReleaseBuffer();
		value = _T("\"") + value + _T("\" -hide");
		reg.SetStringValue(_T("PostAssistant"), value);
	}
	else
		reg.DeleteValue(_T("PostAssistant"));
}
