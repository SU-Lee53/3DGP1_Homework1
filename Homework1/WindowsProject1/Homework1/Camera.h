#pragma once

struct Viewport {
	void SetViewport(int nLeft, int nTop, int nWidth, int nHeight) {
		m_nLeft = nLeft;
		m_nTop = nTop;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
	}

	int m_nLeft = 0;
	int m_nTop = 0;
	int m_nWidth = 0;
	int m_nHeight = 0;
};

class Player;
class Transform;

class Camera {
public:
	Camera();
	~Camera();

public:
	void SetFOVAngle(float fAngle = 90.0f);
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight);
	void SetNearZ(float fNearZ);
	void SetFarZ(float fFarZ);

	void SetPosition(const XMFLOAT3& xmf3NewPosition);
	void SetPosition(const XMVECTOR& xmvNewPosition);
	void SetPosition(float fXPos, float fYPos, float fZPos);

	void SetRotation(const XMFLOAT3& xmf3NewRotation);
	void SetRotation(const XMVECTOR& xmvNewRotation);
	void SetRotation(float fPitch, float fYaw, float fRoll);

	Viewport& GetViewport() { return m_Viewport; }
	XMFLOAT4X4& GetViewMatrix() { return m_xmf4x4View; }
	XMFLOAT4X4& GetInverseViewMatrix() { return m_xmf4x4View; }
	XMFLOAT4X4& GetPerspectiveProjectMatrix() { return m_xmf4x4PerspectiveProject; }
	XMFLOAT4X4& GetViewPerspectiveProjectMatrix();
	XMFLOAT4X4& GetOrthographicProjectMatrix() { return m_xmf4x4OrthographicProject; }
	XMFLOAT4X4& GetViewOrthographicProjectMatrix();
	std::shared_ptr<Transform>& GetTransform() { return m_pTransform; }

public:
	void Initialize(std::shared_ptr<Player> pOwnerPlayer);
	void Update();

	bool IsInFrustum(const BoundingOrientedBox& xmBoundingBox) const;

private:
	void GenerateViewMatrix();
	void GeneratePerspectiveProjectionMatrix();
	void GenerateOrthographicProjectionMatrix();

private:
	std::shared_ptr<Transform> m_pTransform = nullptr;

	// Y angle
	float m_fFOVAngle = 0.f;
	float m_fAspectRatio = 0.f;
	float m_fProjectPlaneDistance = 0.f;
	float m_fNearZ = 0.f;
	float m_fFarZ = 0.f;

	BoundingFrustum m_xmFrustumView = {};
	BoundingFrustum m_xmFrustumWorld = {};

	XMFLOAT4X4 m_xmf4x4View = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4InverseView = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4PerspectiveProject = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4ViewPerspectiveProject = Matrix4x4::Identity();
	XMFLOAT4X4 m_xmf4x4OrthographicProject = Matrix4x4::Identity(); 
	XMFLOAT4X4 m_xmf4x4ViewOrthographicProject = Matrix4x4::Identity(); 

	Viewport m_Viewport = {};

	std::weak_ptr<Player> m_wpOwner;

	BOOL m_bViewUpdated = FALSE;
	BOOL m_bProjectionUpdated = FALSE;

};

