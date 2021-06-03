#include <windows.h>
#include "ButtonControl.h"
#define SZCECOWIZBUTTONPROC TEXT("ButtonProc")

/* ButtonControl �R���X�g���N�^ */
ButtonControl::ButtonControl()
{
	m_bHover = 0;
	m_bPush = 0;
	m_bDisable = 0;
	m_hBmpNorm = NULL;
	m_hBmpSel = NULL;
	m_hBmpHover = NULL;
	m_hBmpDis = NULL;
}

/* ButtonControl �f�X�g���N�^ */
ButtonControl::~ButtonControl()
{
	DeleteObject(m_hBmpNorm);
	DeleteObject(m_hBmpSel);
	DeleteObject(m_hBmpHover);
	DeleteObject(m_hBmpDis);
}

/* �{�^���R���g���[���쐬 */
HWND ButtonControl::CreateButton(
	LPCWSTR lpszText,
	LPRECT lpRect,
	HWND hWndParent,     // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
	HMENU hMenu,         // ���j���[�n���h���܂��͎q�E�B���h�E ID
	HINSTANCE hInstance, // �A�v���P�[�V�����C���X�^���X�̃n���h��
	int nNorm,           // �m�[�}���̂Ƃ��̃��\�[�X�r�b�g�}�b�vID
	int nSel,            // �������Ƃ��̃��\�[�X�r�b�g�}�b�vID
	int nHover,          // �z�o�[�̂Ƃ��̃��\�[�X�r�b�g�}�b�vID
	int nDis             // �f�B�Z�[�u���̂Ƃ��̃��\�[�X�r�b�g�}�b�vID
	)
{
	m_hBmpNorm = LoadBitmap(hInstance, MAKEINTRESOURCE(nNorm));
	m_hBmpSel = LoadBitmap(hInstance, MAKEINTRESOURCE(nSel));
	m_hBmpHover = LoadBitmap(hInstance, MAKEINTRESOURCE(nHover));
	m_hBmpDis = LoadBitmap(hInstance, MAKEINTRESOURCE(nDis));
	m_nWidth = lpRect->right - lpRect->left;
	m_nHeight = lpRect->bottom - lpRect->top;
	HWND hWnd = CreateWindowEx(0, L"BUTTON", lpszText, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, lpRect->left, lpRect->top, m_nWidth, m_nHeight, hWndParent, hMenu, hInstance, NULL);
	if (!hWnd) return NULL;
	SetClassLongPtr(hWnd, GCL_STYLE, GetClassLongPtr(hWnd, GCL_STYLE) & ~CS_DBLCLKS);
	SetProp(hWnd, SZCECOWIZBUTTONPROC, this); // �E�B���h�E�n���h���̃v���p�e�ɃI�u�W�F�N�g�̃|�C���^���֘A�t����
	m_DefBtnProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)GlobalButtonProc);
	return hWnd;
}

/* �O���[�o���i�ÓI�j�̃E�B���h�E�v���V�[�W�� �����[�J���ϐ��ւ̃A�N�Z�X�s�� */
LRESULT CALLBACK ButtonControl::GlobalButtonProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	ButtonControl *pButtonControl = (ButtonControl*)GetProp(hWnd, SZCECOWIZBUTTONPROC);
	if (pButtonControl)
	{
		return pButtonControl->LocalButtonProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

/* ���[�J���i���I�j�̃E�B���h�E�v���V�[�W�� */
LRESULT ButtonControl::LocalButtonProc(
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
			const HDC hdc = CreateCompatibleDC((HDC)wParam);
			if (m_bDisable)
			{
				SelectObject(hdc, m_hBmpDis);
			}
			else if (m_bPush)
			{
				SelectObject(hdc, m_hBmpSel);
			}
			else
			{
				if (m_bHover)
				{
					SelectObject(hdc, m_hBmpHover);
				}
				else
				{
					SelectObject(hdc, m_hBmpNorm);
				}
			}
			BitBlt((HDC)wParam, 0, 0, m_nWidth, m_nHeight, hdc, 0, 0, SRCCOPY);
			TCHAR szText[8];
			RECT rect;
			const DWORD dwOldTextBkMode = SetBkMode((HDC)wParam, TRANSPARENT);
			GetWindowText(hWnd, szText, 8);
			GetClientRect(hWnd, &rect);
			DrawText((HDC)wParam, szText, -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			SetBkMode((HDC)wParam, dwOldTextBkMode);
			DeleteDC(hdc);
		}
		return 1;
	case WM_ENABLE: // �����E�L����Ԃ̐؂蕪��
		m_bDisable = !(BOOL)wParam;
		InvalidateRect(hWnd, 0, TRUE);
		break;
	case WM_LBUTTONDOWN:
		m_bPush = TRUE;
		m_bHover = FALSE;
		InvalidateRect(hWnd, 0, TRUE);
		break;
	case WM_LBUTTONUP:
		m_bPush = FALSE;
		InvalidateRect(hWnd, 0, TRUE);
		break;
	case WM_MOUSEMOVE:
		if (!m_bPush)
		{
			if (!m_bHover)
			{
				m_bHover = TRUE;
				TRACKMOUSEEVENT	tme;
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);
				InvalidateRect(hWnd, 0, TRUE);
			}
		}
		break;
	case WM_MOUSELEAVE:
		m_bHover = FALSE;
		InvalidateRect(hWnd, 0, TRUE);
		break;
	case WM_DESTROY:
		RemoveProp(hWnd, SZCECOWIZBUTTONPROC);
		break;
	}
	return CallWindowProc(m_DefBtnProc, hWnd, msg, wParam, lParam);
}