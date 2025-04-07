#pragma once

class GameObject {
public:
	GameObject() {}
	virtual ~GameObject() { }

public:
	void SetActive(BOOL bActive) { m_bActive = bActive; }
	void SetMesh(const std::shared_ptr<Mesh>& pMesh) { m_pMesh = pMesh; }
	void SetMesh(std::shared_ptr<Mesh>&& pMesh) { m_pMesh = std::move(pMesh); }
	
	void SetColor(COLORREF color) { m_Color = color; }

	void SetRotationTransform(const XMFLOAT4X4& xmf4x4Transform);

	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& xmf3Position);

	void SetMovingDirection(const XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection); }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	void SetRotationAxis(const XMFLOAT3& xmf3RotationAxis) { m_xmf3RotationAxis = Vector3::Normalize(xmf3RotationAxis); }
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }

	void SetCollidedObject(const std::shared_ptr<GameObject>& pObjectCollided) { m_pObjectCollided = pObjectCollided; }
	void SetCollidedObject(nullptr_t) { m_pObjectCollided.reset(); }

	std::shared_ptr<GameObject>& GetCollidedObject() { return m_pObjectCollided; }

	BoundingOrientedBox& GetOBB() { return m_xmOBB; }
	XMFLOAT3& GetMovingDirection() { return m_xmf3MovingDirection; }
	float GetMovingSpeed() { return m_fMovingSpeed; }
	XMFLOAT4X4 GetWorldMatrix() { return m_xmf4x4World; }

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetLook() const;
	XMFLOAT3 GetUp() const;
	XMFLOAT3 GetRight() const;

	void MoveStraft(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);
	void Move(const XMFLOAT3& vDirection, float fSpeed);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);

	void LookTo(const XMFLOAT3& xmf3LookTo, const XMFLOAT3& xmf3Up);
	void LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);

	void UpdateBoundingBox();

	void Render(HDC hDCFrameBuffer, XMFLOAT4X4& pxmf4x4World, std::shared_ptr<Mesh> pMesh) const;

	virtual void OnUpdateTransform() {}
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera);

	void GenerateRayForPicking(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) const;
	int PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, float& fHitDistance) const;


protected:
	BOOL							m_bActive = TRUE;
	
	std::shared_ptr<class Mesh>		m_pMesh = nullptr;
	XMFLOAT4X4						m_xmf4x4World = Matrix4x4::Identity();

	BoundingOrientedBox				m_xmOBB = BoundingOrientedBox{};

	std::shared_ptr<GameObject>		m_pObjectCollided = nullptr;
	COLORREF						m_Color = RGB(255, 0, 0);

	XMFLOAT3						m_xmf3MovingDirection = XMFLOAT3{ 0.f,0.f,0.f };
	float							m_fMovingSpeed = 0.f;
	float							m_fMovingRange = 0.f;

	XMFLOAT3						m_xmf3RotationAxis = XMFLOAT3{ 0.f, 1.f, 0.f };
	float							m_fRotationSpeed = 0.f;

};

// =================
//  ExplosiveObject
// =================

class ExplosiveObject final : public GameObject {
public:
	ExplosiveObject() { }
	~ExplosiveObject() { }

	void SetBlowingUp(BOOL bBlowingUp) { m_bBlowingUp = bBlowingUp; }
	BOOL IsBlowingUp() const { return m_bBlowingUp; }

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera);

	static void PrepareExplosion();

private:
	BOOL										m_bBlowingUp = FALSE;
	std::array<XMFLOAT4X4, EXPLOSION_DEBRISES>	m_xmf4x4Transforms = {};

	float m_fElapsedTimes = 0.f;
	float m_fDuration = 2.0f;
	float m_fExplosionSpeed = 10.0f;
	float m_fExplosionRotation = 720.0f;

private:
	static std::shared_ptr<Mesh> m_pExplosionMesh;
	static std::array<XMFLOAT3, EXPLOSION_DEBRISES> m_xmf3SphereVectors;

};

// =============
//  WallsObject
// =============

class WallsObject final : public GameObject {
public:
	WallsObject();
	virtual ~WallsObject();

	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera);

	void SetWallPlane(int nIndex, const XMFLOAT4& xmf4Plane) {
		m_xmf4WallPlanes[nIndex] = xmf4Plane;
	}
	
	void SetOBB(const BoundingOrientedBox& xmOBB) { m_xmOBBPlayerMoveCheck = xmOBB; }

	std::array<XMFLOAT4, 6>& GetWallPlanes() { return m_xmf4WallPlanes; }

private:
	BoundingOrientedBox			m_xmOBBPlayerMoveCheck = {};
	std::array<XMFLOAT4, 6>		m_xmf4WallPlanes = {};

};

// ==============
//  BulletObject
// ==============

#define _WITH_VECTOR_OPERATION

class BulletObject final : public GameObject {
public:
	BulletObject(float fEffectiveRange) : m_fBulletEffectiveRange{ fEffectiveRange } {}
	virtual ~BulletObject() {}

public:
	void SetFirePosition(XMFLOAT3 xmf3FirePosition) {
		m_xmf3FirePosition = xmf3FirePosition;
		SetPosition(xmf3FirePosition);
	}

	void SetWorldMatrix(const XMFLOAT4X4& xmf4x4World) { m_xmf4x4World = xmf4x4World; }
	void SetLockedObject(std::shared_ptr<GameObject> pLockedObject) { m_pLockedObject = pLockedObject; }
	BOOL IsActive() const { return m_bActive; }

	void Reset() {
		m_pLockedObject.reset();
		m_pLockedObject = nullptr;
		m_fElapsedTimeAfterFire = 0;
		m_fMovingDistance = 0;
		m_fRotationAngle = 0.0f;

		m_bActive = FALSE;
	}

	virtual void Animate(float fElapsedTime);

private:
	float		m_fBulletEffectiveRange = 50.0f;
	float		m_fMovingDistance = 0.0f;
	float		m_fRotationAngle = 0.0f;
	XMFLOAT3	m_xmf3FirePosition = XMFLOAT3{ 0.f, 0.f, 1.f };

	float						m_fElapsedTimeAfterFire = 0.0f;
	float						m_fLockingDelayTime = 0.3f;
	float						m_fLockingTime = 4.0f;
	std::shared_ptr<GameObject> m_pLockedObject = nullptr;

};

// ============
//  AxisObject
// ============

class AxisObject final : public GameObject {
public:
	AxisObject() { }
	virtual ~AxisObject() { }

	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera);

};

