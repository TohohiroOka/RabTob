#include "WindowApp.h"
#include <imgui_impl_win32.h>

UINT WindowApp::windowWidth;
UINT WindowApp::windowHeight;
WNDCLASSEX WindowApp::winClass{};
HWND WindowApp::hwnd = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WindowApp::WindowProcdure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return 1;
	}

	if (msg == WM_DESTROY)
	{
		PostQuitMessage((0));
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WindowApp::Initialize(const int window_Width, const int window_height,const wchar_t* gameName)
{
	windowWidth = window_Width;
	windowHeight = window_height;

	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.lpfnWndProc = (WNDPROC)WindowProcdure;// �E�B���h�E�v���V�[�W����ݒ�
	winClass.lpszClassName = gameName;// �E�B���h�E�N���X��
	winClass.hInstance = GetModuleHandle(nullptr);// �E�B���h�E�n���h��
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);// �J�[�\���w��

	// �E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&winClass);
	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, (LONG)windowWidth, (LONG)windowHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);// �����ŃT�C�Y�␳

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(winClass.lpszClassName,// �N���X��
		gameName,// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,// �W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,// �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,// �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,// �E�B���h�E����
		wrc.bottom - wrc.top,// �E�B���h�E�c��
		nullptr,// �e�E�B���h�E�n���h��
		nullptr,// ���j���[�n���h��

		winClass.hInstance,// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);// �I�v�V����

	// �E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

bool WindowApp::Update()
{
	//���b�Z�[�W����
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//�A�v���P�[�V�������I��鎞��message��WM_QUIT�ɂȂ�
	if (msg.message == WM_QUIT)
	{
		return true;
	}
	else { return false; }
}

void WindowApp::Release()
{
	UnregisterClass(winClass.lpszClassName, winClass.hInstance);
}