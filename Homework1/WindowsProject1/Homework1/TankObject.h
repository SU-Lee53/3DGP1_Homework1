#pragma once
#include "ExplosiveObject.h"


class TankObject : public ExplosiveObject {
public:
	TankObject();
	~TankObject();

public:
	void SetMovingDirection(const XMFLOAT3& xmf3MovingDirection);
	XMFLOAT3 GetMovingDirection() { return m_xmf3MovingDirection; }
	
	void SetMovingSpeed(float xmf3MovingDirection) { m_fMovingSpeed = m_fMovingSpeed; }
	float GetMovingSpeed() { return m_fMovingSpeed; }

	void SetCollisionUpdated(BOOL bUpdated) { m_bCollisionUpdated = bUpdated; }

public:
	virtual void Initialize();
	virtual void Update(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera);

public:
	virtual void OnCollision(std::shared_ptr<GameObject> pOther) override;

private:
	void InitializeMovingDirection();


private:
	XMFLOAT3	m_xmf3MovingDirection = {};
	float		m_fMovingSpeed = 0.f;


	BOOL m_bCollisionUpdated = FALSE;

};

