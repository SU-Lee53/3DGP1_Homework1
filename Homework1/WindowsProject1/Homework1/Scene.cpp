#include "stdafx.h"
#include "Scene.h"
#include "Mesh.h"
#include "Player.h"
#include "GraphicsPipeline.h"
#include "GameFramework.h"

using namespace std;

void Scene::BuildObjects()
{
}

void Scene::ReleaseObjects()
{
	m_pPlayer.reset();
	for (auto& pObj : m_pObjects) {
		pObj.reset();
	}
}

void Scene::Update(float fElapsedTime)
{
	// Update

	std::for_each(m_pObjects.begin(), m_pObjects.end(), [fElapsedTime](std::shared_ptr<GameObject> pObj) { pObj->Update(fElapsedTime); });
	m_pPlayer->Update(fElapsedTime);

	CheckObjectByObjectCollisions();

}

void Scene::Render(HDC hDCFrameBuffer)
{
	GraphicsPipeline::SetViewport(&m_pPlayer->GetCamera()->GetViewport());
	GraphicsPipeline::SetViewPerspectiveProjectTransform(m_pPlayer->GetCamera()->GetViewPerspectiveProjectMatrix());

	for (auto& pObj : m_pObjects) {
		pObj->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
	}

	if (m_pPlayer) {
		m_pPlayer->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
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

std::shared_ptr<GameObject> Scene::FindObjectInScene(std::string_view svName)
{
	auto it = std::find_if(m_pObjects.begin(), m_pObjects.end(), [&svName](std::shared_ptr<GameObject>& pObj) {
		return pObj->GetName() == svName;
	});

	if (it == m_pObjects.end()) return nullptr;
	

	return *it;
}
