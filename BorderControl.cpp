#include <windows.h>
#include "BorderControl.h"
#define SZCECOWIZBUTTONPROC TEXT("BorderProc")

/* BorderControl �R���X�g���N�^ */
BorderControl::BorderControl()
{
}

/* BorderControl �f�X�g���N�^ */
BorderControl::~BorderControl()
{
}

/* �{�[�_�[�R���g���[���쐬 */
HWND BorderControl::CreateBorder(
	int nHeight,
	HWND hWndParent,     // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
	HINSTANCE hInstance // �A�v���P�[�V�����C���X�^���X�̃n���h��
	)
{
	m_nHeight = nHeight;
	const HWND hWnd = CreateWindowEx(0, L"STATIC", 0, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, nHeight, 640, 1, hWndParent, 0, hInstance, NULL);
	if (!hWnd) return NULL;
	SetClassLongPtr(hWnd, GCL_STYLE, GetClassLongPtr(hWnd, GCL_STYLE) & ~CS_DBLCLKS);
	SetProp(hWnd, SZCECOWIZBUTTONPROC, this); // �E�B���h�E�n���h���̃v���p�e�ɃI�u�W�F�N�g�̃|�C���^���֘A�t����
	m_DefBorderProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)GlobalButtonProc);
	return hWnd;
}

/* �O���[�o���i�ÓI�j�̃E�B���h�E�v���V�[�W�� �����[�J���ϐ��ւ̃A�N�Z�X�s�� */
LRESULT CALLBACK BorderControl::GlobalButtonProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	BorderControl *pBorderControl = (BorderControl*)GetProp(hWnd, SZCECOWIZBUTTONPROC);
	if (pBorderControl)
	{
		return pBorderControl->LocalButtonProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

/* ���[�J���i���I�j�̃E�B���h�E�v���V�[�W�� */
LRESULT BorderControl::LocalButtonProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch (msg)
	{
	case WM_PRINTCLIENT: // ������������Ȃ���AnimateWindow()�����삵�Ȃ�
	case WM_ERASEBKGND: // �w�i��`��
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		COLORREF clrPrev = SetBkColor((HDC)wParam, RGB(171, 171, 171));
		ExtTextOut((HDC)wParam, 0, 0, ETO_OPAQUE, &rect, 0, 0, 0);
		SetBkColor((HDC)wParam, clrPrev);
	}
		return 1;
	case WM_DESTROY:
		RemoveProp(hWnd, SZCECOWIZBUTTONPROC);
		break;
	}
	return CallWindowProc(m_DefBorderProc, hWnd, msg, wParam, lParam);
}