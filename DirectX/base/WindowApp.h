#pragma once
#include <Windows.h>

class WindowApp
{
public:
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	WindowApp() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~WindowApp() {};

	/// <summary>
	/// �E�B���h�E�v���\�W���̐���
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns>�E�B���h�E�v���\�W��</returns>
	static LRESULT WindowProcdure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="windowWidth">��ʕ�</param>
	/// <param name="windowHeight">��ʍ���</param>
	/// <param name="gameName">�^�C�g���o�[��</param>
	void Initialize(const int windowWidth, const int windowHeight, const wchar_t* gameName);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <returns>�A�v���P�[�V�������I�����邩�ۂ�</returns>
	bool Update();

	/// <summary>
	/// �o�^����
	/// </summary>
	void Release();

	/// <summary>
	/// �E�B���h�E�I�u�W�F�N�g�̎擾
	/// </summary>
	/// <returns>hwnd</returns>
	static HWND GetHwnd() { return hwnd; }

	/// <summary>
	/// �E�B���h�E�N���X�ݒ�̎擾
	/// </summary>
	/// <returns>hInstance</returns>
	static HINSTANCE GetWinInstance() { return winClass.hInstance; }

	/// <summary>
	/// �����̎擾
	/// </summary>
	/// <returns>windowWidth</returns>
	static UINT GetWindowWidth() { return windowWidth; }

	/// <summary>
	/// �c���̎擾
	/// </summary>
	/// <returns>windowHeight</returns>
	static UINT GetWindowHeight() { return windowHeight; }

private:

	//���b�Z�[�W
	MSG msg = {};
	//����
	static UINT windowWidth;
	//����
	static UINT windowHeight;
	//�E�B���h�E�N���X�̐ݒ�
	static WNDCLASSEX winClass;
	//�E�B���h�E�I�u�W�F�N�g
	static HWND hwnd;
};

