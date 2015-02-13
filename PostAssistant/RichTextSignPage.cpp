// RichTextSignPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PostAssistant.h"
#include "RichTextSignPage.h"
#include "afxdialogex.h"
#include "Global.h"


static const int TIMER_ID = 0;


// CRichTextSignPage �Ի���

IMPLEMENT_DYNAMIC(CRichTextSignPage, CDialog)

CRichTextSignPage::CRichTextSignPage(CWnd* pParent /*=NULL*/)
	: CDialog(CRichTextSignPage::IDD, pParent)
{

	m_restRichTextSigns = 0;
}

CRichTextSignPage::~CRichTextSignPage()
{
}

void CRichTextSignPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_STATIC1, m_static);
}


BEGIN_MESSAGE_MAP(CRichTextSignPage, CDialog)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT1, &CRichTextSignPage::OnEnChangeEdit1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRichTextSignPage ��Ϣ�������

#pragma region UI
// ����Esc�رմ���
void CRichTextSignPage::OnCancel()
{
}

// ���λس��رմ���
void CRichTextSignPage::OnOK()
{
}

// ���ٴ���
void CRichTextSignPage::OnClose()
{
	DestroyWindow();

	CDialog::OnClose();
}

void CRichTextSignPage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect); // Ĭ��343 * 216
	m_edit.SetWindowPos(NULL, 0, 0, rect.Width() - 19, rect.Height() - 40, SWP_NOMOVE | SWP_NOREDRAW);
	m_static.SetWindowPos(NULL, 9, rect.Height() - 21, 0, 0, SWP_NOSIZE | SWP_NOREDRAW);

	Invalidate();
}
#pragma endregion

HBRUSH CRichTextSignPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->m_hWnd == m_static.m_hWnd)
		pDC->SetTextColor(RGB(255, 0, 0));

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CRichTextSignPage::OnEnChangeEdit1()
{
	// 3s��û�иı������
	SetTimer(TIMER_ID, 3000, NULL);
}

void CRichTextSignPage::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(TIMER_ID);
	UpdateRichTextSigns();
	CFile file;
	if (file.Open(RICH_TEXT_SIGNS_PATH, CFile::typeText | CFile::modeCreate | CFile::modeWrite))
	{
		CString text;
		m_edit.GetWindowText(text);
		CStringA textA(text);
		file.Write((LPCSTR)textA, textA.GetLength());
		file.Close();
	}

	CDialog::OnTimer(nIDEvent);
}

void CRichTextSignPage::UpdateRichTextSigns()
{
	CString text;
	m_edit.GetWindowText(text);
	SliptString(m_richTextSigns, text, _T("\r\n"));
	m_restRichTextSigns = m_richTextSigns.GetSize();
}
