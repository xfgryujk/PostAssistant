#pragma once
#include <oleacc.h>
#include <mshtml.h>

// �����ļ�·��
extern CString PROFILE_PATH;
const TCHAR TEXT_SIGNS_PATH[] = _T("text.list");
const TCHAR RICH_TEXT_SIGNS_PATH[] = _T("richtext.list");

// �ȼ�
static const int HOTKEY_POST = 0;
static const int HOTKEY_UNICODE_POST = 1;
static const int HOTKEY_HTML_POST = 2;


// �ַ�������
void SliptString(CStringArray& dst, const CString& src, LPCTSTR slipt);
CString ReplaceString(const CString& src, LPCTSTR oldString, LPCTSTR newString);

// ��ҳ����
BOOL GetIEDocument(CComPtr<IHTMLDocument2>& spDoc);
HRESULT EvalJS(CComPtr<IHTMLDocument2>& document, LPCTSTR script, VARIANT* ret = NULL);

// ȡ[0, max - 1]�����
inline int rand(int max)
{
	return (int)((float)rand() / (RAND_MAX + 1) * max);
}

// ���ظ�ȡ�����Ա
inline CString GetUnrepeatedElement(CStringArray& arr, int& rest)
{
	if (rest == 0)
		rest = arr.GetSize();
	int rnd = rand(rest);

	CString tmp = arr[rnd];
	arr[rnd] = arr[rest - 1];
	arr[rest - 1] = tmp;
	rest--;

	return tmp;
}

// ת�塢����
void EscapeString(CString& src);

// �������
BOOL GetInternetImageSize(LPCTSTR URL, int& width, int& height);
void HTTPGet(LPCTSTR URL, BYTE** buffer = NULL, ULONG* size = NULL);
