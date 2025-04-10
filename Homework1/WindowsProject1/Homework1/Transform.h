#pragma once

class Transform
{
public:
	// Setter
	void SetPosition(const XMFLOAT3& xmf3NewPosition);
	void SetPosition(const XMVECTOR& xmvNewPosition);
	void SetPosition(float fXPos, float fYPos, float fZPos);
	
	void AddPosition(const XMFLOAT3& xmf3AddPosition);
	void AddPosition(const XMVECTOR& xmvAddPosition);
	void AddPosition(float fXPos, float fYPos, float fZPos);

	void SetRotation(const XMFLOAT3& xmf3NewRotation);
	void SetRotation(const XMVECTOR& xmvNewRotation);
	void SetRotation(float fPitch/* x */, float fYaw/* y */, float fRoll/* z */);
	
	void AddRotation(const XMFLOAT3& xmf3AddRotation);
	void AddRotation(const XMVECTOR& xmvAddRotation);
	void AddRotation(float fPitch/* x */, float fYaw/* y */, float fRoll/* z */);

	// Getter
	XMFLOAT3& GetPosition() { return m_xmf3Position; }
	XMFLOAT3& GetRotation() { return m_xmf3Rotation; }
	XMFLOAT4& GetRotationQuarternion() { return m_xmf4RotationQuaternion; }

	XMFLOAT3& GetLook() { return m_xmf3Look; }
	XMFLOAT3& GetRight() { return m_xmf3Right; }
	XMFLOAT3& GetUp() { return m_xmf3Right; }

	XMFLOAT4X4& GetWorldMatrix() { return m_xmf4x4World; }

public:
	void Update();

private:
	void UpdateQuaternion();
	void UpdateBasis();

private:
	XMFLOAT3 m_xmf3Position = XMFLOAT3{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Rotation = XMFLOAT3{ 0.f, 0.f, 0.f };	// pitch, yaw, roll
	XMFLOAT4 m_xmf4RotationQuaternion = XMFLOAT4{ 0.f, 0.f, 0.f, 1.f };
	XMFLOAT4X4 m_xmf4x4World = Matrix4x4::Identity();

	XMFLOAT3 m_xmf3Look = XMFLOAT3{ 0.f, 0.f, 1.f };
	XMFLOAT3 m_xmf3Right = XMFLOAT3{ 1.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Up = XMFLOAT3{ 0.f, 1.f, 0.f };

	BOOL m_bPositionUpdated = FALSE;
	BOOL m_bRotationUpdated = FALSE;

};

