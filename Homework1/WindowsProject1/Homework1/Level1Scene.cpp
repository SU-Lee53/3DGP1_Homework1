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

void Level1Scene::Animate(float fTimeElapsed)
{
	ProcessMouseInput();
	ProcessKeyboardInput();

	if(m_pPlayer)
		m_pPlayer->Animate(fTimeElapsed);
	
	if (!m_pObjects.empty()) {
		std::for_each(m_pObjects.begin(), m_pObjects.end(), [fTimeElapsed](std::shared_ptr<GameObject>& p) {p->Animate(fTimeElapsed); });
	}

}

bool Pressed = false;
POINT CursorDelta = {};
XMFLOAT3 PlayerRotate = {};
void Level1Scene::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	std::wstring wstrOutText{ L"Level1Scene" };
	::TextOut(hDCFrameBuffer, 100, 100, wstrOutText.c_str(), wstrOutText.length());

	if (Pressed) {
		std::wstring wstrOutText2{ L"Pressed" };
		std::wstring wstrDelta = L"{ " + to_wstring(CursorDelta.x) + L", " + to_wstring(CursorDelta.y) + L" }";
		::TextOut(hDCFrameBuffer, 100, 300, wstrOutText2.c_str(), wstrOutText2.length());
		::TextOut(hDCFrameBuffer, 100, 400, wstrDelta.c_str(), wstrDelta.length());
	}

	XMFLOAT3 xmf3PlayerRotation = m_pPlayer->GetRotation();
	std::wstring wstrPlayerRotate = L"{ Pitch, Yaw, Roll } : { " + to_wstring(xmf3PlayerRotation.x) +  L", " + to_wstring(xmf3PlayerRotation.y) + L" , " + to_wstring(xmf3PlayerRotation.z) + L" }";
	::TextOut(hDCFrameBuffer, 100, 200, wstrPlayerRotate.c_str(), wstrPlayerRotate.length());
}

void Level1Scene::ProcessMouseInput()
{
	if (INPUT.GetButtonPressed(VK_LBUTTON)) {
		SetCursor(NULL);
		POINT ptCursorDelta = INPUT.GetCursorDeltaPos();
		float cxMouseDelta = (float)(ptCursorDelta.x) / 3.0f;
		float cyMouseDelta = (float)(ptCursorDelta.y) / 3.0f;

		if (cxMouseDelta || cyMouseDelta) {
			m_pPlayer->Rotate(-cyMouseDelta, cxMouseDelta, 0.0f);
		}

		// DEBUG ZONE
		CursorDelta = ptCursorDelta;
		Pressed = true;
	}
	else {
		Pressed = false;
	}

}

void Level1Scene::ProcessKeyboardInput()
{
}
