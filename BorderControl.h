#pragma once
/* �r�b�g�}�b�v�{�^���̃N���X */
class BorderControl
{
public:
	BorderControl();
	~BorderControl();

	/* BorderControl �f�X�g���N�^ */
	HWND CreateBorder(
		int nHeight,
		HWND hWndParent,     // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
		HINSTANCE hInstance
		);

private:
	int m_nHeight;			// �{�^���̍���
	WNDPROC m_DefBorderProc;	// �{�^���̃f�t�H���g�E�B���h�E�v���V�[�W����ێ�

	virtual LRESULT BorderControl::LocalButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK BorderControl::GlobalButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};