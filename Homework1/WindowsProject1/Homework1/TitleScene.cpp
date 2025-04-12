#include "stdafx.h"
#include "TitleScene.h"

void TitleScene::BuildObjects()
{
}

void TitleScene::ReleaseObjects()
{
}

void TitleScene::Update(float fTimeElapsed)
{
}

void TitleScene::Render(HDC hDCFrameBuffer)
{
	std::wstring wstrOutText{ L"TitleScene" };
	::TextOut(hDCFrameBuffer, 100, 100, wstrOutText.c_str(), wstrOutText.length());

	::Rectangle(hDCFrameBuffer, 0, 0, 100, 10);
}

void TitleScene::ProcessMouseInput()
{
}

void TitleScene::ProcessKeyboardInput()
{
}
