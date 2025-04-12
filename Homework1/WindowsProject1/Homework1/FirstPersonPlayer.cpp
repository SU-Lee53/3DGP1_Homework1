#include "stdafx.h"
#include "FirstPersonPlayer.h"
#include "Camera.h"

using namespace std;

FirstPersonPlayer::FirstPersonPlayer()
{
}

FirstPersonPlayer::~FirstPersonPlayer()
{
}

void FirstPersonPlayer::Initialize()
{
	m_pCamera = make_shared<Camera>();
	m_pCamera->Initialize(shared_from_this());
	m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	m_pCamera->SetFOVAngle(60.0f);
	m_pCamera->SetNearZ(1.01f);
	m_pCamera->SetFarZ(500.0f);
}

void FirstPersonPlayer::Update(float fTimeElapsed)
{
	Player::Update(fTimeElapsed);
}

void FirstPersonPlayer::ProcessKeyboardInput()
{
	if (INPUT.GetButtonPressed('W')) {
		Player::Move(m_pTransform->GetLook(), 0.15f);
	}

	if (INPUT.GetButtonPressed('S')) {
		Player::Move(m_pTransform->GetLook(), -0.15f);
	}

	if (INPUT.GetButtonPressed('D')) {
		Player::Move(m_pTransform->GetRight(), 0.15f);
	}

	if (INPUT.GetButtonPressed('A')) {
		Player::Move(m_pTransform->GetRight(), -0.15f);
	}
	
	if (INPUT.GetButtonPressed('E')) {
		Player::Move(m_pTransform->GetUp(), 0.15f);
	}
	
	if (INPUT.GetButtonPressed('Q')) {
		Player::Move(m_pTransform->GetUp(), -0.15f);
	}

}

void FirstPersonPlayer::ProcessMouseInput()
{
	if (INPUT.GetButtonPressed(VK_LBUTTON)) {
		HWND hWnd = ::GetActiveWindow();

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

		XMVECTOR xmvRotation = XMLoadFloat3(&m_pTransform->GetRotation());
		xmvRotation = XMVectorAdd(xmvRotation, XMVectorSet(ptDelta.y * 0.10f, ptDelta.x * 0.10f, 0.f, 0.f));
		XMFLOAT3 xmf3Rotation;
		XMStoreFloat3(&xmf3Rotation, xmvRotation);
		Player::Rotate(xmf3Rotation.x ,xmf3Rotation.y, xmf3Rotation.z);

		::SetCursorPos(nScreenCenterX, nScreenCenterY);
	}
}

void FirstPersonPlayer::UpdatePlayerCamera(float fTimeElapsed)
{
	// Sync with player
	m_pCamera->SetPosition(m_pTransform->GetPosition());
	//m_pCamera->SetLookAt(m_pTransform->GetPosition(), m_pTransform->GetLook(), m_pTransform->GetUp());
	m_pCamera->SetRotation(m_pTransform->GetRotation());
	m_pCamera->Update();
}
