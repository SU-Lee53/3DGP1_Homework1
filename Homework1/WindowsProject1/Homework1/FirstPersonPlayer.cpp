#include "stdafx.h"
#include "FirstPersonPlayer.h"
#include "Camera.h"

using namespace std;

FirstPersonPlayer::FirstPersonPlayer()
{
	shared_ptr<Camera> pCamera = make_shared<Camera>();
	pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	pCamera->GeneratePerspectiveProjectionMatrix(1.01f, 500.0f, 60.0f);
	pCamera->SetFOVAngle(60.0f);
	pCamera->GenerateOrthographicProjectionMatrix(1.01f, 50.0f, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);

	m_pCamera.swap(pCamera);
}

FirstPersonPlayer::~FirstPersonPlayer()
{
}

void FirstPersonPlayer::Update(float fTimeElapsed)
{
}

void FirstPersonPlayer::ProcessKeyboardInput()
{
}

void FirstPersonPlayer::ProcessMouseInput()
{



	UpdatePlayerCamera(0.0f);
}

void FirstPersonPlayer::UpdatePlayerCamera(float fTimeElapsed)
{
	// Sync with player
	m_pCamera->SetPosition(m_xmf3Position);
	m_pCamera->SetLookAt(m_xmf3Look, m_xmf3Up);
}
