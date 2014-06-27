#pragma once
/* �r�b�g�}�b�v�{�^���̃N���X */
class ButtonControl
{
public:
	ButtonControl();
	~ButtonControl();

	/* ButtonControl �f�X�g���N�^ */
	HWND CreateButton(
		LPCWSTR lpszText,
		LPRECT lpRect,
		HWND hWndParent,     // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
		HMENU hMenu,         // ���j���[�n���h���܂��͎q�E�B���h�E ID
		HINSTANCE hInstance, // �A�v���P�[�V�����C���X�^���X�̃n���h��
		int nNorm,           // �m�[�}���̂Ƃ��̃��\�[�X�r�b�g�}�b�vID
		int nSel,            // �������Ƃ��̃��\�[�X�r�b�g�}�b�vID
		int nHover,          // �z�o�[�̂Ƃ��̃��\�[�X�r�b�g�}�b�vID
		int nDis             // �f�B�Z�[�u���̂Ƃ��̃��\�[�X�r�b�g�}�b�vID
		);

private:
	int m_nWidth;			// �{�^���̕�
	int m_nHeight;			// �{�^���̍���
	HBITMAP m_hBmpNorm;		// �ʏ�̃{�^���摜
	HBITMAP m_hBmpSel;		// �}�E�X�_�E�����̃{�^���摜
	HBITMAP m_hBmpHover;	// �}�E�X�I�[�o�[���̃{�^���摜
	HBITMAP m_hBmpDis;		// �������(�f�B�Z�[�u���j���̃{�^���摜
	WNDPROC m_DefBtnProc;	// �{�^���̃f�t�H���g�E�B���h�E�v���V�[�W����ێ�
	BOOL m_bHover;			// �z�[�o�[��Ԃ��ǂ����̃t���O
	BOOL m_bPush;			// �v�b�V����Ԃ��ǂ����̃t���O
	BOOL m_bDisable;		// �������(�f�B�Z�[�u���j���ǂ����̃t���O

	virtual LRESULT ButtonControl::LocalButtonProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
	static LRESULT CALLBACK ButtonControl::GlobalButtonProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
};