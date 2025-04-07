#pragma once
#include "GameTimer.h"

class Player;
class Scene;
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
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
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

	std::shared_ptr<Player>			m_pPlayer = nullptr;
	std::shared_ptr<Scene>			m_pScene = nullptr;
	std::shared_ptr<GameObject>		m_pLockedObject = nullptr;	// Lock On'd GameObject

};

