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

BOOL Transform::SetRotationEuler(const XMFLOAT3& xmf3NewRotation)
{
	return SetRotationEuler(XMLoadFloat3(&xmf3NewRotation));
}

BOOL Transform::SetRotationEuler(const XMVECTOR& xmvNewRotation)
{
	XMVECTOR xmvRotation = XMLoadFloat3(&m_xmf3RotationEuler);

	if (!XMVector3Equal(xmvRotation, xmvNewRotation)) {
		XMStoreFloat3(&m_xmf3RotationEuler, xmvNewRotation);
		m_bUpdated = TRUE;
	}

	return m_bUpdated;
}

BOOL Transform::SetRotationEuler(float fPitch, float fYaw, float fRoll)
{
	return SetRotationEuler(XMVectorSet(fPitch, fYaw, fRoll, 0.f));
}

BOOL Transform::SetRotationAxisAngle(const XMFLOAT3& xmf3Axis, float fAngle)
{
	return SetRotationAxisAngle(XMLoadFloat3(&xmf3Axis), fAngle);
}

BOOL Transform::SetRotationAxisAngle(const XMVECTOR& xmvAxis, float fAngle)
{
	BOOL bResult = FALSE;
	if (!XMVector3Equal(xmvAxis, XMLoadFloat3(&m_AxisAngle.m_xmf3Axis))) {
		XMStoreFloat3(&m_AxisAngle.m_xmf3Axis, xmvAxis);
		bResult = TRUE;
	}

	if (fAngle != m_AxisAngle.m_fAngle) {
		fAngle = m_AxisAngle.m_fAngle;
		bResult = TRUE;
	}
	
	return bResult;
}

BOOL Transform::AddRotationEuler(const XMFLOAT3& xmf3AddRotation)
{
	return AddRotationEuler(XMLoadFloat3(&xmf3AddRotation));
}

BOOL Transform::AddRotationEuler(const XMVECTOR& xmvAddRotation)
{
	XMVECTOR xmvCurRotation = XMLoadFloat3(&m_xmf3RotationEuler);
	XMVECTOR xmvRotation = XMVectorAdd(xmvCurRotation, xmvAddRotation);

	XMStoreFloat3(&m_xmf3RotationEuler, xmvRotation);
	m_bUpdated = TRUE;

	return m_bUpdated;
}

BOOL Transform::AddRotationEuler(float fPitch, float fYaw, float fRoll)
{
	return AddRotationEuler(XMVectorSet(fPitch, fYaw, fRoll, 0.f));
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

		float fPitch = XMConvertToRadians(m_xmf3RotationEuler.x);
		float fYaw = XMConvertToRadians(m_xmf3RotationEuler.y);
		float fRoll = XMConvertToRadians(m_xmf3RotationEuler.z);

		XMMATRIX xmmtxRotate1 = XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll);
		XMMATRIX xmmtxRotate2 = XMMatrixRotationAxis(XMLoadFloat3(&m_AxisAngle.m_xmf3Axis), XMConvertToRadians(m_AxisAngle.m_fAngle));
		XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(xmmtxRotate1, xmmtxRotate2));

		m_xmf4x4World._41 = m_xmf3Position.x;
		m_xmf4x4World._42 = m_xmf3Position.y;
		m_xmf4x4World._43 = m_xmf3Position.z;
	}

}