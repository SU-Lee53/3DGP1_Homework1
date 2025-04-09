#pragma once
#include "Scene.h"

class TitleScene : public Scene {
public:
	TitleScene() {
		m_eSceneTag = TAG_SCENE_TITLE;
	}

	virtual ~TitleScene() {}

public:
	virtual void BuildObjects() override;
	virtual void ReleaseObjects() override;

	virtual void Animate(float fTimeElapsed) override;
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera) override;

public:
	virtual void ProcessMouseInput() override;
	virtual void ProcessKeyboardInput() override;


};

