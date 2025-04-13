#include "stdafx.h"
#include "Level1Scene.h"
#include "Mesh.h"
#include "FirstPersonPlayer.h"

using namespace std;

void Level1Scene::BuildObjects()
{
	std::vector<XMFLOAT3> RollercoasterRoutes = {};
	shared_ptr<Mesh> pRollercoasterMesh = make_shared<Mesh>();
	MeshHelper::CreateRollercoasterRailMesh(pRollercoasterMesh, m_xmf3MoveRoutes, 10.0f, 150.0f, 30, 5);

	shared_ptr<Mesh> pCubeMesh = make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(pCubeMesh);
	
	shared_ptr<Mesh> pCarMesh = make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(pCarMesh, 12.0f, 5.0f, 15.0f);

	m_pObjects.resize(2);
	m_pObjects[0] = make_shared<GameObject>();
	m_pObjects[0]->SetColor(RGB(0,0,0));
	m_pObjects[0]->SetMesh(pRollercoasterMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 0.f, 0.f);

	m_pObjects[1] = make_shared<GameObject>();
	m_pObjects[1]->SetColor(RGB(255, 0, 0));
	m_pObjects[1]->SetMesh(pCubeMesh);
	m_pObjects[1]->GetTransform()->SetPosition(0.f, 0.f, 0.0f);
	
	m_pPlayer = make_shared<FirstPersonPlayer>();
	m_pPlayer->Initialize();
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);
}

void Level1Scene::ReleaseObjects()
{
}

void Level1Scene::Update(float fTimeElapsed)
{
	ProcessMouseInput();
	ProcessKeyboardInput();

	if (m_bPlayerRide) UpdatePlayerRide(fTimeElapsed);
	if (m_bRollercoasterEnd) GameFramework::ChangeScene(TAG_SCENE_LEVEL2);

	if(m_pPlayer)
		m_pPlayer->Update(fTimeElapsed);
	
	if (!m_pObjects.empty()) {
		std::for_each(m_pObjects.begin(), m_pObjects.end(), [fTimeElapsed](std::shared_ptr<GameObject>& p) {p->Update(fTimeElapsed); });
	}

}

void Level1Scene::Render(HDC hDCFrameBuffer)
{
	XMFLOAT3 xmf3PlayerRotation = m_pPlayer->GetTransform()->GetRotation();
	std::wstring wstrPlayerRotate = L"Player Rotation { Pitch, Yaw, Roll } : { " + to_wstring(xmf3PlayerRotation.x) + L", " + to_wstring(xmf3PlayerRotation.y) + L" , " + to_wstring(xmf3PlayerRotation.z) + L" }";

	std::wstring wstrOutText{ L"Level1Scene" };
	::TextOut(hDCFrameBuffer, 0, 0, wstrOutText.c_str(), wstrOutText.length());
	::TextOut(hDCFrameBuffer, 50, 50, wstrPlayerRotate.c_str(), wstrPlayerRotate.length());

	Scene::Render(hDCFrameBuffer);
}

void Level1Scene::ProcessMouseInput()
{
	m_pPlayer->ProcessMouseInput();
}

void Level1Scene::ProcessKeyboardInput()
{
	if (INPUT.GetButtonDown('R')) {
		m_bPlayerRide = !m_bPlayerRide;
		if (!m_bPlayerRide) ResetPlayerRide();
	}

	if (!m_bPlayerRide) {
		m_pPlayer->ProcessKeyboardInput();
	}
}

void Level1Scene::UpdatePlayerRide(float fTimeElapsed)
{
	if (!m_bPlayerRide) return;

	if (m_nCurrentRoute < m_xmf3MoveRoutes.size() - 2) {
		if (m_fInterpolationFactor <= 1.0f) {
			XMVECTOR xmvPos1 = XMLoadFloat3(&m_xmf3MoveRoutes[m_nCurrentRoute]);
			XMVECTOR xmvPos2 = XMLoadFloat3(&m_xmf3MoveRoutes[m_nCurrentRoute + 1]);

			XMVECTOR xmvCurrentPos = XMVectorLerp(xmvPos1, xmvPos2, m_fInterpolationFactor);
			m_fInterpolationFactor += 5.0f * fTimeElapsed;

			XMFLOAT3 xmf3CurrentPos;
			XMStoreFloat3(&xmf3CurrentPos, xmvCurrentPos);

			m_pPlayer->GetTransform()->SetPosition(Vector3::Add(xmf3CurrentPos, XMFLOAT3{ 0.f, 15.f, 0.f }));
		}
		else {
			m_fInterpolationFactor = 0.f;
			m_nCurrentRoute++;
		}
	}
	else {
		m_bRollercoasterEnd = TRUE;
	}
}

void Level1Scene::ResetPlayerRide()
{
	m_nCurrentRoute = 1;
	m_fInterpolationFactor = 0.f;
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);
	m_pPlayer->GetTransform()->SetRotation(0.f, 0.f, 0.f);
}
