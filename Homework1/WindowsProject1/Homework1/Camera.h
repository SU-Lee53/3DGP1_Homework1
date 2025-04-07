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

class Camera {
public:
	Viewport& GetViewport() { return m_Viewport; }
	XMFLOAT4X4& GetViewPerspectiveProjectMatrix() { return m_xmf4x4ViewPerspectiveProject; }
	XMFLOAT4X4& GetViewOrthographicProjectMatrix() { return m_xmf4x4ViewOrthographicProject; }
	XMFLOAT4X4& GetPerspectiveProjectMatrix() { return m_xmf4x4PerspectiveProject; }
	XMFLOAT4X4& GetViewMatrix() { return m_xmf4x4View; }


	void SetFOVAngle(float fFOVAngle);

	void SetLookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);
	void SetLookAt(const XMFLOAT3& vPosition, const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight);
	void SetPosition(const XMFLOAT3& xmf3Positon) { m_xmf3Position = xmf3Positon; }
	XMFLOAT3& GetPosition() { return m_xmf3Position; }


	void GenerateViewMatrix();
	void GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle);
	void GenerateOrthographicProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fWidth, float fHeight);
	void Move(const XMFLOAT3& xmf3Shift);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void Update(std::shared_ptr<Player> pPlayer, const XMFLOAT3& xmf3LookAt, float fTimeElapsed = 0.016f);

	bool IsInFrustum(const BoundingOrientedBox& xmBoundingBox) const;

private:
	XMFLOAT3 m_xmf3Position = XMFLOAT3{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Right = XMFLOAT3{ 1.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Up = XMFLOAT3{ 0.f, 1.f, 0.f };
	XMFLOAT3 m_xmf3Look = XMFLOAT3{ 0.f ,0.f, 1.f };

	float m_fFOVAngle = 90.0f;
	float m_fProjectRectDistance = 1.0f;

	float m_fAspectRatio = float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT);

	BoundingFrustum		m_xmFrustumView = {};
	BoundingFrustum		m_xmFrustumWorld = {};
	XMFLOAT4X4			m_xmf4x4InverseView = Matrix4x4::Identity();

private:
	XMFLOAT4X4			m_xmf4x4View = Matrix4x4::Identity();
	XMFLOAT4X4			m_xmf4x4PerspectiveProject = Matrix4x4::Identity();
	XMFLOAT4X4			m_xmf4x4ViewPerspectiveProject = Matrix4x4::Identity();

	XMFLOAT4X4			m_xmf4x4OrthographicProject = Matrix4x4::Identity();
	XMFLOAT4X4			m_xmf4x4ViewOrthographicProject = Matrix4x4::Identity();

	Viewport			m_Viewport = {};

};

