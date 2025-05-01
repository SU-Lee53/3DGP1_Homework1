#include "stdafx.h"
#include "TankObject.h"
#include "TankPlayer.h"
#include "BulletObject.h"
#include "WallsObject.h"
#include "ObstacleObject.h"

using namespace std;

TankObject::TankObject()
{
}

TankObject::~TankObject()
{
}

void TankObject::SetMovingDirection(const XMFLOAT3& xmf3MovingDirection)
{
	m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection);

	// Rotate Tank
	XMFLOAT3 xmf3Look = m_pTransform->GetLook();

	XMVECTOR xmvCurrentLook = XMLoadFloat3(&xmf3Look);
	XMVECTOR xmvNewLook = XMLoadFloat3(&m_xmf3MovingDirection);

	float fDot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(xmvCurrentLook), XMVector3Normalize(xmvNewLook)));

	// 스칼라 삼중적 이용 -> 회전방향 구함
	XMVECTOR xmvCross = XMVector3Cross(xmvCurrentLook, xmvNewLook);
	float fShortestRotate = XMVectorGetX(XMVector3Dot(XMVectorSet(0.f, 1.f, 0.f, 0.f), xmvCross));

	float fAngleRadian = acosf(fDot);
	float fAngleDegree = XMConvertToDegrees(fAngleRadian);
	
	if (fShortestRotate < 0) {
		fAngleDegree *= -1;
	}

	m_pTransform->AddRotationEuler(0.f, fAngleDegree, 0.f);
}

void TankObject::InitializeMovingDirection()
{
	XMFLOAT3 xmf3RotationEuler = m_pTransform->GetRotationEuler();
	float fPitch = XMConvertToRadians(xmf3RotationEuler.x);
	float fYaw = XMConvertToRadians(xmf3RotationEuler.y);
	float fRoll = XMConvertToRadians(xmf3RotationEuler.z);

	XMFLOAT4X4 xmf4x4Rotate;
	XMStoreFloat4x4(&xmf4x4Rotate, XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll));
	m_xmf3MovingDirection = Vector3::Normalize(XMFLOAT3{ xmf4x4Rotate._31, xmf4x4Rotate._32, xmf4x4Rotate._33 });
}

void TankObject::Initialize()
{
	shared_ptr<Mesh> pTankMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pTankMesh, L"../Tank.obj");
	SetMesh(pTankMesh);
	SetColor(RGB(255, 0, 0));
	SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });

	XMFLOAT3 xmf3InitialPosition{};
	xmf3InitialPosition.x = RandomGenerator::GenerateRandomFloatInRange(-45.f, 45.f);
	xmf3InitialPosition.y = 0;
	xmf3InitialPosition.z = RandomGenerator::GenerateRandomFloatInRange(-45.f, 45.f);

	XMFLOAT3 xmf3InitialRotation{};
	xmf3InitialRotation.y = RandomGenerator::GenerateRandomFloatInRange(0.f, 360.f);

	m_pTransform->SetPosition(xmf3InitialPosition);
	m_pTransform->SetRotationEuler(xmf3InitialRotation);

	m_fMovingSpeed = 15.0f;

	SetAutoReset(FALSE);

	InitializeMovingDirection();
}

void TankObject::Update(float fElapsedTime)
{
	if (m_fMovingSpeed != 0.0f && !m_bBlowingUp) {
		XMFLOAT3 xmf3Shift = Vector3::Add(XMFLOAT3{ 0,0,0 }, m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);
		m_pTransform->AddPosition(xmf3Shift);
	}

	UpdateBoundingBox();

	ExplosiveObject::Update(fElapsedTime);
}

void TankObject::Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera)
{
	ExplosiveObject::Render(hDCFrameBuffer, pCamera);
}

void TankObject::OnCollision(std::shared_ptr<GameObject> pOther)
{
	if (auto p = dynamic_pointer_cast<TankPlayer>(pOther)) {
		if (p->IsShieldOn()) {
			OnPicked();
		}
		else {
			XMVECTOR xmvNormal = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&m_pTransform->GetPosition()), XMLoadFloat3(&p->GetTransform()->GetPosition())));
			XMVECTOR xmvReflected = XMVector3Reflect(XMLoadFloat3(&m_xmf3MovingDirection), xmvNormal);

			XMFLOAT3 xmf3Reflect;
			XMStoreFloat3(&xmf3Reflect, xmvReflected);
			SetMovingDirection(xmf3Reflect);
		}
	}
	else if (auto p = dynamic_pointer_cast<TankObject>(pOther)) {

		if (!m_bCollisionUpdated) {
			XMFLOAT3 xmf3MovingDirection = p->GetMovingDirection();
			float fMovingSpeed = p->GetMovingSpeed();

			// TODO : 여기 로직 수정필요 (변경된 Direction 과 speed 가 Other 에게 반영되어 같은 방향으로 움직이게 됨)
			p->SetMovingDirection(m_xmf3MovingDirection);
			p->SetMovingSpeed(m_fMovingSpeed);
			p->SetCollisionUpdated(TRUE);

			SetMovingDirection(xmf3MovingDirection);
			SetMovingSpeed(fMovingSpeed);
		}
		else {
			m_bCollisionUpdated = FALSE;
		}
	}
	else if (auto p = dynamic_pointer_cast<BulletObject>(pOther)) {
		OnPicked();
	}
	else if (auto p = dynamic_pointer_cast<WallsObject>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<ObstacleObject>(pOther)) {
		XMVECTOR xmvNormal = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&m_pTransform->GetPosition()), XMLoadFloat3(&p->GetTransform()->GetPosition())));
		XMVECTOR xmvReflected = XMVector3Reflect(XMLoadFloat3(&m_xmf3MovingDirection), xmvNormal);

		XMFLOAT3 xmf3Reflect;
		XMStoreFloat3(&xmf3Reflect, xmvReflected);
		SetMovingDirection(xmf3Reflect);
	}
}
