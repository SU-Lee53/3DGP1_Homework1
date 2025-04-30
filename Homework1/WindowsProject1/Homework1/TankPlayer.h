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
	virtual void ProcessKeyboardInput(float fTimeElapsed) override;
	virtual void ProcessMouseInput(float fTimeElapsed) override;

public:
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) override;

public:
	virtual void OnCollision(std::shared_ptr<GameObject> pOther) override;

public:
	void FireBullet(std::shared_ptr<GameObject> pLockedObject);

	std::array<std::shared_ptr<class BulletObject>, BULLET_COUNT>&
		GetBullets() { return m_pBullets; }

private:
	virtual void UpdatePlayerCamera(float fTimeElapsed) override;


private:
	float m_fSpeed = 8.f;
	float m_fBulletEffectiveRange = 150.f;

	BOOL m_bAutoFire = TRUE;

	std::array<std::shared_ptr<class BulletObject>, BULLET_COUNT> m_pBullets = {};
};

