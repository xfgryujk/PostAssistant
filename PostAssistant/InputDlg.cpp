// InputDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PostAssistant.h"
#include "InputDlg.h"
#include "afxdialogex.h"


// CInputDlg �Ի���

IMPLEMENT_DYNAMIC(CInputDlg, CDialog)

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	m_content = NULL;
}

CInputDlg::CInputDlg(const CString& staticCapital, CString* content, CWnd* pParent /*=NULL*/)
: CDialog(CInputDlg::IDD, pParent)
{
	m_staticCapital = staticCapital;
	m_content       = content;
}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_STATIC1, m_static);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
END_MESSAGE_MAP()


// CInputDlg ��Ϣ�������

BOOL CInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_static.SetWindowText(m_staticCapital);
	if (m_content != NULL)
		m_edit.SetWindowText(*m_content);

	SetForegroundWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CInputDlg::OnOK()
{
	if (m_content != NULL)
		m_edit.GetWindowText(*m_content);

	CDialog::OnOK();
}
