#pragma once
#include "Transform.h"

class GameObject {
public:
	GameObject() {}
	virtual ~GameObject() { }

public:
	void SetActive(BOOL bActive) { m_bActive = bActive; }
	void SetMesh(const std::shared_ptr<Mesh>& pMesh) { m_pMesh = pMesh; }
	void SetMesh(std::shared_ptr<Mesh>&& pMesh) { m_pMesh = std::move(pMesh); }
	
	void SetColor(COLORREF color) { m_Color = color; }

	void SetCollidedObject(const std::shared_ptr<GameObject>& pObjectCollided) { m_pObjectCollided = pObjectCollided; }
	void SetCollidedObject(nullptr_t) { m_pObjectCollided.reset(); }

	std::shared_ptr<GameObject>& GetCollidedObject() { return m_pObjectCollided; }

	BoundingOrientedBox& GetOBB() { return m_xmOBB; }

	void LookTo(const XMFLOAT3& xmf3LookTo, const XMFLOAT3& xmf3Up);
	void LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);

	void UpdateBoundingBox();

	void Render(HDC hDCFrameBuffer, XMFLOAT4X4& pxmf4x4World, std::shared_ptr<Mesh> pMesh) const;

	virtual void OnUpdateTransform() {}
	virtual void Update(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera);

	void GenerateRayForPicking(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) const;
	int PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, float& fHitDistance) const;


protected:
	BOOL							m_bActive = TRUE;
	
	std::shared_ptr<class Mesh>		m_pMesh = nullptr;
	std::shared_ptr<Transform>		m_pTransform = nullptr;

	BoundingOrientedBox				m_xmOBB = BoundingOrientedBox{};

	std::shared_ptr<GameObject>		m_pObjectCollided = nullptr;
	COLORREF						m_Color = RGB(255, 0, 0);

};

