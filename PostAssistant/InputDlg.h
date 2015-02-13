#pragma once
#include "afxwin.h"


// CInputDlg �Ի���

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CInputDlg(const CString& staticCapital, CString* content, CWnd* pParent = NULL);
	virtual ~CInputDlg();

// �Ի�������
	enum { IDD = IDD_INPUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();


	CEdit m_edit;
	CStatic m_static;
protected:
	CString m_staticCapital;
	CString* m_content;
};
