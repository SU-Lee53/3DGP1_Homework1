#include "stdafx.h"
#include "FirstPersonPlayer.h"
#include "Camera.h"

using namespace std;

FirstPersonPlayer::FirstPersonPlayer()
{
	m_pCamera = make_shared<Camera>();
	m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	m_pCamera->GeneratePerspectiveProjectionMatrix(1.01f, 500.0f, 60.0f);
	m_pCamera->SetFOVAngle(60.0f);
	m_pCamera->GenerateOrthographicProjectionMatrix(1.01f, 50.0f, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);


	m_pTransform = make_shared<Transform>();

}

FirstPersonPlayer::~FirstPersonPlayer()
{
}

void FirstPersonPlayer::Update(float fTimeElapsed)
{
	Player::Update(fTimeElapsed);
}

void FirstPersonPlayer::ProcessKeyboardInput()
{
	if (INPUT.GetButtonPressed(VK_UP)) {
		Player::Move(m_pTransform->GetLook(), 0.15f);
	}

	if (INPUT.GetButtonPressed(VK_DOWN)) {
		Player::Move(m_pTransform->GetLook(), -0.15f);
	}

	if (INPUT.GetButtonPressed(VK_RIGHT)) {
		Player::Move(m_pTransform->GetRight(), 0.15f);
	}

	if (INPUT.GetButtonPressed(VK_LEFT)) {
		Player::Move(m_pTransform->GetRight(), -0.15f);
	}
}

void FirstPersonPlayer::ProcessMouseInput()
{
	if (INPUT.GetButtonPressed(VK_LBUTTON)) {
		SetCursor(NULL);
		POINT ptCursorDelta = INPUT.GetCursorDeltaPos();
		float cxMouseDelta = (float)(ptCursorDelta.x) / 3.0f;
		float cyMouseDelta = (float)(ptCursorDelta.y) / 3.0f;

		if (cxMouseDelta || cyMouseDelta) {
			Rotate(-cyMouseDelta, cxMouseDelta, 0.0f);
		}
	}
	UpdatePlayerCamera(0.0f);
}

void FirstPersonPlayer::UpdatePlayerCamera(float fTimeElapsed)
{
	// Sync with player
	m_pCamera->SetPosition(m_pTransform->GetPosition());
	m_pCamera->SetLookAt(m_pTransform->GetLook(), m_pTransform->GetUp());
}
