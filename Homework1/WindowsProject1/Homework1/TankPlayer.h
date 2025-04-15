#pragma once
#include "Player.h"
class TankPlayer : public Player{
public:
	TankPlayer();
	virtual ~TankPlayer();

public:
	virtual void Initialize() override;
	virtual void Update(float fTimeElapsed) override;

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
	float m_fBulletEffectiveRange = 150.f;

};

