#include "stdafx.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Camera.h"
#include "GraphicsPipeline.h"

void GameObject::LookTo(const XMFLOAT3& xmf3LookTo, const XMFLOAT3& xmf3Up)
{
	/*XMFLOAT4X4 xmf4x4View = Matrix4x4::LookToLH(GetPosition(), xmf3LookTo, xmf3Up);
	m_xmf4x4World = Matrix4x4::Transpose(xmf4x4View);*/
}

void GameObject::LookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up)
{
	/*XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(GetPosition(), xmf3LookAt, xmf3Up);
	m_xmf4x4World = Matrix4x4::Transpose(xmf4x4View);*/
}

void GameObject::UpdateBoundingBox()
{
	if (m_pMesh) {
		m_pMesh->GetOBB().Transform(m_xmOBB, XMLoadFloat4x4(&m_pTransform->GetWorldMatrix()));
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

void GameObject::Update(float fElapsedTime)
{
	m_pTransform->Update();
	UpdateBoundingBox();
}

void GameObject::Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera)
{
	if (pCamera->IsInFrustum(m_xmOBB)) {
		GameObject::Render(hDCFrameBuffer, m_pTransform->GetWorldMatrix(), m_pMesh);
	}
}

void GameObject::GenerateRayForPicking(XMVECTOR& xmvPickPosition, const XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) const
{
	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_pTransform->GetWorldMatrix()) * xmmtxView);

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
