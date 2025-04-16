#pragma once
#include "Player.h"

constexpr static int BULLET_COUNT = 50;

class TankPlayer : public Player{
public:
	TankPlayer();
	virtual ~TankPlayer();

public:
	virtual void Initialize() override;
	virtual void Update(float fTimeElapsed) override;
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera);

public:
	virtual void ProcessKeyboardInput() override;
	virtual void ProcessMouseInput() override;

public:
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) override;

public:
	void FireBullet(std::shared_ptr<GameObject> pLockedObject);

private:
	virtual void UpdatePlayerCamera(float fTimeElapsed) override;


private:
	XMFLOAT3 m_xmf3DefaultRotation = XMFLOAT3{ -90.f, 180.f, 0.f };
	float m_fBulletEffectiveRange = 150.f;

	std::array<std::shared_ptr<class BulletObject>, BULLET_COUNT> m_pBullets = {};
};

