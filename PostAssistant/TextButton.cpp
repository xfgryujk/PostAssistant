// TextButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PostAssistant.h"
#include "TextButton.h"


// CTextButton

IMPLEMENT_DYNAMIC(CTextButton, CStatic)

CTextButton::CTextButton()
{
	m_isMouseOver = FALSE;
}

CTextButton::~CTextButton()
{
}


BEGIN_MESSAGE_MAP(CTextButton, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CTextButton ��Ϣ�������



void CTextButton::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize		= sizeof(tme);
	tme.hwndTrack	= m_hWnd;
	tme.dwFlags		= TME_LEAVE | TME_HOVER;
	tme.dwHoverTime	= 1;
	_TrackMouseEvent(&tme);

	CStatic::OnMouseMove(nFlags, point);
}

void CTextButton::OnMouseHover(UINT nFlags, CPoint point)
{
	if (!m_isMouseOver)
	{
		m_isMouseOver = TRUE;
		Invalidate();
	}

	CStatic::OnMouseHover(nFlags, point);
}

void CTextButton::OnMouseLeave()
{
	if (m_isMouseOver)
	{
		m_isMouseOver = FALSE;
		Invalidate();
	}

	CStatic::OnMouseLeave();
}

HBRUSH CTextButton::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_isMouseOver ? RGB(255, 0, 0) : RGB(0, 0, 255));

	// TODO:  �����Ӧ���ø����Ĵ�������򷵻ط� null ����
	return (HBRUSH)(COLOR_BTNFACE + 1);
}

void CTextButton::PreSubclassWindow()
{
	SetWindowLong(m_hWnd, GWL_STYLE, SS_NOTIFY | GetWindowLong(m_hWnd, GWL_STYLE));
	SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));

	CStatic::PreSubclassWindow();
}
