#pragma once
#include "afxwin.h"
#include "afxlinkctrl.h"


// CAboutPage �Ի���

class CAboutPage : public CDialog
{
	DECLARE_DYNAMIC(CAboutPage)

public:
	CAboutPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAboutPage();

// �Ի�������
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CEdit m_edit;
	CMFCLinkCtrl m_link;
};
