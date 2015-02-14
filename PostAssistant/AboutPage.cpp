// AboutPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PostAssistant.h"
#include "AboutPage.h"
#include "afxdialogex.h"


// CAboutPage �Ի���

IMPLEMENT_DYNAMIC(CAboutPage, CDialog)

CAboutPage::CAboutPage(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutPage::IDD, pParent)
{

}

CAboutPage::~CAboutPage()
{
}

void CAboutPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_STATIC1, m_link);
}


BEGIN_MESSAGE_MAP(CAboutPage, CDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_STN_CLICKED(IDC_STATIC1, &CAboutPage::OnStnClickedStatic1)
END_MESSAGE_MAP()


// CAboutPage ��Ϣ�������

#pragma region UI
// ����Esc�رմ���
void CAboutPage::OnCancel()
{
}

// ���λس��رմ���
void CAboutPage::OnOK()
{
}

// ���ٴ���
void CAboutPage::OnClose()
{
	DestroyWindow();

	CDialog::OnClose();
}

void CAboutPage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect); // Ĭ��343 * 216
	m_edit.SetWindowPos(NULL, 0, 0, rect.Width() - 19, rect.Height() - 40, SWP_NOMOVE | SWP_NOREDRAW);
	m_link.SetWindowPos(NULL, rect.Width() - 118, rect.Height() - 21, 0, 0, SWP_NOSIZE | SWP_NOREDRAW);

	Invalidate();
}
#pragma endregion

BOOL CAboutPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edit.SetWindowText(_T("���úú����ȼ�����\r\n\
~~~~~~~~~~~~~~~~~~\r\n\
\r\n\
�˰汾ֻ֧��IE�ں˵��������Firefox��Chrome�ȷ�IE�ں˵����������Ч\r\n\
\r\n\
���¡�Chrome�����أ�http ://pan.baidu.com/s/1i3ooIFz\r\n\
\r\n\
�˳���ͨ��MITЭ�鿪Դ���������https://github.com/xfgryujk/PostAssistantForIE�õ�Դ�룬��ӭ���Ƴ���"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CAboutPage::OnStnClickedStatic1()
{
	ShellExecute(NULL, _T("open"), _T("http://tieba.baidu.com/i/37897560"), NULL, NULL, SW_NORMAL);
}
