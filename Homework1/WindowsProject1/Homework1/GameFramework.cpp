#include "stdafx.h"
#include "GameFramework.h"
#include "Mesh.h"
#include "Camera.h"
#include "Player.h"

#include "TitleScene.h"
#include "MenuScene.h"
#include "Level1Scene.h"
#include "Level2Scene.h"

using namespace std;

std::shared_ptr<Scene> GameFramework::m_pCurrentScene = nullptr;
std::array<std::shared_ptr<Scene>, TAG_SCENE_COUNT> GameFramework::m_pScenes = {};
TAG_SCENE_NAME GameFramework::m_eCurrentSceneTag = TAG_SCENE_UNDEFINED;

void GameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;
	INPUT.Initialize(m_hWnd);

	BuildFrameBuffer();

	BuildObjects();

	_tcscpy_s(m_pszFrameRate, _T("LabProject ("));
}

void GameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
	if (m_hBitmapSelect) ::DeleteObject(m_hDCFrameBuffer);
}

void GameFramework::Update()
{
	m_GameTimer.Tick(60.0f);

	INPUT.Update();

	ProcessInput();
	AnimateObjects();
}

void GameFramework::Draw()
{
	ClearFrameBuffer(RGB(255, 255, 255));

	//std::shared_ptr<Camera> pCamera = m_pPlayer->GetCamera();
	if (m_pCurrentScene) {
		m_pCurrentScene->Render(m_hDCFrameBuffer);
	}

	PresentFrameBuffer();

	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	::SetWindowText(m_hWnd, m_pszFrameRate);
}

void GameFramework::BuildFrameBuffer()
{
	::GetClientRect(m_hWnd, &m_rcClient);

	HDC hDC = ::GetDC(m_hWnd);

	m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
	::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void GameFramework::ClearFrameBuffer(COLORREF ClearColor)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 0, ClearColor);
	HPEN hOldPen = (HPEN)::SelectObject(m_hDCFrameBuffer, hPen);
	HBRUSH hBrush = ::CreateSolidBrush(ClearColor);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	::Rectangle(m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);
	::SelectObject(m_hDCFrameBuffer, hOldBrush);
	::SelectObject(m_hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

void GameFramework::PresentFrameBuffer()
{
	HDC hDC = ::GetDC(m_hWnd);
	::BitBlt(hDC, m_rcClient.left, m_rcClient.top, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top, m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, SRCCOPY);
	::ReleaseDC(m_hWnd, hDC);
}

void GameFramework::BuildObjects()
{
	// Build Scenes
	{
		m_pScenes[TAG_SCENE_TITLE] = make_shared<TitleScene>();
		m_pScenes[TAG_SCENE_MENU] = make_shared<MenuScene>();
		m_pScenes[TAG_SCENE_LEVEL1] = make_shared<Level1Scene>();
		m_pScenes[TAG_SCENE_LEVEL2] = make_shared<Level2Scene>();

		m_eCurrentSceneTag = TAG_SCENE_LEVEL2;
		m_pCurrentScene = m_pScenes[m_eCurrentSceneTag];
		m_pCurrentScene->BuildObjects();
	}

}


void GameFramework::ReleaseObjects()
{
	// Release and reset pScene
	// reset m_pPlayer
}

void GameFramework::ProcessInput()
{
	if (INPUT.GetButtonDown('1')) {
		m_eCurrentSceneTag = TAG_SCENE_TITLE;
		ChangeScene(TAG_SCENE_TITLE);
	}

	if (INPUT.GetButtonDown('2')) {
		m_eCurrentSceneTag = TAG_SCENE_MENU;
		ChangeScene(TAG_SCENE_MENU);
	}
	
	if (INPUT.GetButtonDown('3')) {
		m_eCurrentSceneTag = TAG_SCENE_LEVEL1;
		ChangeScene(TAG_SCENE_LEVEL1);
	}
	
	if (INPUT.GetButtonDown('4')) {
		m_eCurrentSceneTag = TAG_SCENE_LEVEL2;
		ChangeScene(TAG_SCENE_LEVEL2);
	}
	
	if (INPUT.GetButtonDown('R')) {
		ResetScene();
	}

}

void GameFramework::AnimateObjects()
{
	float fTimeElapsed = m_GameTimer.GetTimeElapsed();
	if (m_pCurrentScene) {
		m_pCurrentScene->Update(fTimeElapsed);
	}
}

void GameFramework::FrameAdvance()
{
	Update();
	Draw();
}

BOOL GameFramework::ChangeScene(TAG_SCENE_NAME eTargetSceneTag)
{
	if (!INPUT.IsCursorShown()) {
		INPUT.ShowCursor();
	}

	m_pCurrentScene->ReleaseObjects();
	m_pCurrentScene.reset();

	m_eCurrentSceneTag = eTargetSceneTag;
	m_pCurrentScene = m_pScenes[m_eCurrentSceneTag];
	m_pCurrentScene->BuildObjects();

	return TRUE;
}

BOOL GameFramework::ResetScene()
{
	m_pCurrentScene->ReleaseObjects();
	m_pCurrentScene.reset();

	m_pCurrentScene = m_pScenes[m_eCurrentSceneTag];
	m_pCurrentScene->BuildObjects();

	return TRUE;
}

LRESULT GameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
			m_GameTimer.Stop();
		else
			m_GameTimer.Start();
		break;
	}
	case WM_SIZE:
		break;
	}
	return(0);
}
