#include "stdafx.h"
#include "Level2Scene.h"
#include "TankPlayer.h"
#include "ExplosiveObject.h"

using namespace std;

void Level2Scene::BuildObjects()
{
	shared_ptr<Mesh> pTankMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pTankMesh, L"../Tank.obj");

	shared_ptr<Mesh> pAxis = make_shared<AxisMesh>();

	shared_ptr<Mesh> pCubeMesh = make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(pCubeMesh, pTankMesh->GetOBB().Extents.x * 2, pTankMesh->GetOBB().Extents.y * 2, pTankMesh->GetOBB().Extents.z * 2);

	m_pObjects.resize(5);
	m_pObjects[0] = make_shared<ExplosiveObject>();
	m_pObjects[0]->SetColor(RGB(0, 150, 0));
	m_pObjects[0]->SetMesh(pTankMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 0.f, 15.f);
	m_pObjects[0]->SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });
	
	m_pObjects[1] = make_shared<ExplosiveObject>();
	m_pObjects[1]->SetColor(RGB(0, 150, 0));
	m_pObjects[1]->SetMesh(pTankMesh);
	m_pObjects[1]->GetTransform()->SetPosition(15.f, 0.f, 0.f);
	m_pObjects[1]->SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });
	
	m_pObjects[2] = make_shared<ExplosiveObject>();
	m_pObjects[2]->SetColor(RGB(0, 150, 0));
	m_pObjects[2]->SetMesh(pTankMesh);
	m_pObjects[2]->GetTransform()->SetPosition(0.f, 0.f, -15.f);
	m_pObjects[2]->SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });
	
	m_pObjects[3] = make_shared<ExplosiveObject>();
	m_pObjects[3]->SetColor(RGB(0, 150, 0));
	m_pObjects[3]->SetMesh(pTankMesh);
	m_pObjects[3]->GetTransform()->SetPosition(-15.f, 0.f, 0.f);
	m_pObjects[3]->SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });
	
	m_pObjects[4] = make_shared<GameObject>();
	m_pObjects[4]->SetColor(RGB(255, 0, 0));
	m_pObjects[4]->SetMesh(pAxis);
	m_pObjects[4]->GetTransform()->SetPosition(0.f, 0.f, 0.0f);

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
	ProcessMouseInput(fTimeElapsed);
	ProcessKeyboardInput(fTimeElapsed);

	if (m_pPlayer)
		m_pPlayer->Update(fTimeElapsed);

	if (!m_pObjects.empty()) {
		std::for_each(m_pObjects.begin(), m_pObjects.end(), [fTimeElapsed](std::shared_ptr<GameObject>& p) { p->Update(fTimeElapsed); });
	}

}

void Level2Scene::Render(HDC hDCFrameBuffer)
{
	std::wstring wstrOutText{ L"Level2Scene" };
	::TextOut(hDCFrameBuffer, 0, 0, wstrOutText.c_str(), wstrOutText.length());

	Scene::Render(hDCFrameBuffer);
}

void Level2Scene::ProcessMouseInput(float fTimeElapsed)
{
	if (INPUT.GetButtonDown(VK_LBUTTON)) {
		POINT ptCursorPos = INPUT.GetCurrentCursorPos();
		std::shared_ptr<GameObject> pPickedObj = PickObjectPointedByCursor(ptCursorPos.x, ptCursorPos.y, m_pPlayer->GetCamera());
		if (auto p = dynamic_pointer_cast<ExplosiveObject>(pPickedObj)) {
			pPickedObj->OnPicked();
			m_pPickedObject = pPickedObj;
		}
	}

	m_pPlayer->ProcessMouseInput(fTimeElapsed);
}

void Level2Scene::ProcessKeyboardInput(float fTimeElapsed)
{
	m_pPlayer->ProcessKeyboardInput(fTimeElapsed);
}
