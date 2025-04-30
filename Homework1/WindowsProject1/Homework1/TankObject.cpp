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

void TankObject::Initialize()
{
	shared_ptr<Mesh> pTankMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pTankMesh, L"../Tank.obj");
	SetMesh(pTankMesh);
	SetColor(RGB(255, 0, 0));
	SetMeshDefaultOrientation(XMFLOAT3{ -90.f, 180.f, 0.f });
}

void TankObject::Update(float fElapsedTime)
{
	ExplosiveObject::Update(fElapsedTime);
}

void TankObject::Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera)
{
	ExplosiveObject::Render(hDCFrameBuffer, pCamera);
}

void TankObject::OnCollision(std::shared_ptr<GameObject> pOther)
{
	if (auto p = dynamic_pointer_cast<TankPlayer>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<TankObject>(pOther)) {
		XMFLOAT3 xmf3MovingDirection = p->GetMovingDirection();
		float fMovingSpeed = p->GetMovingSpeed();

		SetMovingDirection(xmf3MovingDirection);
		SetMovingSpeed(fMovingSpeed);
	}
	else if (auto p = dynamic_pointer_cast<BulletObject>(pOther)) {
		OnPicked();
	}
	else if (auto p = dynamic_pointer_cast<WallsObject>(pOther)) {
	
	}
	else if (auto p = dynamic_pointer_cast<ObstacleObject>(pOther)) {
	
	}
}
