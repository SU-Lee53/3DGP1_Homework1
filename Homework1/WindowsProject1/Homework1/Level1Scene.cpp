#include "stdafx.h"
#include "Level1Scene.h"
#include "Mesh.h"
#include "FirstPersonPlayer.h"

using namespace std;

void Level1Scene::BuildObjects()
{
	shared_ptr<Mesh> pRollercoasterMesh = make_shared<Mesh>();
	MeshHelper::CreateRollercoasterRailMesh(pRollercoasterMesh, 20.0f, 30.0f, 10, 30);

	shared_ptr<Mesh> pCubeMesh = make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(pCubeMesh);

	m_pObjects.resize(2);
	m_pObjects[0] = make_shared<GameObject>();
	m_pObjects[0]->SetColor(RGB(255, 0, 0));
	m_pObjects[0]->SetMesh(pRollercoasterMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 0.f, 0.f);

	m_pObjects[1] = make_shared<GameObject>();
	m_pObjects[1]->SetColor(RGB(255, 0, 0));
	m_pObjects[1]->SetMesh(pCubeMesh);
	m_pObjects[1]->GetTransform()->SetPosition(0.f, 0.f, 0.0f);

	m_pPlayer = make_shared<FirstPersonPlayer>();
	m_pPlayer->Initialize();
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, -15.f);
}

void Level1Scene::ReleaseObjects()
{
}

void Level1Scene::Update(float fTimeElapsed)
{
	ProcessMouseInput();
	ProcessKeyboardInput();

	if(m_pPlayer)
		m_pPlayer->Update(fTimeElapsed);
	
	if (!m_pObjects.empty()) {
		std::for_each(m_pObjects.begin(), m_pObjects.end(), [fTimeElapsed](std::shared_ptr<GameObject>& p) {p->Update(fTimeElapsed); });
	}

}

bool Pressed = false;
POINT CursorDelta = {};
XMFLOAT3 PlayerRotate = {};
XMFLOAT3 PlayerPosition = {};

void Level1Scene::Render(HDC hDCFrameBuffer)
{
	std::wstring wstrOutText{ L"Level1Scene" };
	::TextOut(hDCFrameBuffer, 0, 0, wstrOutText.c_str(), wstrOutText.length());

	XMFLOAT3 xmf3PlayerRotation = m_pPlayer->GetTransform()->GetRotation();
	XMFLOAT3 xmf3PlayerPosition = m_pPlayer->GetTransform()->GetPosition();
	XMFLOAT3 xmf3PlayerRight = m_pPlayer->GetTransform()->GetRight();
	XMFLOAT3 xmf3PlayerUp = m_pPlayer->GetTransform()->GetUp();
	XMFLOAT3 xmf3PlayerLook = m_pPlayer->GetTransform()->GetLook();
	
	std::wstring wstrPlayerRotate = L"Player Rotation { Pitch, Yaw, Roll } : { " + to_wstring(xmf3PlayerRotation.x) +  L", " + to_wstring(xmf3PlayerRotation.y) + L" , " + to_wstring(xmf3PlayerRotation.z) + L" }";
	std::wstring wstrPlayerPosition = L"Player Position { X, Y, Z } : { " + to_wstring(xmf3PlayerPosition.x) +  L", " + to_wstring(xmf3PlayerPosition.y) + L" , " + to_wstring(xmf3PlayerPosition.z) + L" }";
	std::wstring wstrPlayerRight = L"Player Right { X, Y, Z } : { " + to_wstring(xmf3PlayerRight.x) +  L", " + to_wstring(xmf3PlayerRight.y) + L" , " + to_wstring(xmf3PlayerRight.z) + L" }";
	std::wstring wstrPlayerUp = L"Player Up { X, Y, Z } : { " + to_wstring(xmf3PlayerUp.x) +  L", " + to_wstring(xmf3PlayerUp.y) + L" , " + to_wstring(xmf3PlayerUp.z) + L" }";
	std::wstring wstrPlayerLook = L"Player Look { X, Y, Z } : { " + to_wstring(xmf3PlayerLook.x) +  L", " + to_wstring(xmf3PlayerLook.y) + L" , " + to_wstring(xmf3PlayerLook.z) + L" }";
	
	::TextOut(hDCFrameBuffer, 20, 50, wstrPlayerRotate.c_str(), wstrPlayerRotate.length());
	::TextOut(hDCFrameBuffer, 20, 70, wstrPlayerPosition.c_str(), wstrPlayerPosition.length());
	::TextOut(hDCFrameBuffer, 20, 90, wstrPlayerRight.c_str(), wstrPlayerRight.length());
	::TextOut(hDCFrameBuffer, 20, 110, wstrPlayerUp.c_str(), wstrPlayerUp.length());
	::TextOut(hDCFrameBuffer, 20, 130, wstrPlayerLook.c_str(), wstrPlayerLook.length());
				


	Scene::Render(hDCFrameBuffer);
}

void Level1Scene::ProcessMouseInput()
{
	m_pPlayer->ProcessMouseInput();
}

void Level1Scene::ProcessKeyboardInput()
{
	m_pPlayer->ProcessKeyboardInput();
}
