#include "stdafx.h"
#include "Level2Scene.h"

void Level2Scene::BuildObjects()
{
}

void Level2Scene::ReleaseObjects()
{
}

void Level2Scene::Update(float fTimeElapsed)
{
}

void Level2Scene::Render(HDC hDCFrameBuffer)
{
	std::wstring wstrOutText{ L"Level2Scene" };
	::TextOut(hDCFrameBuffer, 100, 100, wstrOutText.c_str(), wstrOutText.length());
}

void Level2Scene::ProcessMouseInput()
{
}

void Level2Scene::ProcessKeyboardInput()
{

}
