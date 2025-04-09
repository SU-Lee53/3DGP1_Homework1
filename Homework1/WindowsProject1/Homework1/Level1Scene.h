#pragma once
#include "Scene.h"

class Level1Scene : public Scene {
public:
	Level1Scene() {
		m_eSceneTag = TAG_SCENE_TITLE;
	}

	virtual ~Level1Scene() {}

public:
	virtual void BuildObjects() override;
	virtual void ReleaseObjects() override;

	virtual void Animate(float fTimeElapsed) override;
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera) override;

public:
	virtual void ProcessMouseInput() override;
	virtual void ProcessKeyboardInput() override;

};

