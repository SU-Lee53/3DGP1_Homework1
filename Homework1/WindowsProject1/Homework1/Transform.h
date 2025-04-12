#pragma once

class Transform
{
public:
	// Setter
	BOOL SetPosition(const XMFLOAT3& xmf3NewPosition);
	BOOL SetPosition(const XMVECTOR& xmvNewPosition);
	BOOL SetPosition(float fXPos, float fYPos, float fZPos);
	
	BOOL AddPosition(const XMFLOAT3& xmf3AddPosition);
	BOOL AddPosition(const XMVECTOR& xmvAddPosition);
	BOOL AddPosition(float fXPos, float fYPos, float fZPos);
	
	BOOL SetRotation(const XMFLOAT3& xmf3NewRotation);
	BOOL SetRotation(const XMVECTOR& xmvNewRotation);
	BOOL SetRotation(float fPitch/* x */, float fYaw/* y */, float fRoll/* z */);
	
	BOOL AddRotation(const XMFLOAT3& xmf3AddRotation);
	BOOL AddRotation(const XMVECTOR& xmvAddRotation);
	BOOL AddRotation(float fPitch/* x */, float fYaw/* y */, float fRoll/* z */);

	// Getter
	XMFLOAT3& GetPosition() { return m_xmf3Position; }
	XMFLOAT3& GetRotation() { return m_xmf3Rotation; }

	XMFLOAT3& GetLook() { return m_xmf3Look; }
	XMFLOAT3& GetRight() { return m_xmf3Right; }
	XMFLOAT3& GetUp() { return m_xmf3Up; }

	XMFLOAT4X4& GetWorldMatrix() { return m_xmf4x4World; }

public:
	void Update();

private:
	void UpdateBasis();

private:
	XMFLOAT3 m_xmf3Position = XMFLOAT3{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Rotation = XMFLOAT3{ 0.f, 0.f, 0.f };	// pitch, yaw, roll
	XMFLOAT4X4 m_xmf4x4World = Matrix4x4::Identity();

	XMFLOAT3 m_xmf3Look = XMFLOAT3{ 0.f, 0.f, 1.f };
	XMFLOAT3 m_xmf3Right = XMFLOAT3{ 1.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Up = XMFLOAT3{ 0.f, 1.f, 0.f };

	BOOL m_bPositionUpdated = FALSE;
	BOOL m_bRotationUpdated = FALSE;

};

