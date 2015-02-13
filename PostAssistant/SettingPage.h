#pragma once
#include "afxwin.h"


// CSettingPage �Ի���

class CSettingPage : public CDialog
{
	DECLARE_DYNAMIC(CSettingPage)

public:
	CSettingPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingPage();

// �Ի�������
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedCheck3();

	CButton m_textSignCheck;
	CButton m_richTextSignCheck;
	CButton m_keepFormatCheck;
	CComboBox m_postHotkeyCombo;
	CComboBox m_unicodePostHotkeyCombo;
	CButton m_autoRunCheck;
	afx_msg void OnBnClickedCheck4();
};
