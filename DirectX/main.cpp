#include "WindowApp.h"
#include "MainEngine.h"
#include "SafeDelete.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �E�B���h�E�T�C�Y
	const int window_width = 1280;// ����
	const int window_height = 720;// �c��

	//�Q�[����
	const wchar_t* gameName = L"NAVE";

	//�E�B���h�E������
	WindowApp* winApp = new WindowApp();
	winApp->Initialize(window_width, window_height, gameName);

	//�S�̂̏�����
	MainEngine* engine = new MainEngine();
	engine->Initialize();

	while (true)
	{
		if (engine->Update()|| winApp->Update()) { break; }
		engine->Draw();

		//�t���[�����[�g�Ǘ�
		engine->FrameControl(winApp);
	}

	//�o�^����
	safe_delete(engine);
	winApp->Release();

	return 0;
}