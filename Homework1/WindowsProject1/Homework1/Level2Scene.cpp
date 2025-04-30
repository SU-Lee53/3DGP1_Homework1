#include "stdafx.h"
#include "Level2Scene.h"
#include "TankPlayer.h"
#include "ExplosiveObject.h"
#include "BulletObject.h"
#include "WallsObject.h"
#include "TankObject.h"
#include "ObstacleObject.h"

using namespace std;

void Level2Scene::BuildObjects()
{
	shared_ptr<Mesh> pTankMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pTankMesh, L"../Tank.obj");

	shared_ptr<Mesh> pAxis = make_shared<AxisMesh>();

	shared_ptr<Mesh> pCubeMesh = make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(pCubeMesh, pTankMesh->GetOBB().Extents.x * 2, pTankMesh->GetOBB().Extents.y * 2, pTankMesh->GetOBB().Extents.z * 2);

	// WallsObject
	{
		float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 45.0f;
		shared_ptr<Mesh> pWallMesh = make_shared<Mesh>();
		MeshHelper::CreateWallMesh(pWallMesh, fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);

		m_pWallsObject = make_shared<WallsObject>();
		m_pWallsObject->GetTransform()->SetPosition(0.0f, fHalfHeight, 0.0f);
		m_pWallsObject->SetMesh(std::move(pWallMesh));
		m_pWallsObject->SetColor(RGB(0, 0, 0));
		m_pWallsObject->SetWallPlane(0, XMFLOAT4{ +1.0f, 0.0f, 0.0f, fHalfWidth });
		m_pWallsObject->SetWallPlane(1, XMFLOAT4{ -1.0f, 0.0f, 0.0f, fHalfWidth });
		m_pWallsObject->SetWallPlane(2, XMFLOAT4{ 0.0f, +1.0f, 0.0f, fHalfHeight });
		m_pWallsObject->SetWallPlane(3, XMFLOAT4{ 0.0f, -1.0f, 0.0f, fHalfHeight });
		m_pWallsObject->SetWallPlane(4, XMFLOAT4{ 0.0f, 0.0f, +1.0f, fHalfDepth });
		m_pWallsObject->SetWallPlane(5, XMFLOAT4{ 0.0f, 0.0f, -1.0f, fHalfDepth });
		m_pWallsObject->SetOBB(BoundingOrientedBox{ XMFLOAT3{0.f, 0.f, 0.f}, XMFLOAT3{fHalfWidth, fHalfHeight, fHalfDepth * 0.05f}, XMFLOAT4{0.f, 0.f, 0.f, 1.f} });
	}

	m_pObjects.resize(10);


	// Enemy Tank
	{
		m_pObjects[0] = make_shared<TankObject>();
		m_pObjects[0]->SetColor(RGB(0, 150, 0));
		m_pObjects[0]->SetMesh(pTankMesh);
		m_pObjects[0]->GetTransform()->SetPosition(0.f, 0.f, 15.f);
		m_pObjects[0]->SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });

		m_pObjects[1] = make_shared<TankObject>();
		m_pObjects[1]->SetColor(RGB(0, 150, 0));
		m_pObjects[1]->SetMesh(pTankMesh);
		m_pObjects[1]->GetTransform()->SetPosition(15.f, 0.f, 0.f);
		m_pObjects[1]->SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });

		m_pObjects[2] = make_shared<TankObject>();
		m_pObjects[2]->SetColor(RGB(0, 150, 0));
		m_pObjects[2]->SetMesh(pTankMesh);
		m_pObjects[2]->GetTransform()->SetPosition(0.f, 0.f, -15.f);
		m_pObjects[2]->SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });

		m_pObjects[3] = make_shared<TankObject>();
		m_pObjects[3]->SetColor(RGB(0, 150, 0));
		m_pObjects[3]->SetMesh(pTankMesh);
		m_pObjects[3]->GetTransform()->SetPosition(-15.f, 0.f, 0.f);
		m_pObjects[3]->SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });

	}

	// Obstacle
	{
		m_pObjects[4] = make_shared<ObstacleObject>();
		m_pObjects[4]->SetColor(RGB(255, 0, 0));
		m_pObjects[4]->Initialize();
		
		m_pObjects[5] = make_shared<ObstacleObject>();
		m_pObjects[5]->SetColor(RGB(255, 0, 0));
		m_pObjects[5]->Initialize();
		
		m_pObjects[6] = make_shared<ObstacleObject>();
		m_pObjects[6]->SetColor(RGB(255, 0, 0));
		m_pObjects[6]->Initialize();
		
		m_pObjects[7] = make_shared<ObstacleObject>();
		m_pObjects[7]->SetColor(RGB(255, 0, 0));
		m_pObjects[7]->Initialize();
		
		m_pObjects[8] = make_shared<ObstacleObject>();
		m_pObjects[8]->SetColor(RGB(255, 0, 0));
		m_pObjects[8]->Initialize();
		
		m_pObjects[9] = make_shared<ObstacleObject>();
		m_pObjects[9]->SetColor(RGB(255, 0, 0));
		m_pObjects[9]->Initialize();

	}

	m_pPlayer = make_shared<TankPlayer>();
	m_pPlayer->Initialize();
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);

	ExplosiveObject::PrepareExplosion();
}

void Level2Scene::ReleaseObjects()
{
	Scene::ReleaseObjects();
}

void Level2Scene::Update(float fTimeElapsed)
{
	if (m_pPlayer)
		m_pPlayer->GetTransform()->CacheLastFrameMovement();

	ProcessMouseInput(fTimeElapsed);
	ProcessKeyboardInput(fTimeElapsed);

	if (m_pPlayer)
		m_pPlayer->Update(fTimeElapsed);

	if (!m_pObjects.empty()) {
		std::for_each(m_pObjects.begin(), m_pObjects.end(), [fTimeElapsed](std::shared_ptr<GameObject>& p) { p->Update(fTimeElapsed); });
	}
	
	m_pWallsObject->Update(fTimeElapsed);

	CheckPlayerByWallCollisions();
	CheckObjectByWallCollisions();
	CheckObjectByObjectCollisions();
	CheckObjectByBulletCollisions();
	CheckPlayerByObjectCollisions();

}

void Level2Scene::Render(HDC hDCFrameBuffer)
{
	std::wstring wstrOutText{ L"Level2Scene" };
	::TextOut(hDCFrameBuffer, 0, 0, wstrOutText.c_str(), wstrOutText.length());


	Scene::UpdatePipelineVaribles(m_pPlayer->GetCamera());
	m_pWallsObject->Render(hDCFrameBuffer, m_pPlayer->GetCamera());
	Scene::Render(hDCFrameBuffer);
}

void Level2Scene::ProcessMouseInput(float fTimeElapsed)
{
	if (INPUT.GetButtonDown(VK_RBUTTON)) {
		POINT ptCursorPos = INPUT.GetCurrentCursorPos();
		std::shared_ptr<GameObject> pPickedObj = PickObjectPointedByCursor(ptCursorPos.x, ptCursorPos.y, m_pPlayer->GetCamera());
		if (auto p = dynamic_pointer_cast<ExplosiveObject>(pPickedObj)) {
			//pPickedObj->OnPicked();
			//m_pPickedObject = pPickedObj;
			static_pointer_cast<TankPlayer>(m_pPlayer)->FireBullet(p);
		}
	}

	m_pPlayer->ProcessMouseInput(fTimeElapsed);
}

void Level2Scene::ProcessKeyboardInput(float fTimeElapsed)
{
	m_pPlayer->ProcessKeyboardInput(fTimeElapsed);
}

void Level2Scene::CheckObjectByBulletCollisions()
{
	auto pBullets = static_pointer_cast<TankPlayer>(m_pPlayer)->GetBullets();

	for (auto& pObj : m_pObjects) {
		for (auto& pBullet : pBullets) {
			if (pBullet->IsActive() && pObj->GetOBB().Intersects(pBullet->GetOBB())) {
				pObj->OnCollision(pBullet);
				pBullet->OnCollision(pObj);
			}
		}
	}
}

void Level2Scene::CheckObjectByWallCollisions()
{
	for (shared_ptr<GameObject>& pObj : m_pObjects) {
		if (auto p = dynamic_pointer_cast<TankObject>(pObj))
		{
			switch (m_pWallsObject->GetOBB().Contains(p->GetOBB())) {
			case DISJOINT:
			{
				int nPlaneIndex = -1;
				for (auto& [idx, xmf4Plane] : std::views::enumerate(m_pWallsObject->GetWallPlanes())) {
					PlaneIntersectionType intersectType = p->GetOBB().Intersects(XMLoadFloat4(&xmf4Plane));
					if (intersectType == BACK) {
						nPlaneIndex = idx;
						break;
					}
				}
				if (nPlaneIndex != -1) {
					XMVECTOR xmvNormal = XMVectorSetW(XMLoadFloat4(&m_pWallsObject->GetWallPlanes().at(nPlaneIndex)), 0.0f);
					XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&p->GetMovingDirection()), xmvNormal);
					XMFLOAT3 xmf3Reflect;
					XMStoreFloat3(&xmf3Reflect, xmvReflect);
					p->SetMovingDirection(xmf3Reflect);
				}
				break;
			}
			case INTERSECTS:
			{
				int nPlaneIndex = -1;
				for (auto& [idx, xmf4Plane] : std::views::enumerate(m_pWallsObject->GetWallPlanes())) {
					PlaneIntersectionType intersectType = p->GetOBB().Intersects(XMLoadFloat4(&xmf4Plane));
					if (intersectType == INTERSECTING) {
						nPlaneIndex = idx;
						break;
					}
				}
				if (nPlaneIndex != -1) {
					XMVECTOR xmvNormal = XMVectorSetW(XMLoadFloat4(&m_pWallsObject->GetWallPlanes().at(nPlaneIndex)), 0.0f);
					XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&p->GetMovingDirection()), xmvNormal);
					XMFLOAT3 xmf3Reflect;
					XMStoreFloat3(&xmf3Reflect, xmvReflect);
					p->SetMovingDirection(xmf3Reflect);
				}
				break;

				break;
			}
			case CONTAINS:
				break;
			}
		}
	}
}

void Level2Scene::CheckPlayerByWallCollisions()
{
	BoundingOrientedBox xmOBBPlayerMoveCheck;
	m_pWallsObject->GetOBB().Transform(xmOBBPlayerMoveCheck, XMLoadFloat4x4(&m_pWallsObject->GetTransform()->GetWorldMatrix()));
	XMStoreFloat4(&xmOBBPlayerMoveCheck.Orientation, XMQuaternionNormalize(XMLoadFloat4(&xmOBBPlayerMoveCheck.Orientation)));

	if (!xmOBBPlayerMoveCheck.Intersects(m_pPlayer->GetOBB())) {
		m_pPlayer->OnCollision(m_pWallsObject);
	}
}

void Level2Scene::CheckObjectByObjectCollisions()
{
	for (int i = 0; i < m_pObjects.size(); ++i) {
		for (int j = i + 1; j < m_pObjects.size(); ++j) {
			if (m_pObjects[i]->GetOBB().Intersects(m_pObjects[j]->GetOBB())) {
				m_pObjects[i]->OnCollision(m_pObjects[j]);
				m_pObjects[j]->OnCollision(m_pObjects[i]);
			}
		}
	}
}

void Level2Scene::CheckPlayerByObjectCollisions()
{
	for (shared_ptr<GameObject> pObj : m_pObjects) {
		if (m_pPlayer->GetOBB().Intersects(pObj->GetOBB())) {
			m_pPlayer->OnCollision(pObj);
			pObj->OnCollision(m_pPlayer);
		}
	}
}
