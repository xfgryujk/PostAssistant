// TextSignPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PostAssistant.h"
#include "TextSignPage.h"
#include "afxdialogex.h"
#include "Global.h"


static const int TIMER_ID = 0;


// CTextSignPage �Ի���

IMPLEMENT_DYNAMIC(CTextSignPage, CDialog)

CTextSignPage::CTextSignPage(CWnd* pParent /*=NULL*/)
	: CDialog(CTextSignPage::IDD, pParent)
{

	m_restTextSigns = 0;
}

CTextSignPage::~CTextSignPage()
{
}

void CTextSignPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_STATIC1, m_static);
}


BEGIN_MESSAGE_MAP(CTextSignPage, CDialog)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT1, &CTextSignPage::OnEnChangeEdit1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTextSignPage ��Ϣ�������

#pragma region UI
// ����Esc�رմ���
void CTextSignPage::OnCancel()
{
}

// ���λس��رմ���
void CTextSignPage::OnOK()
{
}

// ���ٴ���
void CTextSignPage::OnClose()
{
	DestroyWindow();

	CDialog::OnClose();
}

void CTextSignPage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect); // Ĭ��343 * 216
	m_edit.SetWindowPos(NULL, 0, 0, rect.Width() - 19, rect.Height() - 40, SWP_NOMOVE | SWP_NOREDRAW);
	m_static.SetWindowPos(NULL, 9, rect.Height() - 21, 0, 0, SWP_NOSIZE | SWP_NOREDRAW);

	Invalidate();
}
#pragma endregion


void CTextSignPage::OnEnChangeEdit1()
{
	// 3s��û�иı������
	SetTimer(TIMER_ID, 3000, NULL);
}

void CTextSignPage::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(TIMER_ID);
	UpdateTextSigns();
	CFile file;
	if (file.Open(TEXT_SIGNS_PATH, CFile::typeText | CFile::modeCreate | CFile::modeWrite))
	{
		CString text;
		m_edit.GetWindowText(text);
		CStringA textA(text);
		file.Write((LPCSTR)textA, textA.GetLength());
		file.Close();
	}

	CDialog::OnTimer(nIDEvent);
}

void CTextSignPage::UpdateTextSigns()
{
	CString text;
	m_edit.GetWindowText(text);
	SliptString(m_textSigns, text, _T("\r\n"));
	m_restTextSigns = m_textSigns.GetSize();
}

