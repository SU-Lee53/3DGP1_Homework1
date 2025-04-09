#include "stdafx.h"
#include "MenuScene.h"

void MenuScene::BuildObjects()
{
}

void MenuScene::ReleaseObjects()
{
}

void MenuScene::Animate(float fTimeElapsed)
{
}

void MenuScene::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	std::wstring wstrOutText{ L"MenuScene" };
	::TextOut(hDCFrameBuffer, 100, 100, wstrOutText.c_str(), wstrOutText.length());
}

void MenuScene::ProcessMouseInput()
{
}

void MenuScene::ProcessKeyboardInput()
{
}
