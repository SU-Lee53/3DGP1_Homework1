#pragma once

#include "GameObject.h"
#include "Camera.h"

#define _WITH_DRAW_AXIS

class Scene {
public:
	Scene(std::shared_ptr<Player> pPlayer) : m_pPlayer{ pPlayer } { };
	virtual ~Scene() { }

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera);
	
	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
	void CheckObjectByObjectCollisions();
	void CheckObjectByWallCollisions();
	void CheckPlayerByWallCollisions();
	void CheckObjectByBulletCollisions();

	std::shared_ptr<GameObject>& PickObjectPointedByCursor(int xClient, int yClient, std::shared_ptr<Camera> pCamera);


private:
	std::vector<std::shared_ptr<GameObject>> m_pObjects = {};
	
	std::shared_ptr<WallsObject>	m_pWallsObject = nullptr;
	std::weak_ptr<GameObject>		m_pPlayer;

#ifdef _WITH_DRAW_AXIS
	std::shared_ptr<GameObject>		m_pWorldAxis = NULL;
#endif




};

