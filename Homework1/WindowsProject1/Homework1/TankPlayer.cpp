#include "stdafx.h"
#include "TankPlayer.h"
#include "ThirdPersonCamera.h"

using namespace std;

TankPlayer::TankPlayer()
{
}

TankPlayer::~TankPlayer()
{
}

void TankPlayer::Initialize()
{
	m_pCamera = make_shared<ThirdPersonCamera>();
	m_pCamera->Initialize(shared_from_this());
	m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	m_pCamera->SetFOVAngle(60.0f);
	m_pCamera->SetNearZ(1.01f);
	m_pCamera->SetFarZ(500.0f);

	MeshHelper::CreateMeshFromOBJFiles(m_pMesh, L"../Tank.obj");

}

void TankPlayer::Update(float fTimeElapsed)
{
	Player::Update(fTimeElapsed);
}

void TankPlayer::ProcessKeyboardInput()
{
	if (INPUT.GetButtonPressed('W')) {
		Player::Move(m_pTransform->GetLook(), 0.5f);
	}

	if (INPUT.GetButtonPressed('S')) {
		Player::Move(m_pTransform->GetLook(), -0.5f);
	}

	if (INPUT.GetButtonPressed('D')) {
		Player::Move(m_pTransform->GetRight(), 0.5f);
	}

	if (INPUT.GetButtonPressed('A')) {
		Player::Move(m_pTransform->GetRight(), -0.5f);
	}

	if (INPUT.GetButtonPressed('E')) {
		Player::Move(m_pTransform->GetUp(), 0.5f);
	}

	if (INPUT.GetButtonPressed('Q')) {
		Player::Move(m_pTransform->GetUp(), -0.5f);
	}
}

void TankPlayer::ProcessMouseInput()
{
	if (INPUT.GetButtonPressed(VK_LBUTTON)) {
		HWND hWnd = ::GetActiveWindow();

		::SetCursor(NULL);

		RECT rtClientRect;
		::GetClientRect(hWnd, &rtClientRect);
		::ClientToScreen(hWnd, (LPPOINT)&rtClientRect.left);
		::ClientToScreen(hWnd, (LPPOINT)&rtClientRect.right);

		int nScreenCenterX = 0, nScreenCenterY = 0;
		nScreenCenterX = rtClientRect.left + FRAME_BUFFER_WIDTH / 2;
		nScreenCenterY = rtClientRect.top + FRAME_BUFFER_HEIGHT / 2;

		POINT ptCursorPos;
		::GetCursorPos(&ptCursorPos);

		POINT ptDelta{ (ptCursorPos.x - nScreenCenterX), (ptCursorPos.y - nScreenCenterY) };

		Rotate(ptDelta.y * 0.10f, ptDelta.x * 0.10f, 0.f);

		::SetCursorPos(nScreenCenterX, nScreenCenterY);
	}
}

void TankPlayer::Rotate(float fPitch, float fYaw, float fRoll)
{
}

void TankPlayer::UpdatePlayerCamera(float fTimeElapsed)
{
}
