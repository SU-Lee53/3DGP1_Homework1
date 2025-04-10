#include "stdafx.h"
#include "Level1Scene.h"
#include "Mesh.h"
#include "FirstPersonPlayer.h"

using namespace std;

void Level1Scene::BuildObjects()
{
	shared_ptr<Mesh> pRollercoasterMesh = make_shared<Mesh>();
	MeshHelper::CreateRollercoasterRailMesh(pRollercoasterMesh);

	m_pPlayer = make_shared<FirstPersonPlayer>();

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
void Level1Scene::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	std::wstring wstrOutText{ L"Level1Scene" };
	::TextOut(hDCFrameBuffer, 100, 100, wstrOutText.c_str(), wstrOutText.length());

	XMFLOAT3 xmf3PlayerRotation = m_pPlayer->GetTransform()->GetRotation();
	XMFLOAT3 xmf3PlayerPosition = m_pPlayer->GetTransform()->GetPosition();
	std::wstring wstrPlayerRotate = L"{ Pitch, Yaw, Roll } : { " + to_wstring(xmf3PlayerRotation.x) +  L", " + to_wstring(xmf3PlayerRotation.y) + L" , " + to_wstring(xmf3PlayerRotation.z) + L" }";
	std::wstring wstrPlayerPosition = L"{ X, Y, Z } : { " + to_wstring(xmf3PlayerPosition.x) +  L", " + to_wstring(xmf3PlayerPosition.y) + L" , " + to_wstring(xmf3PlayerPosition.z) + L" }";
	::TextOut(hDCFrameBuffer, 100, 200, wstrPlayerRotate.c_str(), wstrPlayerRotate.length());
	::TextOut(hDCFrameBuffer, 100, 300, wstrPlayerPosition.c_str(), wstrPlayerPosition.length());
}

void Level1Scene::ProcessMouseInput()
{
	m_pPlayer->ProcessMouseInput();
}

void Level1Scene::ProcessKeyboardInput()
{
	m_pPlayer->ProcessKeyboardInput();
}
