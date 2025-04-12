#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"
#include "Player.h"

using namespace std;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::SetFOVAngle(float fAngle)
{
	m_fFOVAngle = fAngle;
	m_fProjectPlaneDistance = float(1.0f / tan(XMConvertToRadians(m_fFOVAngle * 0.5f)));
	m_bProjectionUpdated = TRUE;
}

void Camera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight)
{
	m_Viewport.SetViewport(xTopLeft, yTopLeft, nWidth, nHeight);
	m_fAspectRatio = float(m_Viewport.m_nWidth) / float(m_Viewport.m_nHeight);
	m_bProjectionUpdated = TRUE;
}

void Camera::SetNearZ(float fNearZ)
{
	m_fNearZ = fNearZ;
	m_bProjectionUpdated = TRUE;
}

void Camera::SetFarZ(float fFarZ)
{
	m_fFarZ = fFarZ;
	m_bProjectionUpdated = TRUE;
}

void Camera::SetPosition(const XMFLOAT3& xmf3NewPosition)
{
	XMVECTOR xmvNewPos = XMLoadFloat3(&xmf3NewPosition);
	if (!m_wpOwner.expired()) {
		XMVECTOR xmvOffset = XMLoadFloat3(&m_wpOwner.lock()->GetCameraOffset());
		xmvNewPos = XMVectorAdd(xmvNewPos, xmvOffset);
	}

	if (m_pTransform->SetPosition(xmvNewPos)){
		m_bViewUpdated = TRUE;
	};
}

void Camera::SetPosition(const XMVECTOR& xmvNewPosition)
{
	XMVECTOR xmvNewPos = xmvNewPosition;
	if (!m_wpOwner.expired()) {
		XMVECTOR xmvOffset = XMLoadFloat3(&m_wpOwner.lock()->GetCameraOffset());
		xmvNewPos = XMVectorAdd(xmvNewPos, xmvOffset);
	}

	if (m_pTransform->SetPosition(xmvNewPos)){
		m_bViewUpdated = TRUE;
	}
}

void Camera::SetPosition(float fXPos, float fYPos, float fZPos)
{
	XMVECTOR xmvNewPos = XMVectorSet(fXPos, fYPos, fZPos, 1.f);
	if (!m_wpOwner.expired()) {
		XMVECTOR xmvOffset = XMLoadFloat3(&m_wpOwner.lock()->GetCameraOffset());
		xmvNewPos = XMVectorAdd(xmvNewPos, xmvOffset);
	}

	if (m_pTransform->SetPosition(xmvNewPos)){
		m_bViewUpdated = TRUE;
	};
}

void Camera::SetRotation(const XMFLOAT3& xmf3NewRotation)
{
	if (m_pTransform->SetRotation(xmf3NewRotation)) {
		m_bViewUpdated = TRUE;
	}
}

void Camera::SetRotation(const XMVECTOR& xmvNewRotation)
{
	if (m_pTransform->SetRotation(xmvNewRotation)) {
		m_bViewUpdated = TRUE;
	}
}

void Camera::SetRotation(float fPitch, float fYaw, float fRoll)
{
	if (m_pTransform->SetRotation(fPitch, fYaw, fRoll)) {
		m_bViewUpdated = TRUE;
	}
}

XMFLOAT4X4& Camera::GetViewPerspectiveProjectMatrix()
{
	XMMATRIX xmmtxViewPerspectiveProject =
		XMMatrixMultiply(XMLoadFloat4x4(&m_xmf4x4View), XMLoadFloat4x4(&m_xmf4x4PerspectiveProject));
	XMStoreFloat4x4(&m_xmf4x4ViewPerspectiveProject, xmmtxViewPerspectiveProject);
	return m_xmf4x4ViewPerspectiveProject;
}

XMFLOAT4X4& Camera::GetViewOrthographicProjectMatrix()
{
	XMMATRIX xmmtxViewOrthographicProject =
		XMMatrixMultiply(XMLoadFloat4x4(&m_xmf4x4View), XMLoadFloat4x4(&m_xmf4x4OrthographicProject));
	XMStoreFloat4x4(&m_xmf4x4ViewOrthographicProject, xmmtxViewOrthographicProject);
	return m_xmf4x4ViewOrthographicProject;
}

void Camera::Initialize(shared_ptr<Player> pOwnerPlayer)
{
	m_wpOwner = pOwnerPlayer;
	m_pTransform = make_shared<Transform>();
}

void Camera::Update()
{
	if (!m_wpOwner.expired()) { // If camera owned by someone(Player)
		if (m_bViewUpdated) {
			GenerateViewMatrix();
			m_bViewUpdated = FALSE;
		}

		if (m_bProjectionUpdated) {
			GeneratePerspectiveProjectionMatrix();
			GenerateOrthographicProjectionMatrix();
			m_bProjectionUpdated = FALSE;
		}
	}
	else {	// If camera stands only
		if (m_bViewUpdated) {
			GenerateViewMatrix();
			m_bViewUpdated = FALSE;
		}
		
		if (m_bProjectionUpdated) {
			GeneratePerspectiveProjectionMatrix();
			GenerateOrthographicProjectionMatrix();
			m_bProjectionUpdated = FALSE;
		}
	}
}

void Camera::GenerateViewMatrix()
{
	XMFLOAT3 xmf3Look = Vector3::Normalize(m_pTransform->GetLook());
	XMFLOAT3 xmf3Right = Vector3::Normalize(Vector3::CrossProduct(m_pTransform->GetUp(), xmf3Look));
	XMFLOAT3 xmf3Up = Vector3::Normalize(Vector3::CrossProduct(xmf3Look, xmf3Right));
	m_xmf4x4View._11 = xmf3Right.x; m_xmf4x4View._12 = xmf3Up.x; m_xmf4x4View._13 = xmf3Look.x;
	m_xmf4x4View._21 = xmf3Right.y; m_xmf4x4View._22 = xmf3Up.y; m_xmf4x4View._23 = xmf3Look.y;
	m_xmf4x4View._31 = xmf3Right.z; m_xmf4x4View._32 = xmf3Up.z; m_xmf4x4View._33 = xmf3Look.z;
	m_xmf4x4View._41 = -Vector3::DotProduct(m_pTransform->GetPosition(), xmf3Right);
	m_xmf4x4View._42 = -Vector3::DotProduct(m_pTransform->GetPosition(), xmf3Up);
	m_xmf4x4View._43 = -Vector3::DotProduct(m_pTransform->GetPosition(), xmf3Look);

	m_xmf4x4ViewPerspectiveProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4PerspectiveProject);
	m_xmf4x4OrthographicProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4OrthographicProject);

	m_xmf4x4InverseView._11 = xmf3Right.x;			m_xmf4x4InverseView._12 = xmf3Right.y;			m_xmf4x4InverseView._13 = xmf3Right.z;
	m_xmf4x4InverseView._21 = xmf3Up.x;				m_xmf4x4InverseView._22 = xmf3Up.y;				m_xmf4x4InverseView._23 = xmf3Up.z;
	m_xmf4x4InverseView._31 = xmf3Look.x;			m_xmf4x4InverseView._32 = xmf3Look.y;			m_xmf4x4InverseView._33 = xmf3Look.z;
	m_xmf4x4InverseView._41 = m_pTransform->GetPosition().x;		
	m_xmf4x4InverseView._42 = m_pTransform->GetPosition().y;		
	m_xmf4x4InverseView._43 = m_pTransform->GetPosition().z;

	m_xmFrustumView.Transform(m_xmFrustumWorld, XMLoadFloat4x4(&m_xmf4x4InverseView));
}

void Camera::GeneratePerspectiveProjectionMatrix()
{
	XMMATRIX xmmtxPerspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fFOVAngle), m_fAspectRatio, m_fNearZ, m_fFarZ);
	XMStoreFloat4x4(&m_xmf4x4PerspectiveProject, xmmtxPerspective);
	BoundingFrustum::CreateFromMatrix(m_xmFrustumView, xmmtxPerspective);
}

void Camera::GenerateOrthographicProjectionMatrix()
{
	XMMATRIX xmmtxOrthographic = XMMatrixOrthographicLH(m_Viewport.m_nWidth, m_Viewport.m_nHeight, m_fNearZ, m_fFarZ);
	XMStoreFloat4x4(&m_xmf4x4OrthographicProject, xmmtxOrthographic);
}

bool Camera::IsInFrustum(const BoundingOrientedBox& xmBoundingBox) const
{
	return m_xmFrustumWorld.Intersects(xmBoundingBox);
}
