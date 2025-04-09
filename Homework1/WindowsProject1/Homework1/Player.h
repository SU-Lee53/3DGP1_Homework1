#pragma once
#include "GameObject.h"

class Camera;

class Player : public GameObject, public std::enable_shared_from_this<Player> {
public:
	Player();
	virtual ~Player();

public:
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	XMFLOAT3 GetRotation() { return XMFLOAT3{ m_fPitch, m_fYaw, m_fRoll }; }

	void LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);

	void Move(DWORD dwDirection, float fDistance);
	void Move(const XMFLOAT3& xmf3Shift, BOOL bUpdateVelocity);
	void Move(float x, float y, float z);

	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);

	void SetCameraOffset(const XMFLOAT3& xmf3CameraOffset);

	virtual void Update(float fTimeElapsed = 0.016f);

	virtual void ProcessMouseInput() {};
	virtual void ProcessKeyboardInput() {};
	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera);

	void SetCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	std::shared_ptr<Camera>& GetCamera() { return m_pCamera; }

protected:
	virtual void UpdatePlayerCamera(float fTimeElapsed);

protected:
	XMFLOAT3 m_xmf3Position = XMFLOAT3{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Right = XMFLOAT3{ 1.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Up = XMFLOAT3{ 0.f, 1.f, 0.f };
	XMFLOAT3 m_xmf3Look = XMFLOAT3{ 0.f, 0.f, 1.f };

	XMFLOAT3 m_xmf3CameraOffset = XMFLOAT3{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3{ 0.f ,0.f, 0.f };

	float m_fFriction = 125.0f;

	float m_fPitch = 0.0f;
	float m_fYaw = 0.0f;
	float m_fRoll = 0.0f;

	std::shared_ptr<Camera> m_pCamera = nullptr;

};

