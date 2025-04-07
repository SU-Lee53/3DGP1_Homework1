#include "stdafx.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Camera.h"
#include "GraphicsPipeline.h"

void GameObject::SetRotationTransform(const XMFLOAT4X4& xmf4x4Transform)
{
	//::memcpy(&m_xmf4x4World, &xmf4x4Transform, sizeof(XMFLOAT4X4));
	m_xmf4x4World._11 = xmf4x4Transform._11; m_xmf4x4World._12 = xmf4x4Transform._12; m_xmf4x4World._13 = xmf4x4Transform._13;
	m_xmf4x4World._21 = xmf4x4Transform._21; m_xmf4x4World._22 = xmf4x4Transform._22; m_xmf4x4World._23 = xmf4x4Transform._23;
	m_xmf4x4World._31 = xmf4x4Transform._31; m_xmf4x4World._32 = xmf4x4Transform._32; m_xmf4x4World._33 = xmf4x4Transform._33;
}

void GameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}

void GameObject::SetPosition(const XMFLOAT3& xmf3Position)
{
	//::memcpy(m_xmf4x4World.m[3], &xmf3Position, sizeof(XMFLOAT3));
	m_xmf4x4World._41 = xmf3Position.x;
	m_xmf4x4World._42 = xmf3Position.y;
	m_xmf4x4World._43 = xmf3Position.z;
}

void GameObject::MoveStraft(float fDistance)
{
	GameObject::SetPosition(Vector3::Add(GetPosition(), Vector3::ScalarProduct(GetRight(), fDistance)));
}

void GameObject::MoveUp(float fDistance)
{
	GameObject::SetPosition(Vector3::Add(GetPosition(), Vector3::ScalarProduct(GetUp(), fDistance)));
}

void GameObject::MoveForward(float fDistance)
{
	GameObject::SetPosition(Vector3::Add(GetPosition(), Vector3::ScalarProduct(GetLook(), fDistance)));
}

void GameObject::Move(const XMFLOAT3& vDirection, float fSpeed)
{
	SetPosition(
		m_xmf4x4World._41 + vDirection.x * fSpeed,
		m_xmf4x4World._42 + vDirection.y * fSpeed,
		m_xmf4x4World._43 + vDirection.z * fSpeed
	);
}

void GameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	m_xmf4x4World = Matrix4x4::Multiply(Matrix4x4::RotationYawPitchRoll(fPitch, fYaw, fRoll), m_xmf4x4World);
}

void GameObject::Rotate(XMFLOAT3& xmf3Axis, float fAngle)
{
	m_xmf4x4World = Matrix4x4::Multiply(Matrix4x4::RotationAxis(xmf3Axis, fAngle), m_xmf4x4World);
}

XMFLOAT3 GameObject::GetPosition() const
{
	return XMFLOAT3{ m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43 };
}

XMFLOAT3 GameObject::GetLook() const
{
	return Vector3::Normalize(XMFLOAT3{ m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33 });
}

XMFLOAT3 GameObject::GetUp() const
{
	return Vector3::Normalize(XMFLOAT3{ m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23 });
}

XMFLOAT3 GameObject::GetRight() const
{
	return Vector3::Normalize(XMFLOAT3{ m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13 });
}

void GameObject::LookTo(const XMFLOAT3& xmf3LookTo, const XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookToLH(GetPosition(), xmf3LookTo, xmf3Up);
	m_xmf4x4World = Matrix4x4::Transpose(xmf4x4View);
}

void GameObject::LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(GetPosition(), xmf3LookAt, xmf3Up);
	m_xmf4x4World = Matrix4x4::Transpose(xmf4x4View);
}

void GameObject::UpdateBoundingBox()
{
	if (m_pMesh) {
		m_pMesh->GetOBB().Transform(m_xmOBB, XMLoadFloat4x4(&m_xmf4x4World));
		XMStoreFloat4(&m_xmOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOBB.Orientation)));
	}
}

void GameObject::Render(HDC hDCFrameBuffer, XMFLOAT4X4& pxmf4x4World, std::shared_ptr<Mesh> pMesh) const
{
	if (pMesh) {
		GraphicsPipeline::SetWorldTransform(pxmf4x4World);

		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_Color);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		pMesh->Render(hDCFrameBuffer);
		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

void GameObject::Animate(float fElapsedTime)
{
	if (m_fRotationSpeed != 0.0f) Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
	if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);

	UpdateBoundingBox();
}

void GameObject::Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera)
{
	if (pCamera->IsInFrustum(m_xmOBB)) {
		GameObject::Render(hDCFrameBuffer, m_xmf4x4World, m_pMesh);
	}
}

void GameObject::GenerateRayForPicking(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) const
{
	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_xmf4x4World) * xmmtxView);

	XMFLOAT3 xmf3CameraOrigin{ 0.f, 0.f,0.f };
	xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
	xmvPickRayDirection = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
	xmvPickRayDirection = XMVector3Normalize(xmvPickRayDirection - xmvPickRayOrigin);
}

int GameObject::PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, float& fHitDistance) const
{
	int nIntersected = 0;
	if (m_pMesh) {
		XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
		GenerateRayForPicking(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
		nIntersected = m_pMesh->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, fHitDistance);
	}

	return nIntersected;
}

// =================
//  ExplosiveObject
// =================

std::shared_ptr<Mesh> ExplosiveObject::m_pExplosionMesh = nullptr;
std::array<XMFLOAT3, EXPLOSION_DEBRISES> ExplosiveObject::m_xmf3SphereVectors = {};

void ExplosiveObject::Animate(float fElapsedTime)
{
	if (m_bBlowingUp) {
		m_fElapsedTimes += fElapsedTime;
		if (m_fElapsedTimes <= m_fDuration) {
			XMFLOAT3 xmf3Position = GetPosition();
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				m_xmf4x4Transforms[i] = Matrix4x4::Identity();
				m_xmf4x4Transforms[i]._41 = xmf3Position.x + m_xmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;
				m_xmf4x4Transforms[i]._42 = xmf3Position.y + m_xmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
				m_xmf4x4Transforms[i]._43 = xmf3Position.z + m_xmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
				m_xmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_xmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_xmf4x4Transforms[i]);
			}
		}
		else {
			m_bBlowingUp = FALSE;
			m_fElapsedTimes = 0.0f;
		}
	}
	else {
		GameObject::Animate(fElapsedTime);
	}
}

void ExplosiveObject::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	if (m_bBlowingUp) {
		for (XMFLOAT4X4& xmf4x4Transform : m_xmf4x4Transforms) {
			GameObject::Render(hDCFrameBuffer, xmf4x4Transform, m_pExplosionMesh);
		}
	}
	else {
		GameObject::Render(hDCFrameBuffer, pCamera);
	}
}

void ExplosiveObject::PrepareExplosion()
{
	for (XMFLOAT3& vec : m_xmf3SphereVectors) {
		XMStoreFloat3(&vec, RandomGenerator::GenerateRandomUnitVectorOnSphere());
	}

	m_pExplosionMesh = std::make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(m_pExplosionMesh, 0.5f, 0.5f, 0.5f);
}

// =============
//  WallsObject
// =============

WallsObject::WallsObject()
{
}

WallsObject::~WallsObject()
{
}

void WallsObject::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	GameObject::Render(hDCFrameBuffer, m_xmf4x4World, m_pMesh);
}

// ==============
//  BulletObject
// ==============

void BulletObject::Animate(float fElapsedTime)
{
	m_fElapsedTimeAfterFire += fElapsedTime;

	float fDistance = m_fMovingSpeed * fElapsedTime;


	if ((m_fElapsedTimeAfterFire > m_fLockingDelayTime) && m_pLockedObject) {
		XMFLOAT3 xmf3Position = GetPosition();
		XMVECTOR xmvPosition = XMLoadFloat3(&xmf3Position);

		XMFLOAT3 xmf3LockedObjectPosition = m_pLockedObject->GetPosition();
		XMVECTOR xmvLockedObjectPosition = XMLoadFloat3(&xmf3LockedObjectPosition);
		XMVECTOR xmvToLockedObject = XMVectorSubtract(xmvLockedObjectPosition, xmvPosition);
		xmvToLockedObject = XMVector3Normalize(xmvToLockedObject);

		XMVECTOR xmvMovingDirection = XMLoadFloat3(&m_xmf3MovingDirection);
		xmvMovingDirection = XMVector3Normalize(XMVectorLerp(xmvMovingDirection, xmvToLockedObject, 0.25f));
		XMStoreFloat3(&m_xmf3MovingDirection, xmvMovingDirection);
	}

#ifdef _WITH_VECTOR_OPERATION
	XMFLOAT3 xmf3Position = GetPosition();

	m_fRotationAngle += m_fRotationSpeed * fElapsedTime;
	if (m_fRotationAngle > 360.0f) m_fRotationAngle = m_fRotationAngle - 360.0f;

	XMFLOAT4X4 mtxRotate1 = Matrix4x4::RotationYawPitchRoll(0.0f, m_fRotationAngle, 0.0f);

	XMFLOAT3 xmf3RotationAxis = Vector3::CrossProduct(m_xmf3RotationAxis, m_xmf3MovingDirection, TRUE);
	float fDotProduct = Vector3::DotProduct(m_xmf3RotationAxis, m_xmf3MovingDirection);
	float fRotationAngle = ::IsEqual(fDotProduct, 1.0f) ? 0.0f : (float)XMConvertToDegrees(acos(fDotProduct));
	XMFLOAT4X4 mtxRotate2 = Matrix4x4::RotationAxis(xmf3RotationAxis, fRotationAngle);

	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate1, mtxRotate2);

	XMFLOAT3 xmf3Movement = Vector3::ScalarProduct(m_xmf3MovingDirection, fDistance, FALSE);
	xmf3Position = Vector3::Add(xmf3Position, xmf3Movement);
	SetPosition(xmf3Position);
#else
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(0.0f, m_fRotationSpeed * fElapsedTime, 0.0f);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
	XMFLOAT3 xmf3Movement = Vector3::ScalarProduct(m_xmf3MovingDirection, fDistance, FALSE);
	XMFLOAT3 xmf3Position = GetPosition();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Movement);
	SetPosition(xmf3Position);
	m_fMovingDistance += fDistance;
#endif

	UpdateBoundingBox();

	if ((m_fMovingDistance > m_fBulletEffectiveRange) || (m_fElapsedTimeAfterFire > m_fLockingTime)) Reset();

}

// ============
//  AxisObject
// ============

void AxisObject::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	GraphicsPipeline::SetWorldTransform(m_xmf4x4World);

	m_pMesh->Render(hDCFrameBuffer);
}
