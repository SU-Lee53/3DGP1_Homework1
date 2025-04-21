#pragma once
#include "Scene.h"

class Level2Scene : public Scene {
public:
	Level2Scene() {
		m_eSceneTag = TAG_SCENE_TITLE;
	}

	virtual ~Level2Scene() {}

public:
	virtual void BuildObjects() override;
	virtual void ReleaseObjects() override;

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(HDC hDCFrameBuffer) override;

public:
	virtual void ProcessMouseInput(float fTimeElapsed) override;
	virtual void ProcessKeyboardInput(float fTimeElapsed) override;

public:
	virtual void CheckObjectByBulletCollisions();
};

