#include "stdafx.h"
#include "MenuScene.h"
#include "FirstPersonPlayer.h"
#include "ExplosiveObject.h"

using namespace std;

void MenuScene::BuildObjects()
{
	m_bSceneChanged = FALSE;

	shared_ptr<Mesh> pTutorialMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pTutorialMesh, L"../tutorial.obj");

	shared_ptr<Mesh> pLevel1Mesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pLevel1Mesh, L"../Level-1.obj");
	
	shared_ptr<Mesh> pLevel2Mesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pLevel2Mesh, L"../Level-2.obj");

	shared_ptr<Mesh> pStartMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pStartMesh, L"../Start.obj");
	
	shared_ptr<Mesh> pEndMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pEndMesh, L"../End.obj");

	m_pObjects.resize(5);
	m_pObjects[0] = make_shared<GameObject>();
	m_pObjects[0]->SetColor(RandomGenerator::GenerateRandomColor());
	m_pObjects[0]->SetMesh(pTutorialMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 20.f, 70.f);
	m_pObjects[0]->GetTransform()->SetRotationEuler(90.f, 0.f, 0.f);

	m_pObjects[1] = make_shared<ExplosiveObject>();
	m_pObjects[1]->SetColor(RandomGenerator::GenerateRandomColor());
	m_pObjects[1]->SetMesh(pLevel1Mesh);
	m_pObjects[1]->GetTransform()->SetPosition(0.f, 10.f, 70.f);
	m_pObjects[1]->GetTransform()->SetRotationEuler(90.f, 0.f, 0.f);
	static_pointer_cast<ExplosiveObject>(m_pObjects[1])->SetAutoReset(TRUE);
	
	m_pObjects[2] = make_shared<ExplosiveObject>();
	m_pObjects[2]->SetColor(RandomGenerator::GenerateRandomColor());
	m_pObjects[2]->SetMesh(pLevel2Mesh);
	m_pObjects[2]->GetTransform()->SetPosition(0.f, 0.f, 70.f);
	m_pObjects[2]->GetTransform()->SetRotationEuler(90.f, 0.f, 0.f);
	static_pointer_cast<ExplosiveObject>(m_pObjects[2])->SetAutoReset(TRUE);
	
	m_pObjects[3] = make_shared<ExplosiveObject>();
	m_pObjects[3]->SetColor(RandomGenerator::GenerateRandomColor());
	m_pObjects[3]->SetMesh(pStartMesh);
	m_pObjects[3]->GetTransform()->SetPosition(-20.f, -15.f, 70.f);
	m_pObjects[3]->GetTransform()->SetRotationEuler(90.f, 0.f, 0.f);
	m_pObjects[3]->SetName("Start_Text");
	static_pointer_cast<ExplosiveObject>(m_pObjects[3])->SetAutoReset(FALSE);
	
	m_pObjects[4] = make_shared<ExplosiveObject>();
	m_pObjects[4]->SetColor(RandomGenerator::GenerateRandomColor());
	m_pObjects[4]->SetMesh(pEndMesh);
	m_pObjects[4]->GetTransform()->SetPosition(20.f, -15.f, 70.f);
	m_pObjects[4]->GetTransform()->SetRotationEuler(90.f, 0.f, 0.f);
	static_pointer_cast<ExplosiveObject>(m_pObjects[4])->SetAutoReset(TRUE);

	m_pPlayer = make_shared<FirstPersonPlayer>();
	m_pPlayer->Initialize();
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);

	ExplosiveObject::PrepareExplosion();
}

void MenuScene::ReleaseObjects()
{
	Scene::ReleaseObjects();
}

void MenuScene::Update(float fTimeElapsed)
{
	if (auto p = FindObjectInScene("Start_Text")) {
		if (static_pointer_cast<ExplosiveObject>(p)->IsExploded()) {
			GameFramework::ChangeScene(TAG_SCENE_LEVEL1);
			m_bSceneChanged = TRUE;
			return;
		}
	}

	ProcessMouseInput(fTimeElapsed);
	ProcessKeyboardInput(fTimeElapsed);

	if (m_bSceneChanged) {
		return;
	}

	if (m_pPlayer)
		m_pPlayer->Update(fTimeElapsed);

	if (!m_pObjects.empty()) {
		std::for_each(m_pObjects.begin(), m_pObjects.end(), [fTimeElapsed](std::shared_ptr<GameObject>& p) { p->Update(fTimeElapsed); });
	}
}

void MenuScene::Render(HDC hDCFrameBuffer)
{
	std::wstring wstrOutText{ L"MenuScene" };
	::TextOut(hDCFrameBuffer, 0, 0, wstrOutText.c_str(), wstrOutText.length());

	Scene::Render(hDCFrameBuffer);
}

void MenuScene::ProcessMouseInput(float fTimeElapsed)
{
	if (INPUT.GetButtonDown(VK_LBUTTON)) {
		POINT ptCursorPos = INPUT.GetCurrentCursorPos();
		std::shared_ptr<GameObject> pPickedObj = PickObjectPointedByCursor(ptCursorPos.x, ptCursorPos.y, m_pPlayer->GetCamera());
		if (pPickedObj) {
			pPickedObj->OnPicked();
			m_pPickedObject = pPickedObj;
		}
	}
}

void MenuScene::ProcessKeyboardInput(float fTimeElapsed)
{
}
