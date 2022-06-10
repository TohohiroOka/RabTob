#include "TestField.h"
#include "SceneManager.h"
#include "DirectInput.h"
#include "XInputManager.h"
#include "DebugText.h"

#include <imgui.h>
#include <iomanip>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <memory>

void TestField::Initialize()
{
	map = Map::Create();
}

void TestField::Update()
{
	DirectInput* input = DirectInput::GetInstance();
	XInputManager* xinput = XInputManager::GetInstance();

	map->Update();
}

void TestField::Draw()
{
	assert(cmdList);

	Object3d::PreDraw(cmdList);
	map->Draw();
	Object3d::PostDraw();
}

void TestField::Finalize()
{
}

void TestField::ImguiDraw()
{
}

void TestField::GetConstbufferNum()
{
}