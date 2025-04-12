#include "stdafx.h"
#include "Transform.h"

BOOL Transform::SetPosition(const XMFLOAT3& xmf3NewPosition)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMVECTOR xmvNewPosition = XMLoadFloat3(&xmf3NewPosition);

	if (!XMVector3Equal(xmvPosition, xmvNewPosition)) {
		XMStoreFloat3(&m_xmf3Position, xmvNewPosition);
		m_bPositionUpdated = TRUE;
	}

	return m_bPositionUpdated;
}

BOOL Transform::SetPosition(const XMVECTOR& xmvNewPosition)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);

	if (!XMVector3Equal(xmvPosition, xmvNewPosition)) {
		XMStoreFloat3(&m_xmf3Position, xmvNewPosition);
		m_bPositionUpdated = TRUE;
	}

	return m_bPositionUpdated;
}

BOOL Transform::SetPosition(float fXPos, float fYPos, float fZPos)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMVECTOR xmvNewPosition = XMVectorSet(fXPos, fYPos, fZPos, 1.0f);

	if (!XMVector3Equal(xmvPosition, xmvNewPosition)) {
		XMStoreFloat3(&m_xmf3Position, xmvNewPosition);
		m_bPositionUpdated = TRUE;
	}

	return m_bPositionUpdated;
}

BOOL Transform::AddPosition(const XMFLOAT3& xmf3AddPosition)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMVECTOR xmvAddPosition = XMLoadFloat3(&xmf3AddPosition);
	XMStoreFloat3(&m_xmf3Position, XMVectorAdd(xmvPosition, xmvAddPosition));
	m_bPositionUpdated = TRUE;
	return m_bPositionUpdated;
}

BOOL Transform::AddPosition(const XMVECTOR& xmvAddPosition)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMStoreFloat3(&m_xmf3Position, XMVectorAdd(xmvPosition, xmvAddPosition));
	m_bPositionUpdated = TRUE;
	return m_bPositionUpdated;
}

BOOL Transform::AddPosition(float fXPos, float fYPos, float fZPos)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMVECTOR xmvAddPosition = XMVectorSet(fXPos, fYPos, fZPos, 1.0f);
	XMStoreFloat3(&m_xmf3Position, XMVectorAdd(xmvPosition, xmvAddPosition));
	m_bPositionUpdated = TRUE;
	return m_bPositionUpdated;
}

BOOL Transform::SetRotation(const XMFLOAT3& xmf3NewRotation)
{
	XMVECTOR xmvRotation = XMLoadFloat3(&m_xmf3Rotation);
	XMVECTOR xmvNewRotation = XMLoadFloat3(&xmf3NewRotation);

	if (!XMVector3Equal(xmvRotation, xmvNewRotation)) {
		m_xmf3Rotation = xmf3NewRotation;
		m_bRotationUpdated = TRUE;

		UpdateBasis();
	}

	return m_bRotationUpdated;
}

BOOL Transform::SetRotation(const XMVECTOR& xmvNewRotation)
{
	XMVECTOR xmvRotation = XMLoadFloat3(&m_xmf3Rotation);

	if (!XMVector3Equal(xmvRotation, xmvNewRotation)) {
		XMStoreFloat3(&m_xmf3Rotation, xmvNewRotation);
		m_bRotationUpdated = TRUE;

		UpdateBasis();
	}

	return m_bRotationUpdated;
}

BOOL Transform::SetRotation(float fPitch, float fYaw, float fRoll)
{
	XMVECTOR xmvRotation = XMLoadFloat3(&m_xmf3Rotation);
	XMVECTOR xmvNewRotation = XMVectorSet(fPitch, fYaw, fRoll, 0.f);

	if (!XMVector3Equal(xmvRotation, xmvNewRotation)) {
		XMStoreFloat3(&m_xmf3Rotation, xmvNewRotation);
		m_bRotationUpdated = TRUE;

		UpdateBasis();
	}

	return m_bRotationUpdated;
}

BOOL Transform::AddRotation(const XMFLOAT3& xmf3AddRotation)
{
	XMVECTOR xmvRotation = XMLoadFloat3(&m_xmf3Rotation);
	XMVECTOR xmvAddRotation = XMLoadFloat3(&xmf3AddRotation);
	XMStoreFloat3(&m_xmf3Rotation, XMVectorAdd(xmvRotation, xmvAddRotation));
	m_bRotationUpdated = TRUE;

	UpdateBasis();

	return m_bRotationUpdated;
}

BOOL Transform::AddRotation(const XMVECTOR& xmvAddRotation)
{
	XMVECTOR xmvRotation = XMLoadFloat3(&m_xmf3Rotation);
	XMStoreFloat3(&m_xmf3Rotation, XMVectorAdd(xmvRotation, xmvAddRotation));
	m_bRotationUpdated = TRUE;

	UpdateBasis();

	return m_bRotationUpdated;
}

BOOL Transform::AddRotation(float fPitch, float fYaw, float fRoll)
{
	XMVECTOR xmvRotation = XMLoadFloat3(&m_xmf3Rotation);
	XMVECTOR xmvAddRotation = XMVectorSet(fPitch, fYaw, fRoll, 0.f);
	XMStoreFloat3(&m_xmf3Rotation, XMVectorAdd(xmvRotation, xmvAddRotation));
	m_bRotationUpdated = TRUE;

	UpdateBasis();

	return m_bRotationUpdated;
}

void Transform::Update()
{/*
	if (m_bPositionUpdated) {
		m_xmf4x4World._41 = m_xmf3Position.x; m_xmf4x4World._42 = m_xmf3Position.y; m_xmf4x4World._43 = m_xmf3Position.z;

		m_bPositionUpdated = FALSE;
	}

	if (m_bRotationUpdated) {
		m_xmf4x4World._11 = m_xmf3Right.x; m_xmf4x4World._12 = m_xmf3Right.y; m_xmf4x4World._13 = m_xmf3Right.z;
		m_xmf4x4World._21 = m_xmf3Up.x; m_xmf4x4World._22 = m_xmf3Up.y; m_xmf4x4World._23 = m_xmf3Up.z;
		m_xmf4x4World._31 = m_xmf3Look.x; m_xmf4x4World._32 = m_xmf3Look.y; m_xmf4x4World._33 = m_xmf3Look.z;

		m_bRotationUpdated = FALSE;
	}*/

	//float fPitch = XMConvertToRadians(m_xmf3Rotation.x);
	//float fYaw = XMConvertToRadians(m_xmf3Rotation.y);
	//float fRoll = XMConvertToRadians(m_xmf3Rotation.z);
	//XMMATRIX xmmtxWorld;

	XMMATRIX xmmtxRotX = XMMatrixRotationX(XMConvertToRadians(m_xmf3Rotation.x));
	XMMATRIX xmmtxRotY = XMMatrixRotationY(XMConvertToRadians(m_xmf3Rotation.y));
	XMMATRIX xmmtxRotZ = XMMatrixRotationZ(XMConvertToRadians(m_xmf3Rotation.z));
	XMMATRIX xmmtxRotation = XMMatrixMultiply(XMMatrixMultiply(xmmtxRotX, xmmtxRotY), xmmtxRotZ);

	XMMATRIX xmmtxWorld = XMMatrixMultiply(xmmtxRotation, XMMatrixTranslationFromVector(XMLoadFloat3(&m_xmf3Position)));

	XMStoreFloat4x4(&m_xmf4x4World, xmmtxWorld);
}


void Transform::UpdateBasis()
{
	float fPitch = XMConvertToRadians(m_xmf3Rotation.x);
	float fYaw = XMConvertToRadians(m_xmf3Rotation.y);
	float fRoll = XMConvertToRadians(m_xmf3Rotation.z);

	XMMATRIX xmmtxRotate = XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll);
	XMVECTOR xmvLook = XMVector3Normalize(XMVector3TransformNormal(XMVectorSet(0.f, 0.f, 1.f, 0.f), xmmtxRotate));
	XMVECTOR xmvUp = XMVector3Normalize(XMVector3TransformNormal(XMVectorSet(0.f, 1.f, 0.f, 0.f), xmmtxRotate));
	XMVECTOR xmvRight = XMVector3Normalize(XMVector3Cross(xmvUp, xmvLook));

	XMStoreFloat3(&m_xmf3Look, xmvLook);
	XMStoreFloat3(&m_xmf3Up, xmvUp);
	XMStoreFloat3(&m_xmf3Right, xmvRight);
}
