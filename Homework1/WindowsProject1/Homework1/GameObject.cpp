#include "stdafx.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Camera.h"
#include "GraphicsPipeline.h"

GameObject::GameObject()
{
	m_pTransform = std::make_shared<Transform>();
}

void GameObject::SetMesh(const std::shared_ptr<Mesh>& pMesh)
{
	m_pMesh = pMesh;

#ifdef _DEBUG_COLLISION
	m_pMeshOBBMesh = std::make_shared<Mesh>();
	XMFLOAT3 xmf3OBBExtents = pMesh->GetOBB().Extents;
	MeshHelper::CreateCubeMesh(m_pMeshOBBMesh, xmf3OBBExtents.x * 2, xmf3OBBExtents.y * 2, xmf3OBBExtents.z * 2);
	
	m_pObjectOBBMesh = std::make_shared<Mesh>();
	xmf3OBBExtents = m_xmOBB.Extents;
	MeshHelper::CreateCubeMesh(m_pObjectOBBMesh, xmf3OBBExtents.x * 2, xmf3OBBExtents.y * 2, xmf3OBBExtents.z * 2);
#endif
}

void GameObject::SetMesh(std::shared_ptr<Mesh>&& pMesh)
{
	m_pMesh = std::move(pMesh);

#ifdef _DEBUG_COLLISION
	m_pMeshOBBMesh = std::make_shared<Mesh>();
	XMFLOAT3 xmf3OBBExtents = pMesh->GetOBB().Extents;
	MeshHelper::CreateCubeMesh(m_pMeshOBBMesh, xmf3OBBExtents.x, xmf3OBBExtents.y, xmf3OBBExtents.z);

	m_pObjectOBBMesh = std::make_shared<Mesh>();
	xmf3OBBExtents = m_xmOBB.Extents;
	MeshHelper::CreateCubeMesh(m_pObjectOBBMesh, xmf3OBBExtents.x, xmf3OBBExtents.y, xmf3OBBExtents.z);
#endif
}

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
		XMFLOAT4X4 xmf4x4FinalWorld = m_pTransform->GetWorldMatrix();
		if (!XMVector3Equal(XMLoadFloat3(&m_xmf3DefaultOrientation), XMVectorZero())) {
			XMMATRIX xmmtxCurrentWorld = XMLoadFloat4x4(&m_pTransform->GetWorldMatrix());
			XMMATRIX xmmtxAdditionalRotation = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(m_xmf3DefaultOrientation.x),
				XMConvertToRadians(m_xmf3DefaultOrientation.y),
				XMConvertToRadians(m_xmf3DefaultOrientation.z)
			);
			XMStoreFloat4x4(&xmf4x4FinalWorld, XMMatrixMultiply(xmmtxAdditionalRotation, xmmtxCurrentWorld));
		}

		m_pMesh->GetOBB().Transform(m_xmOBB, XMLoadFloat4x4(&xmf4x4FinalWorld));
		XMStoreFloat4(&m_xmOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOBB.Orientation)));
	}
}

void GameObject::Render(HDC hDCFrameBuffer, XMFLOAT4X4& xmf4x4World, std::shared_ptr<Mesh> pMesh) const
{
	if (pMesh && m_bActive) {
		XMFLOAT4X4 xmf4x4FinalWorld = xmf4x4World;
		if (!XMVector3Equal(XMLoadFloat3(&m_xmf3DefaultOrientation), XMVectorZero())) {
			XMMATRIX xmmtxCurrentWorld = XMLoadFloat4x4(&xmf4x4World);
			XMMATRIX xmmtxAdditionalRotation = XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(m_xmf3DefaultOrientation.x),
				XMConvertToRadians(m_xmf3DefaultOrientation.y),
				XMConvertToRadians(m_xmf3DefaultOrientation.z)
			);
			XMStoreFloat4x4(&xmf4x4FinalWorld, XMMatrixMultiply(xmmtxAdditionalRotation, xmmtxCurrentWorld));
		}

		GraphicsPipeline::SetWorldTransform(xmf4x4FinalWorld);

		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_Color);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		pMesh->Render(hDCFrameBuffer);

#ifdef _DEBUG_COLLISION
		if (m_pMeshOBBMesh) {
			m_pMeshOBBMesh->Render(hDCFrameBuffer);
		}
		if (m_pObjectOBBMesh) {
			m_pObjectOBBMesh->Render(hDCFrameBuffer);
		}
#endif


		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

void GameObject::Initialize()
{
}

void GameObject::Update(float fElapsedTime)
{
	if (m_bActive) {
		m_pTransform->Update();
		UpdateBoundingBox();
	}

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
