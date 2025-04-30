#pragma once
#include "GameTimer.h"
#include "Scene.h"

class Player;
class GameObject;

class GameFramework {
public:
	GameFramework() { }
	~GameFramework() { }

public:
	void OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void Update();
	void Draw();

private:
	void BuildFrameBuffer();
	void ClearFrameBuffer(COLORREF ClearColor);
	void PresentFrameBuffer();

	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

public:
	static BOOL ChangeScene(TAG_SCENE_NAME eTargetSceneTag);

public:
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE	m_hInstance = NULL;
	HWND		m_hWnd = NULL;
	
	BOOL		m_bActive = TRUE;

	RECT		m_rcClient = {};

	HDC			m_hDCFrameBuffer = NULL;
	HBITMAP		m_hBitmapFrameBuffer = NULL;
	HBITMAP		m_hBitmapSelect = NULL;

	GameTimer	m_GameTimer = {};

	POINT		m_ptOldCursorPos = {};

	_TCHAR		m_pszFrameRate[50] = {};

	std::shared_ptr<GameObject>		m_pLockedObject = nullptr;	// Lock On'd GameObject


private:
	// Manages Scenes
	static std::shared_ptr<Scene> m_pCurrentScene;
	static std::array<std::shared_ptr<Scene>, TAG_SCENE_COUNT> m_pScenes;
};
