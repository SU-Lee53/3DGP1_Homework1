#include "stdafx.h"
#include "Scene.h"
#include "Mesh.h"
#include "GraphicsPipeline.h"

using namespace std;

void Scene::BuildObjects()
{
}

void Scene::ReleaseObjects()
{
}

void Scene::Animate(float fElapsedTime)
{
	m_pWallsObject->Animate(fElapsedTime);

	for (auto& pObj : m_pObjects) {
		pObj->Animate(fElapsedTime);
	}

	CheckPlayerByWallCollisions();
	CheckObjectByWallCollisions();
	CheckObjectByObjectCollisions();
	CheckObjectByBulletCollisions();
}

void Scene::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	GraphicsPipeline::SetViewport(&pCamera->GetViewport());
	GraphicsPipeline::SetViewPerspectiveProjectTransform(pCamera->GetViewPerspectiveProjectMatrix());
	m_pWallsObject->Render(hDCFrameBuffer, pCamera);
	for (auto& pObj : m_pObjects) {
		pObj->Render(hDCFrameBuffer, pCamera);
	}

	if (!m_pPlayer.expired()) {
		m_pPlayer.lock()->Render(hDCFrameBuffer, pCamera);
	}

#ifdef _WITH_DRAW_AXIS
	GraphicsPipeline::SetViewOrthographicProjectTransform(pCamera->GetViewOrthographicProjectMatrix());
	m_pWorldAxis->SetRotationTransform(m_pPlayer.lock()->GetWorldMatrix());
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif

}

void Scene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void Scene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID) {
	case WM_KEYDOWN:
		switch (wParam) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			static_pointer_cast<ExplosiveObject>(m_pObjects[int(wParam - '1')])->SetBlowingUp(TRUE);
			break;
		}

		case 'A':
			for (std::shared_ptr<GameObject>& pExplosiveObject : m_pObjects) {
				static_pointer_cast<ExplosiveObject>(pExplosiveObject)->SetBlowingUp(TRUE);
			}
			break;

		default:
			break;
		}
		break;

	default:
		break;

	}
}

void Scene::CheckObjectByObjectCollisions()
{
	std::for_each(m_pObjects.begin(), m_pObjects.end(), [](std::shared_ptr<GameObject>& p) { p->SetCollidedObject(nullptr); });

	for (int i = 0; i < m_pObjects.size(); ++i) {
		for (int j = i + 1; j < m_pObjects.size(); ++j) {
			if (m_pObjects[i]->GetOBB().Intersects(m_pObjects[j]->GetOBB())) {
				m_pObjects[i]->SetCollidedObject(m_pObjects[j]);
				m_pObjects[j]->SetCollidedObject(m_pObjects[i]);
			}
		}
	}
	
	for (shared_ptr<GameObject>& pObj : m_pObjects) {
		if (pObj->GetCollidedObject()) {
			XMFLOAT3 xmf3MovingDirection = pObj->GetMovingDirection();
			float fMovingSpeed = pObj->GetMovingSpeed();

			pObj->SetMovingDirection(pObj->GetCollidedObject()->GetMovingDirection());
			pObj->SetMovingSpeed(pObj->GetCollidedObject()->GetMovingSpeed());
			pObj->GetCollidedObject()->SetMovingSpeed(fMovingSpeed);
			pObj->GetCollidedObject()->SetCollidedObject(nullptr);
			pObj->SetCollidedObject(nullptr);
		}

	}

}

void Scene::CheckObjectByWallCollisions()
{
	for (shared_ptr<GameObject>& pObj : m_pObjects) {
		switch (m_pWallsObject->GetOBB().Contains(pObj->GetOBB())) {
		case DISJOINT:
		{
			int nPlaneIndex = -1;
			for (auto& [idx, xmf4Plane] : std::views::enumerate(m_pWallsObject->GetWallPlanes())) {
				PlaneIntersectionType intersectType = pObj->GetOBB().Intersects(XMLoadFloat4(&xmf4Plane));
				if (intersectType == BACK) {
					nPlaneIndex = idx;
					break;
				}
			}
			if (nPlaneIndex != -1) {
				XMVECTOR xmvNormal = XMVectorSetW(XMLoadFloat4(&m_pWallsObject->GetWallPlanes().at(nPlaneIndex)), 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&pObj->GetMovingDirection()), xmvNormal);
				XMFLOAT3 xmf3Reflect;
				XMStoreFloat3(&xmf3Reflect, xmvReflect);
				pObj->SetMovingDirection(xmf3Reflect);
			}
			break;
		}
		case INTERSECTS:
		{
			int nPlaneIndex = -1;
			for (auto& [idx, xmf4Plane] : std::views::enumerate(m_pWallsObject->GetWallPlanes())) {
				PlaneIntersectionType intersectType = pObj->GetOBB().Intersects(XMLoadFloat4(&xmf4Plane));
				if (intersectType == INTERSECTING) {
					nPlaneIndex = idx;
					break;
				}
			}
			if (nPlaneIndex != -1) {
				XMVECTOR xmvNormal = XMVectorSetW(XMLoadFloat4(&m_pWallsObject->GetWallPlanes().at(nPlaneIndex)), 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&pObj->GetMovingDirection()), xmvNormal);
				XMFLOAT3 xmf3Reflect;
				XMStoreFloat3(&xmf3Reflect, xmvReflect);
				pObj->SetMovingDirection(xmf3Reflect);
			}
			break;

			break;
		}
		case CONTAINS:
			break;
		}
	}
}

void Scene::CheckPlayerByWallCollisions()
{
	BoundingOrientedBox xmOBBPlayerMoveCheck;
	m_pWallsObject->GetOBB().Transform(xmOBBPlayerMoveCheck, XMLoadFloat4x4(&m_pWallsObject->GetWorldMatrix()));
	XMStoreFloat4(&xmOBBPlayerMoveCheck.Orientation, XMQuaternionNormalize(XMLoadFloat4(&xmOBBPlayerMoveCheck.Orientation)));

	if (!xmOBBPlayerMoveCheck.Intersects(m_pPlayer.lock()->GetOBB())) {
		m_pWallsObject->SetPosition(m_pPlayer.lock()->GetPosition());
	}
}

void Scene::CheckObjectByBulletCollisions()
{
	//auto pBullets = static_pointer_cast<AirplanePlayer>(m_pPlayer.lock())->GetBullets();

	//for (auto& pObj : m_pObjects) {
	//	for (auto& pBullet : pBullets) {
	//		if (pBullet->IsActive() && pObj->GetOBB().Intersects(pBullet->GetOBB())) {
	//			std::shared_ptr<ExplosiveObject> pExplosiveObject = static_pointer_cast<ExplosiveObject>(pObj);
	//			pExplosiveObject->SetBlowingUp(TRUE);
	//			pBullet->Reset();
	//		}
	//	}
	//}

}


std::shared_ptr<GameObject>& Scene::PickObjectPointedByCursor(int xClient, int yClient, std::shared_ptr<Camera> pCamera)
{
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / (float)pCamera->GetViewport().m_nWidth) - 1) / pCamera->GetPerspectiveProjectMatrix()._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / (float)pCamera->GetViewport().m_nHeight) - 1) / pCamera->GetPerspectiveProjectMatrix()._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->GetViewMatrix());

	int nIntersected = 0;
	float fNearestHitDistance = std::numeric_limits<float>::max();
	shared_ptr<GameObject> pNearestObject = nullptr;
	for (shared_ptr<GameObject>& pObj : m_pObjects) {
		float fHitDistance = std::numeric_limits<float>::max();
		nIntersected = pObj->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, fHitDistance);
		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance)) {
			fNearestHitDistance = fHitDistance;
			pNearestObject = pObj;
		}
	}

	return pNearestObject;

}
