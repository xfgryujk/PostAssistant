#include "stdafx.h"
#include "Global.h"
#include <vector>
#include <msxml2.h>


CString PROFILE_PATH;


// �ָ��ַ���
void SplitString(CStringArray& dst, const CString& src, LPCTSTR slipt)
{
	dst.RemoveAll();
	const int len = _tcslen(slipt);

	int start = 0, end = 0;
	while ((end = src.Find(slipt, end)) != -1)
	{
		dst.Add(src.Mid(start, end - start));
		start = end += len;
	}
	dst.Add(src.Right(src.GetLength() - start));
}

// ö��Ѱ��Internet Explorer_Server����
static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	TCHAR buf[30];
	GetClassName(hwnd, buf, _countof(buf));
	if (_tcscmp(buf, _T("Internet Explorer_Server")) == 0)
	{
		*(HWND*)lParam = hwnd;
		return FALSE;
	}
	else
		return TRUE;
}

// ȡ���λ��IE��ҳ�ĵ��ӿ�
BOOL GetIEDocument(CComPtr<IHTMLDocument2>& spDoc)
{
	BOOL result = FALSE;

	// ȡ���λ�ô��ھ��
	POINT point;
	GetCursorPos(&point);
	HWND hWnd = WindowFromPoint(point);
	// ȡInternet Explorer_Server���ھ��
	TCHAR buf[30];
	GetClassName(hWnd, buf, _countof(buf));
	if (_tcscmp(buf, _T("Internet Explorer_Server")) != 0)
	{
		hWnd = NULL;
		EnumChildWindows(hWnd, EnumChildProc, (LPARAM)&hWnd);
	}
	
	// ȡdocument�ӿ�
	if (hWnd != NULL)
	{
		LRESULT lRes;
		UINT nMsg = RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));
		SendMessageTimeout(hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes);
		result = SUCCEEDED(ObjectFromLresult(lRes, IID_IHTMLDocument2, 0, (void**)&spDoc));
	}

	return result;
}

// ִ��JavaScript
HRESULT EvalJS(CComPtr<IHTMLDocument2>& document, LPCTSTR script, VARIANT* ret)
{
	HRESULT hr;

	// ȡwindow
	CComPtr<IHTMLWindow2> window;
	if (FAILED(hr = document->get_parentWindow(&window)))
		return hr;

	// ȡeval ID
	static DISPID dispID = -1;
	if (dispID == -1)
	{
		CComPtr<IDispatchEx> dispEx;
		if (FAILED(hr = window->QueryInterface(&dispEx)))
			return hr;
		if (FAILED(hr = dispEx->GetDispID(_bstr_t("eval"), fdexNameCaseSensitive, &dispID)))
			return hr;
	}

	// ����eval
	CComDispatchDriver disp = window;
	hr = disp.Invoke1(dispID, &_variant_t(script), ret);
	
	return hr;
}

// ת��
void EscapeString(CString& src)
{
	src.Replace(_T("&"), _T("&amp;"));
	src.Replace(_T(" "), _T("&nbsp;"));
	src.Replace(_T("\""), _T("\\\""));
	src.Replace(_T("'"), _T("\\'"));
	src.Replace(_T("'"), _T("\\'"));
	src.Replace(_T("<"), _T("&lt;"));
	src.Replace(_T(">"), _T("&gt;"));
}

// ȡ����ͼƬ�ߴ�
BOOL GetInternetImageSize(LPCTSTR URL, int& width, int& height)
{
	// ����ͼƬ
	BYTE* buffer;
	ULONG size;
	HTTPGet(URL, &buffer, &size);
	if (buffer == NULL)
	{
		width = height = 500;
		return FALSE;
	}
	
	// ������
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, size);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, buffer, size);
	delete buffer;
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// ���ص�CImage
	CImage image;
	image.Load(pstm);
	// �ͷ���
	GlobalUnlock(m_hMem);
	pstm->Release();

	width  = image.GetWidth();
	height = image.GetHeight();
	return TRUE;
}

// http get����ע������delete buffer
void HTTPGet(LPCTSTR URL, BYTE** buffer, ULONG* size)
{
	if (buffer != NULL)
		*buffer = NULL;
	if (size != NULL)
		*size = 0;
	CComPtr<IServerXMLHTTPRequest> xml;
	if (FAILED(xml.CoCreateInstance(__uuidof(ServerXMLHTTP))))
		return;

	xml->open(_bstr_t(_T("GET")), _bstr_t(URL), _variant_t(false), _variant_t(), _variant_t());
	xml->setTimeouts(5000, 5000, 5000, 5000);
	xml->send(_variant_t());
	long state = 0;
	xml->get_readyState(&state);
	if (state == 4 && buffer != NULL && size != NULL)
	{
		_variant_t body;
		xml->get_responseBody(body.GetAddress());
		BYTE* p;
		if (SUCCEEDED(SafeArrayAccessData(body.parray, (void**)&p)))
		{
			*size = body.parray->rgsabound[0].cElements;
			*buffer = new BYTE[*size];
			memcpy(*buffer, p, *size);
			SafeArrayUnaccessData(body.parray);
		}
	}
}
