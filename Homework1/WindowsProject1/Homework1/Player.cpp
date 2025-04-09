#include "stdafx.h"
#include "Player.h"
#include "Camera.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::SetPosition(float x, float y, float z)
{
	m_xmf3Position = XMFLOAT3{ x,y,z };
	GameObject::SetPosition(x, y, z);
}

void Player::SetRotation(float x, float y, float z)
{
}

void Player::LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, xmf3Up);
	m_xmf3Right = Vector3::Normalize(XMFLOAT3(xmf4x4View._11, xmf4x4View._21, xmf4x4View._31));
	m_xmf3Up = Vector3::Normalize(XMFLOAT3(xmf4x4View._12, xmf4x4View._22, xmf4x4View._32));
	m_xmf3Look = Vector3::Normalize(XMFLOAT3(xmf4x4View._13, xmf4x4View._23, xmf4x4View._33));
}

void Player::Move(DWORD dwDirection, float fDistance)
{
	if (dwDirection) {
		XMFLOAT3 xmf3Shift = XMFLOAT3{ 0.f, 0.f, 0.f };

		if (dwDirection & DIR_FORWARD) {
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
		}

		if (dwDirection & DIR_BACKWARD) {
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
		}

		if (dwDirection & DIR_RIGHT) {
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		}

		if (dwDirection & DIR_LEFT) {
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		}

		if (dwDirection & DIR_UP) {
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
		}

		if (dwDirection & DIR_DOWN) {
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);
		}

		Move(xmf3Shift, TRUE);
	}
}

void Player::Move(const XMFLOAT3& xmf3Shift, BOOL bUpdateVelocity)
{
	if (bUpdateVelocity) {
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else {
		m_xmf3Position = Vector3::Add(xmf3Shift, m_xmf3Position);
		m_pCamera->Move(xmf3Shift);
	}
}

void Player::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z), FALSE);
}

void Player::Rotate(float fPitch, float fYaw, float fRoll)
{
	m_pCamera->Rotate(fPitch, fYaw, fRoll);
	if (fPitch != 0.0f) {
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
	}
	if (fYaw != 0.0f) {
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}
	if (fRoll != 0.0f) {
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Up, m_xmf3Look));
	m_xmf3Up = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Look, m_xmf3Right));

	m_fPitch += fPitch;
	m_fYaw += fYaw;
	m_fRoll += fRoll;
}

void Player::SetCameraOffset(const XMFLOAT3& xmf3CameraOffset)
{
	m_xmf3CameraOffset = xmf3CameraOffset;
	m_pCamera->SetLookAt(Vector3::Add(m_xmf3Position, m_xmf3CameraOffset), m_xmf3Position, m_xmf3Up);
	m_pCamera->GenerateViewMatrix();
}

void Player::Update(float fTimeElapsed)
{
	Move(m_xmf3Velocity, FALSE);

	UpdatePlayerCamera(fTimeElapsed);
	m_pCamera->GenerateViewMatrix();

	XMFLOAT3 xmf3Deceleration = Vector3::Normalize(Vector3::ScalarProduct(m_xmf3Velocity, -1.0f));
	float fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = m_fFriction * fTimeElapsed;
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Deceleration, fDeceleration);

}

void Player::OnUpdateTransform()
{
	m_xmf4x4World._11 = m_xmf3Right.x; m_xmf4x4World._12 = m_xmf3Right.y; m_xmf4x4World._13 = m_xmf3Right.z;
	m_xmf4x4World._21 = m_xmf3Up.x; m_xmf4x4World._22 = m_xmf3Up.y; m_xmf4x4World._23 = m_xmf3Up.z;
	m_xmf4x4World._31 = m_xmf3Look.x; m_xmf4x4World._32 = m_xmf3Look.y; m_xmf4x4World._33 = m_xmf3Look.z;
	m_xmf4x4World._41 = m_xmf3Position.x; m_xmf4x4World._42 = m_xmf3Position.y; m_xmf4x4World._43 = m_xmf3Position.z;
}

void Player::Animate(float fElapsedTime)
{
	OnUpdateTransform();

	GameObject::Animate(fElapsedTime);
}

void Player::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	GameObject::Render(hDCFrameBuffer, pCamera);
}

void Player::UpdatePlayerCamera(float fTimeElapsed)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::Identity();
	mtxRotate._11 = m_xmf3Right.x; mtxRotate._21 = m_xmf3Up.x; mtxRotate._31 = m_xmf3Look.x;
	mtxRotate._12 = m_xmf3Right.y; mtxRotate._22 = m_xmf3Up.y; mtxRotate._32 = m_xmf3Look.y;
	mtxRotate._13 = m_xmf3Right.z; mtxRotate._23 = m_xmf3Up.z; mtxRotate._33 = m_xmf3Look.z;

	XMFLOAT3 xmfOffset = Vector3::TransformCoord(m_xmf3CameraOffset, mtxRotate);
	XMFLOAT3 xmf3Position = Vector3::Add(m_xmf3Position, xmfOffset);
	XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, m_pCamera->GetPosition());
	float fLength = Vector3::Length(xmf3Direction);
	xmf3Direction = Vector3::Normalize(xmf3Direction);

	float fTimeLagScale = fTimeElapsed * (1.0f / 0.25f);
	float fDistance = fLength * fTimeLagScale;
	if (fDistance > fLength) fDistance = fLength;
	if (fLength < 0.01f) fDistance = fLength;
	if (fDistance > 0) {
		m_pCamera->SetPosition(Vector3::Add(m_pCamera->GetPosition(), xmf3Direction, fDistance));
		m_pCamera->SetLookAt(m_xmf3Position, m_xmf3Up);
	}
}
