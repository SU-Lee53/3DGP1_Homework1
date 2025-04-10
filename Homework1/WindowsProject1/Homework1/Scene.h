#pragma once

#include "GameObject.h"
#include "Camera.h"

//#define _WITH_DRAW_AXIS

enum TAG_SCENE_NAME : UINT8 {
	TAG_SCENE_TITLE = 0,
	TAG_SCENE_MENU,
	TAG_SCENE_LEVEL1,
	TAG_SCENE_LEVEL2,

	TAG_SCENE_COUNT,

	TAG_SCENE_UNDEFINED = 99
};

class Scene {
public:
	Scene() { };
	virtual ~Scene() { }

public:
	TAG_SCENE_NAME GetSceneTag() { return m_eSceneTag; }

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void Update(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera);
	
	virtual void ProcessMouseInput() { }
	virtual void ProcessKeyboardInput() { }

	void CheckObjectByObjectCollisions();

	std::shared_ptr<GameObject>& PickObjectPointedByCursor(int xClient, int yClient, std::shared_ptr<Camera> pCamera);


protected:
	std::vector<std::shared_ptr<GameObject>> m_pObjects = {};
	std::shared_ptr<Player>		m_pPlayer;

	TAG_SCENE_NAME m_eSceneTag = TAG_SCENE_UNDEFINED;

#ifdef _WITH_DRAW_AXIS
	std::shared_ptr<GameObject>		m_pWorldAxis = NULL;
#endif

};
