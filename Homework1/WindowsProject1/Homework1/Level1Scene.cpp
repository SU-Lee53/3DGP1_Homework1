#include "stdafx.h"
#include "Level1Scene.h"
#include "Mesh.h"
#include "FirstPersonPlayer.h"

using namespace std;

void Level1Scene::BuildObjects()
{
	shared_ptr<Mesh> pRollercoasterMesh = make_shared<Mesh>();
	MeshHelper::CreateRollercoasterRailMesh(pRollercoasterMesh);

	shared_ptr<Mesh> pCubeMesh = make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(pCubeMesh);

	m_pObjects.resize(1);
//	m_pObjects[0] = make_shared<GameObject>();
//	m_pObjects[0]->SetColor(RGB(255, 0, 0));
//	m_pObjects[0]->SetMesh(pRollercoasterMesh);
//	m_pObjects[0]->GetTransform()->SetPosition(0.f, 0.f, 0.f);

	m_pObjects[0] = make_shared<GameObject>();
	m_pObjects[0]->SetColor(RGB(255, 0, 0));
	m_pObjects[0]->SetMesh(pCubeMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 0.f, 0.0f);

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
	
	XMFLOAT3 xmf3CameraRotation = m_pPlayer->GetCamera()->GetTransform()->GetRotation();
	XMFLOAT3 xmf3CameraPosition = m_pPlayer->GetCamera()->GetTransform()->GetPosition();
	XMFLOAT3 xmf3CameraRight = m_pPlayer->GetCamera()->GetTransform()->GetRight();
	XMFLOAT3 xmf3CameraUp = m_pPlayer->GetCamera()->GetTransform()->GetUp();
	XMFLOAT3 xmf3CameraLook = m_pPlayer->GetCamera()->GetTransform()->GetLook();

	XMFLOAT4X4 xmf4x4View = m_pPlayer->GetCamera()->GetViewMatrix();
	XMFLOAT4X4 xmf4x4InverseView = m_pPlayer->GetCamera()->GetInverseViewMatrix();

	std::wstring wstrPlayerRotate = L"Player Rotation { Pitch, Yaw, Roll } : { " + to_wstring(xmf3PlayerRotation.x) +  L", " + to_wstring(xmf3PlayerRotation.y) + L" , " + to_wstring(xmf3PlayerRotation.z) + L" }";
	std::wstring wstrPlayerPosition = L"Player Position { X, Y, Z } : { " + to_wstring(xmf3PlayerPosition.x) +  L", " + to_wstring(xmf3PlayerPosition.y) + L" , " + to_wstring(xmf3PlayerPosition.z) + L" }";
	std::wstring wstrPlayerRight = L"Player Right { X, Y, Z } : { " + to_wstring(xmf3PlayerRight.x) +  L", " + to_wstring(xmf3PlayerRight.y) + L" , " + to_wstring(xmf3PlayerRight.z) + L" }";
	std::wstring wstrPlayerUp = L"Player Up { X, Y, Z } : { " + to_wstring(xmf3PlayerUp.x) +  L", " + to_wstring(xmf3PlayerUp.y) + L" , " + to_wstring(xmf3PlayerUp.z) + L" }";
	std::wstring wstrPlayerLook = L"Player Look { X, Y, Z } : { " + to_wstring(xmf3PlayerLook.x) +  L", " + to_wstring(xmf3PlayerLook.y) + L" , " + to_wstring(xmf3PlayerLook.z) + L" }";
	
	std::wstring wstrCameraRotate = L"Camera Rotation { Pitch, Yaw, Roll } : { " + to_wstring(xmf3CameraRotation.x) +  L", " + to_wstring(xmf3CameraRotation.y) + L" , " + to_wstring(xmf3CameraRotation.z) + L" }";
	std::wstring wstrCameraPosition = L"Camera Position { X, Y, Z } : { " + to_wstring(xmf3CameraPosition.x) +  L", " + to_wstring(xmf3CameraPosition.y) + L" , " + to_wstring(xmf3CameraPosition.z) + L" }";
	std::wstring wstrCameraRight = L"Camera Right { X, Y, Z } : { " + to_wstring(xmf3CameraRight.x) +  L", " + to_wstring(xmf3CameraRight.y) + L" , " + to_wstring(xmf3CameraRight.z) + L" }";
	std::wstring wstrCameraUp = L"Camera Up { X, Y, Z } : { " + to_wstring(xmf3CameraUp.x) +  L", " + to_wstring(xmf3CameraUp.y) + L" , " + to_wstring(xmf3CameraUp.z) + L" }";
	std::wstring wstrCameraLook = L"Camera Look { X, Y, Z } : { " + to_wstring(xmf3CameraLook.x) +  L", " + to_wstring(xmf3CameraLook.y) + L" , " + to_wstring(xmf3CameraLook.z) + L" }";

	std::wstring wstrCameraView1 = L"Camera View r1 { X, Y, Z, W } : { " + to_wstring(xmf4x4View._11) +  L", " + to_wstring(xmf4x4View._12) + L" , " + to_wstring(xmf4x4View._13) + to_wstring(xmf4x4View._14) + L" }";
	std::wstring wstrCameraView2 = L"Camera View r2 { X, Y, Z, W } : { " + to_wstring(xmf4x4View._21) +  L", " + to_wstring(xmf4x4View._22) + L" , " + to_wstring(xmf4x4View._23) + to_wstring(xmf4x4View._24) + L" }";
	std::wstring wstrCameraView3 = L"Camera View r3 { X, Y, Z, W } : { " + to_wstring(xmf4x4View._31) +  L", " + to_wstring(xmf4x4View._32) + L" , " + to_wstring(xmf4x4View._33) + to_wstring(xmf4x4View._34) + L" }";
	std::wstring wstrCameraView4 = L"Camera View r4 { X, Y, Z, W } : { " + to_wstring(xmf4x4View._41) +  L", " + to_wstring(xmf4x4View._42) + L" , " + to_wstring(xmf4x4View._43) + to_wstring(xmf4x4View._44) + L" }";
	
	std::wstring wstrCameraInverseView1 = L"Camera Inv-View r1 { X, Y, Z, W } : { " + to_wstring(xmf4x4InverseView._11) +  L", " + to_wstring(xmf4x4InverseView._12) + L" , " + to_wstring(xmf4x4InverseView._13) + to_wstring(xmf4x4InverseView._14) + L" }";
	std::wstring wstrCameraInverseView2 = L"Camera Inv-View r2 { X, Y, Z, W } : { " + to_wstring(xmf4x4InverseView._21) +  L", " + to_wstring(xmf4x4InverseView._22) + L" , " + to_wstring(xmf4x4InverseView._23) + to_wstring(xmf4x4InverseView._24) + L" }";
	std::wstring wstrCameraInverseView3 = L"Camera Inv-View r3 { X, Y, Z, W } : { " + to_wstring(xmf4x4InverseView._31) +  L", " + to_wstring(xmf4x4InverseView._32) + L" , " + to_wstring(xmf4x4InverseView._33) + to_wstring(xmf4x4InverseView._34) + L" }";
	std::wstring wstrCameraInverseView4 = L"Camera Inv-View r4 { X, Y, Z, W } : { " + to_wstring(xmf4x4InverseView._41) +  L", " + to_wstring(xmf4x4InverseView._42) + L" , " + to_wstring(xmf4x4InverseView._43) + to_wstring(xmf4x4InverseView._44) + L" }";

	::TextOut(hDCFrameBuffer, 20, 50, wstrPlayerRotate.c_str(), wstrPlayerRotate.length());
	::TextOut(hDCFrameBuffer, 20, 70, wstrPlayerPosition.c_str(), wstrPlayerPosition.length());
	::TextOut(hDCFrameBuffer, 20, 90, wstrPlayerRight.c_str(), wstrPlayerRight.length());
	::TextOut(hDCFrameBuffer, 20, 110, wstrPlayerUp.c_str(), wstrPlayerUp.length());
	::TextOut(hDCFrameBuffer, 20, 130, wstrPlayerLook.c_str(), wstrPlayerLook.length());
							  
	::TextOut(hDCFrameBuffer, 20, 150, wstrCameraRotate.c_str(), wstrCameraRotate.length());
	::TextOut(hDCFrameBuffer, 20, 170, wstrCameraPosition.c_str(), wstrCameraPosition.length());
	::TextOut(hDCFrameBuffer, 20, 190, wstrCameraRight.c_str(), wstrCameraRight.length());
	::TextOut(hDCFrameBuffer, 20, 210, wstrCameraUp.c_str(), wstrCameraUp.length());
	::TextOut(hDCFrameBuffer, 20, 230, wstrCameraLook.c_str(), wstrCameraLook.length());

	::TextOut(hDCFrameBuffer, 20, 300, wstrCameraView1.c_str(), wstrCameraView1.length());
	::TextOut(hDCFrameBuffer, 20, 320, wstrCameraView2.c_str(), wstrCameraView2.length());
	::TextOut(hDCFrameBuffer, 20, 340, wstrCameraView3.c_str(), wstrCameraView3.length());
	::TextOut(hDCFrameBuffer, 20, 360, wstrCameraView4.c_str(), wstrCameraView4.length());
	
	::TextOut(hDCFrameBuffer, 20, 400, wstrCameraInverseView1.c_str(), wstrCameraInverseView1.length());
	::TextOut(hDCFrameBuffer, 20, 420, wstrCameraInverseView2.c_str(), wstrCameraInverseView2.length());
	::TextOut(hDCFrameBuffer, 20, 440, wstrCameraInverseView3.c_str(), wstrCameraInverseView3.length());
	::TextOut(hDCFrameBuffer, 20, 460, wstrCameraInverseView4.c_str(), wstrCameraInverseView4.length());



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
