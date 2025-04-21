#pragma once
#include "ExplosiveObject.h"


class TankObject : public ExplosiveObject {
public:
	TankObject();
	~TankObject();

public:
	virtual void Initialize();
	virtual void Update(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera);

public:
	virtual void OnCollision(std::shared_ptr<GameObject> pOther) override;

private:
	XMFLOAT3	m_xmf3MovingDirection = {};
	float		m_fMovingSpeed = 0.f;


};

