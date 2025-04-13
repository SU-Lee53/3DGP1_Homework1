#include "stdafx.h"
#include "Transform.h"

BOOL Transform::SetPosition(const XMFLOAT3& xmf3NewPosition)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMVECTOR xmvNewPosition = XMLoadFloat3(&xmf3NewPosition);

	if (!XMVector3Equal(xmvPosition, xmvNewPosition)) {
		XMStoreFloat3(&m_xmf3Position, xmvNewPosition);
		m_bUpdated = TRUE;
	}

	return m_bUpdated;
}

BOOL Transform::SetPosition(const XMVECTOR& xmvNewPosition)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);

	if (!XMVector3Equal(xmvPosition, xmvNewPosition)) {
		XMStoreFloat3(&m_xmf3Position, xmvNewPosition);
		m_bUpdated = TRUE;
	}

	return m_bUpdated;
}

BOOL Transform::SetPosition(float fXPos, float fYPos, float fZPos)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMVECTOR xmvNewPosition = XMVectorSet(fXPos, fYPos, fZPos, 1.0f);

	if (!XMVector3Equal(xmvPosition, xmvNewPosition)) {
		XMStoreFloat3(&m_xmf3Position, xmvNewPosition);
		m_bUpdated = TRUE;
	}

	return m_bUpdated;
}

BOOL Transform::AddPosition(const XMFLOAT3& xmf3AddPosition)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMVECTOR xmvAddPosition = XMLoadFloat3(&xmf3AddPosition);
	XMStoreFloat3(&m_xmf3Position, XMVectorAdd(xmvPosition, xmvAddPosition));
	m_bUpdated = TRUE;
	return m_bUpdated;
}

BOOL Transform::AddPosition(const XMVECTOR& xmvAddPosition)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMStoreFloat3(&m_xmf3Position, XMVectorAdd(xmvPosition, xmvAddPosition));
	m_bUpdated = TRUE;
	return m_bUpdated;
}

BOOL Transform::AddPosition(float fXPos, float fYPos, float fZPos)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMVECTOR xmvAddPosition = XMVectorSet(fXPos, fYPos, fZPos, 1.0f);
	XMStoreFloat3(&m_xmf3Position, XMVectorAdd(xmvPosition, xmvAddPosition));
	m_bUpdated = TRUE;
	return m_bUpdated;
}

BOOL Transform::SetRotation(const XMFLOAT3& xmf3NewRotation)
{
	return SetRotation(XMLoadFloat3(&xmf3NewRotation));
}

BOOL Transform::SetRotation(const XMVECTOR& xmvNewRotation)
{
	XMVECTOR xmvRotation = XMLoadFloat3(&m_xmf3Rotation);

	if (!XMVector3Equal(xmvRotation, xmvNewRotation)) {
		XMStoreFloat3(&m_xmf3Rotation, xmvNewRotation);
		m_bUpdated = TRUE;
	}

	return m_bUpdated;
}

BOOL Transform::SetRotation(float fPitch, float fYaw, float fRoll)
{
	return SetRotation(XMVectorSet(fPitch, fYaw, fRoll, 0.f));
}

BOOL Transform::AddRotation(const XMFLOAT3& xmf3AddRotation)
{
	return AddRotation(XMLoadFloat3(&xmf3AddRotation));
}

BOOL Transform::AddRotation(const XMVECTOR& xmvAddRotation)
{
	XMVECTOR xmvCurRotation = XMLoadFloat3(&m_xmf3Rotation);
	XMVECTOR xmvRotation = XMVectorAdd(xmvCurRotation, xmvAddRotation);

	XMStoreFloat3(&m_xmf3Rotation, xmvRotation);
	m_bUpdated = TRUE;

	return m_bUpdated;
}

BOOL Transform::AddRotation(float fPitch, float fYaw, float fRoll)
{
	return AddRotation(XMVectorSet(fPitch, fYaw, fRoll, 0.f));
}

XMFLOAT3 Transform::GetLook() const
{
	return Vector3::Normalize(XMFLOAT3{ m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33 });
}

XMFLOAT3 Transform::GetUp() const
{
	return Vector3::Normalize(XMFLOAT3{ m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23 });
}

XMFLOAT3 Transform::GetRight() const
{
	return Vector3::Normalize(XMFLOAT3{ m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13 });
}

void Transform::Update()
{
	if (m_bUpdated) {

		float fPitch = XMConvertToRadians(m_xmf3Rotation.x);
		float fYaw = XMConvertToRadians(m_xmf3Rotation.y);
		float fRoll = XMConvertToRadians(m_xmf3Rotation.z);

		XMMATRIX xmmtxRotate = XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll);
		XMStoreFloat4x4(&m_xmf4x4World, xmmtxRotate);
		m_xmf4x4World._41 = m_xmf3Position.x;
		m_xmf4x4World._42 = m_xmf3Position.y;
		m_xmf4x4World._43 = m_xmf3Position.z;
	}


}