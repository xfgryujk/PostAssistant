/*
The MIT License (MIT)

Copyright (c) 2015 xfgryujk

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "stdafx.h"
#include "PostAssistant.h"
#include "PostAssistantDlg.h"
#include "afxdialogex.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����
static const UINT WM_TASKBARCREATED = RegisterWindowMessage(_T("TaskbarCreated"));
static const UINT WM_TRAY = WM_APP + 1;

#pragma region
static const TCHAR POST_SCRIPT1[] = _T("UE.instants.ueditorInstant0.getContent=function(){\
var d=UE.Editor.prototype.getContent.call(this);return d.replace(/&#39;/g,\"'\").replace(/&quot;/g,'\"')\
.replace(/(^(<br\\/>)+)|((<br\\/>)+$)/g,\"\")+'");
static const TCHAR NO_OLD_CONTENT_POST_SCRIPT1[] = _T("UE.instants.ueditorInstant0.getContent = function(){return '");
static const TCHAR KEEP_FORMAT_POST_SCRIPT1[] = _T("UE.instants.ueditorInstant0.getContent=function(){\
var d=UE.Editor.prototype.getContent.call(this);return d.replace(/&#39;/g,\"'\").replace(/&quot;/g,'\"')\
.replace(/(^(<br\\/>)+)|((<br\\/>)+$)/g,\"\").replace(/&nbsp;&nbsp;/g,\"����\").replace(/<br\\/>/g,\"<br>\")+'");
static const TCHAR POST_SCRIPT2[] = _T("'};$(\".poster_submit\")[0].click();");

static const TCHAR VIDEO1[] = _T("<embed class = \"BDE_Flash\" type=\"application/x-shockwave-flash\" \
pluginspage=\"http://www.macromedia.com/go/getflashplayer\" wmode=\"transparent\" play=\"true\" \
loop=\"false\" menu=\"false\" src=\"");
static const TCHAR VIDEO2[] = _T("\" width=\"500\" height=\"450\" allowscriptaccess=\"never\" \
allowfullscreen=\"true\" scale=\"noborder\">");

static const TCHAR IMAGE_FORMAT[] = _T("<img class = \"BDE_Image\" src = \"%s\" unselectable=\"on\" \
pic_type=\"1\" height=\"%d\" width=\"%d\" / >");

static const TCHAR GET_CONTENT_SCRIPT[] = _T("document.getElementById('ueditor_replace').innerHTML");
static const TCHAR SET_CONTENT_SCRIPT1[] = _T("document.getElementById('ueditor_replace').innerHTML = '");
static const TCHAR SET_CONTENT_SCRIPT2[] = _T("';");
#pragma endregion


// ���캯��
CPostAssistantDlg::CPostAssistantDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPostAssistantDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ��ʼ��m_pages
	m_pages[0] = &m_settingPage;
	m_pages[1] = &m_textSignPage;
	m_pages[2] = &m_richTextSignPage;
	m_pages[3] = &m_converCodePage;
	m_pages[4] = &m_aboutPage;

	// ��ʼ������ͼ������
	m_nfData.cbSize				= sizeof(NOTIFYICONDATA);
	m_nfData.hIcon				= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_tcscpy_s(m_nfData.szTip, _T("��������"));
	m_nfData.uCallbackMessage	= WM_TRAY;
	m_nfData.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
}

#pragma region MFC
void CPostAssistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CPostAssistantDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CPostAssistantDlg::OnTcnSelchangeTab1)
	ON_WM_HOTKEY()
	ON_MESSAGE(WM_TRAY, OnTray)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskBarCreated)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPostAssistantDlg ��Ϣ�������


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPostAssistantDlg::OnPaint()
{
	static BOOL first = TRUE;
	if (first)
	{
		first = FALSE;
		CStringArray argv;
		SliptString(argv, AfxGetApp()->m_lpCmdLine, _T(" "));
		for (int i = 0; i < argv.GetSize(); i++)
			if (argv[i].MakeLower() == _T("-hide")) // ��ʼ���ش��ڣ�����OnInitDialog��Ч
			{
				ShowWindow(SW_HIDE);
				Shell_NotifyIcon(NIM_ADD, &m_nfData);
				break;
			}
		return;
	}

	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPostAssistantDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
#pragma endregion

// ��ʼ��
BOOL CPostAssistantDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ��ʼ�����̴��ھ��
	m_nfData.hWnd = m_hWnd;

	// ��ʼ��m_tab
	m_tab.InsertItem(0, _T("����"));
	m_tab.InsertItem(1, _T("����ǩ��"));
	m_tab.InsertItem(2, _T("���ı�ǩ��"));
	m_tab.InsertItem(3, _T("����ת��"));
	m_tab.InsertItem(4, _T("����&&����"));

	// ��ʼ����ҳ
	m_settingPage.Create(IDD_SETTING, &m_tab);
	m_textSignPage.Create(IDD_TEXTSIGN, &m_tab);
	m_richTextSignPage.Create(IDD_RICHTEXTSIGN, &m_tab);
	m_converCodePage.Create(IDD_CONVERTCODE, &m_tab);
	m_aboutPage.Create(IDD_ABOUT, &m_tab);

	CRect rect;
	m_tab.GetClientRect(&rect);
	rect.left += 1; rect.right -= 3; rect.top += 22; rect.bottom -= 2;
	m_settingPage.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	m_textSignPage.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_richTextSignPage.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_converCodePage.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_aboutPage.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	// ��ȡ����
	TCHAR tmpText[10];
	// ʹ������ǩ��
	GetPrivateProfileString(_T("����"), _T("text"), _T("1"), tmpText, _countof(tmpText), PROFILE_PATH);
	m_settingPage.m_textSignCheck.SetCheck(tmpText[0] == _T('1'));
	// ʹ�ø��ı�ǩ��
	GetPrivateProfileString(_T("����"), _T("richtext"), _T("1"), tmpText, _countof(tmpText), PROFILE_PATH);
	m_settingPage.m_richTextSignCheck.SetCheck(tmpText[0] == _T('1'));
	// ���ָ�ʽ
	GetPrivateProfileString(_T("����"), _T("KeepFormat"), _T("0"), tmpText, _countof(tmpText), PROFILE_PATH);
	m_settingPage.m_keepFormatCheck.SetCheck(tmpText[0] == _T('1'));
	// �����ȼ�
	m_settingPage.m_postHotkeyCombo.SetCurSel(GetPrivateProfileInt(_T("����"), _T("ft"), 'Q' - 'A', PROFILE_PATH));
	// ���������ȼ�
	m_settingPage.m_unicodePostHotkeyCombo.SetCurSel(GetPrivateProfileInt(_T("����"), _T("ftz"), 'W' - 'A', PROFILE_PATH));
	// ��������
	::CRegKey reg;
	if (reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), KEY_QUERY_VALUE) == ERROR_SUCCESS)
	{
		CString value;
		ULONG length;
		reg.QueryStringValue(_T("PostAssistant"), value.GetBuffer(MAX_PATH), &length);
		value.ReleaseBuffer();
		CString exePath;
		GetModuleFileName(NULL, exePath.GetBuffer(MAX_PATH), MAX_PATH);
		exePath.ReleaseBuffer();
		m_settingPage.m_autoRunCheck.SetCheck(value.Find(exePath) != -1);
	}
	// ����ǩ��
	CFile file;
	CStringA buffer;
	if (file.Open(TEXT_SIGNS_PATH, CFile::typeText | CFile::modeRead))
	{
		const ULONGLONG len = file.GetLength();
		LPSTR p = buffer.GetBuffer((int)len + 1);
		file.Read(p, (UINT)len);
		file.Close();
		p[len] = '\0';
		buffer.ReleaseBuffer();
		m_textSignPage.m_edit.SetWindowText(CString(buffer));
	}
	m_textSignPage.UpdateTextSigns();
	// ���ı�ǩ��
	if (file.Open(RICH_TEXT_SIGNS_PATH, CFile::typeText | CFile::modeRead))
	{
		const ULONGLONG len = file.GetLength();
		LPSTR p = buffer.GetBuffer((int)len + 1);
		file.Read(p, (UINT)len);
		file.Close();
		p[len] = '\0';
		buffer.ReleaseBuffer();
		m_richTextSignPage.m_edit.SetWindowText(CString(buffer));
	}
	m_richTextSignPage.UpdateRichTextSigns();

	// ע���ȼ�
	m_settingPage.OnCbnSelchangeCombo1();
	m_settingPage.OnCbnSelchangeCombo2();
	RegisterHotKey(m_hWnd, HOTKEY_HTML_POST, MOD_CONTROL | MOD_SHIFT | MOD_ALT, VK_F11);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �ͷ�
void CPostAssistantDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// ����ͼ��
	Shell_NotifyIcon(NIM_DELETE, &m_nfData);
}

#pragma region UI
// ���� /////////////////////////////////////////////////////////////////////////////////

// ����Esc�رմ���
void CPostAssistantDlg::OnCancel()
{
}

// ���λس��رմ���
void CPostAssistantDlg::OnOK()
{
}

// ���ٴ���
void CPostAssistantDlg::OnClose()
{
	DestroyWindow();

	CDialog::OnClose();
}

// �л���ǩ
void CPostAssistantDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int index = m_tab.GetCurSel();
	for (int i = 0; i < 5; i++)
		m_pages[i]->ShowWindow(i == index ? SW_SHOW : SW_HIDE);
}

// ������С�ߴ�
void CPostAssistantDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 384;
	lpMMI->ptMinTrackSize.y = 299;

	CDialog::OnGetMinMaxInfo(lpMMI);
}

// �ı�ߴ�
void CPostAssistantDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect); // Ĭ��368 * 261
	m_tab.SetWindowPos(NULL, 0, 0, rect.Width() - 21, rect.Height() - 21, SWP_NOMOVE | SWP_NOREDRAW);

	m_tab.GetClientRect(&rect);
	rect.left += 1; rect.right -= 3; rect.top += 22; rect.bottom -= 2;
	m_settingPage.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOREDRAW);
	m_textSignPage.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOREDRAW);
	m_richTextSignPage.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOREDRAW);
	m_converCodePage.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOREDRAW);
	m_aboutPage.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOREDRAW);

	Invalidate();
}

// ���� /////////////////////////////////////////////////////////////////////////////////

// ��С��
void CPostAssistantDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE) // ��С��ʱ���ش���
	{
		ShowWindow(SW_HIDE);
		Shell_NotifyIcon(NIM_ADD, &m_nfData);
		return;
	}

	CDialog::OnSysCommand(nID, lParam);
}

// ������Ϣ
LRESULT CPostAssistantDlg::OnTray(WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_LBUTTONDOWN) // ������ʾ����
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nfData);
		ShowWindow(SW_SHOW);
	}

	return 0;
}

// �������ؽ�
LRESULT CPostAssistantDlg::OnTaskBarCreated(WPARAM wParam, LPARAM lParam)
{
	if (!IsWindowVisible())
		Shell_NotifyIcon(NIM_ADD, &m_nfData);
	return 0;
}

// �ȼ� /////////////////////////////////////////////////////////////////////////////////

// �ȼ�
void CPostAssistantDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	switch (nHotKeyId)
	{
	case HOTKEY_POST:
		Post();
		break;
	case HOTKEY_UNICODE_POST:
		UnicodePost();
		break;
	case HOTKEY_HTML_POST:
		HTMLPost();
		break;
	}

	CDialog::OnHotKey(nHotKeyId, nKey1, nKey2);
}
#pragma endregion

// ��ͨ����
void CPostAssistantDlg::Post()
{
	UnregisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_POST);

	CComPtr<IHTMLDocument2> document;
	if (GetIEDocument(document) && IsTieba(document))
	{
		CString script = m_settingPage.m_keepFormatCheck.GetCheck() ? KEEP_FORMAT_POST_SCRIPT1 : POST_SCRIPT1;
		AddSign(script);
		script += POST_SCRIPT2;
		EvalJS(document, script);
	}
	
	RegisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_POST, MOD_CONTROL, 'A' + m_settingPage.m_postHotkeyCombo.GetCurSel());
}

// Unicode��
void CPostAssistantDlg::UnicodePost()
{
	// ��ʱû�취
}

// �༭Դ��
void CPostAssistantDlg::HTMLPost()
{
	UnregisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_HTML_POST);

	CComPtr<IHTMLDocument2> document;
	if (GetIEDocument(document) && IsTieba(document))
	{
		// ȡԭ����������HTML
		_variant_t vContent;
		EvalJS(document, GET_CONTENT_SCRIPT, &vContent);
		CString content = vContent;
		CInputDlg inputDlg(_T("�ı���Դ��"), &content);
		if (inputDlg.DoModal() == IDOK && content != _T(""))
		{
			// ת��
			content = ReplaceString(content, _T("\r\n"), _T("<br>"));
			content = ReplaceString(content, _T("\\"), _T("\\\\"));
			content = ReplaceString(content, _T("'"), _T("\\'"));

			CString script = NO_OLD_CONTENT_POST_SCRIPT1 + content;
			AddSign(script);
			script += POST_SCRIPT2;
			EvalJS(document, SET_CONTENT_SCRIPT1 + content + SET_CONTENT_SCRIPT2);
			EvalJS(document, script);
		}
	}

	RegisterHotKey(m_hWnd, HOTKEY_HTML_POST, MOD_CONTROL | MOD_SHIFT | MOD_ALT, VK_F11);
}

// ���� /////////////////////////////////////////////////////////////////////////////////

// JS����ǩ��
void CPostAssistantDlg::AddSign(CString& script)
{
	BOOL hasTextSign = m_settingPage.m_textSignCheck.GetCheck() && m_textSignPage.m_edit.GetWindowTextLengthW() > 0;
	BOOL hasRichTextSign = m_settingPage.m_richTextSignCheck.GetCheck() && m_richTextSignPage.m_edit.GetWindowTextLengthW() > 0;
	if (!hasTextSign && !hasRichTextSign)
		return;

	script += _T("<br><br><br>");

	// ����ǩ��
	if (hasTextSign)
	{
		CString sign = GetUnrepeatedElement(m_textSignPage.m_textSigns, m_textSignPage.m_restTextSigns);
		EscapeString(sign);
		script += sign;
	}

	// ���ı�ǩ��
	if (hasRichTextSign)
	{
		if (hasTextSign)
			script += _T("<br>");
		CString sign = GetUnrepeatedElement(m_richTextSignPage.m_richTextSigns, m_richTextSignPage.m_restRichTextSigns);
		CString ext = sign.Right(4).MakeLower();
		if (!(ext == ".jpg" || ext == ".gif" || ext == ".png")) // ����Ƶ
			script += VIDEO1 + sign + VIDEO2;
		else // ��ͼƬ
		{
			// ȡͼƬ�ߴ�
			TCHAR tmp[20];
			GetPrivateProfileString(_T("ͼƬ�ߴ�"), sign, _T(""), tmp, _countof(tmp), PROFILE_PATH);
			int width, height;
			if (_stscanf_s(tmp, _T("%dx%d"), &width, &height) != 2) // �޳ߴ��¼
			{
				if(!GetInternetImageSize(sign, width, height))
					AfxMessageBox(_T("�r(�s_�t)�qͼƬ�ߴ��ȡʧ��(�������⣿)��Ĭ��500x500"), MB_ICONINFORMATION);
				CString text;
				text.Format(_T("%dx%d"), width, height);
				WritePrivateProfileString(_T("ͼƬ�ߴ�"), sign, text, PROFILE_PATH);
			}
			// ���ߴ�560*600
			if (width > 560 || height > 600)
			{
				float scale1 = 560.0f / (float)width;
				float scale2 = 600.0f / (float)height;
				float scale = scale1 < scale2 ? scale1 : scale2;
				width  = (int)(width  * scale);
				height = (int)(height * scale);
			}

			CString image;
			image.Format(IMAGE_FORMAT, sign, height, width);
			script += image;
		}
	}
}

// ��������ַ
BOOL CPostAssistantDlg::IsTieba(CComPtr<IHTMLDocument2>& document)
{
	BSTR url_;
	document->get_URL(&url_);
	CString url = _bstr_t(url_, FALSE);
	return url.Find(_T("http://tieba.baidu.com/f")) != -1 || url.Find(_T("http://tieba.baidu.com/p")) != -1;
}
