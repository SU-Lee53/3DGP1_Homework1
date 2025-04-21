#include "stdafx.h"
#include "TankPlayer.h"
#include "ThirdPersonCamera.h"
#include "BulletObject.h"

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
	SetCameraOffset(XMFLOAT3{ 0.f, 3.0f, -10.0f });


	m_pMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(m_pMesh, L"../Tank.obj");
	SetColor(RGB(255, 0, 0));
	SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });

	shared_ptr<Mesh> pBulletMesh = make_shared<Mesh>();
	MeshHelper::CreateCubeMesh(pBulletMesh);
	std::generate_n(m_pBullets.begin(), BULLET_COUNT, [this, &pBulletMesh]()->std::shared_ptr<BulletObject> {
		shared_ptr<BulletObject> pBullet = make_shared<BulletObject>(m_fBulletEffectiveRange);
		pBullet->SetMesh(pBulletMesh);
		pBullet->SetRotationAxis(XMFLOAT3{ 0.f, 1.f, 0.f });
		pBullet->SetRotationSpeed(360.f);
		pBullet->SetMovingSpeed(120.0f);
		pBullet->SetActive(FALSE);
		return pBullet;
	});
}

void TankPlayer::Update(float fTimeElapsed)
{
	Player::Update(fTimeElapsed);
}

void TankPlayer::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	Player::Render(hDCFrameBuffer, pCamera);
}

void TankPlayer::ProcessKeyboardInput(float fTimeElapsed)
{
	if (INPUT.GetButtonPressed('W')) {
		Player::Move(m_pTransform->GetLook(), m_fSpeed * fTimeElapsed);
	}

	if (INPUT.GetButtonPressed('S')) {
		Player::Move(m_pTransform->GetLook(), -m_fSpeed * fTimeElapsed);
	}

	if (INPUT.GetButtonPressed('D')) {
		Player::Move(m_pTransform->GetRight(), m_fSpeed * fTimeElapsed);
	}

	if (INPUT.GetButtonPressed('A')) {
		Player::Move(m_pTransform->GetRight(), -m_fSpeed * fTimeElapsed);
	}
}

void TankPlayer::ProcessMouseInput(float fTimeElapsed)
{
	if (INPUT.GetButtonPressed(VK_RBUTTON)) {
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
	m_pTransform->AddRotationEuler(0.f, fYaw, 0.f);
	m_pCamera->Rotate(fPitch, fYaw, fRoll);
}

void TankPlayer::UpdatePlayerCamera(float fTimeElapsed)
{
	m_pCamera->Update(fTimeElapsed);
}
