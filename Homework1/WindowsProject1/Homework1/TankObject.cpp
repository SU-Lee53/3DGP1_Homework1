#include "stdafx.h"
#include "TankObject.h"
#include "TankPlayer.h"
#include "BulletObject.h"

TankObject::TankObject()
{
}

TankObject::~TankObject()
{
}

void TankObject::Initialize()
{
}

void TankObject::Update(float fElapsedTime)
{
	ExplosiveObject::Update(fElapsedTime);
}

void TankObject::Render(HDC hDCFrameBuffer, std::shared_ptr<class Camera> pCamera)
{
	GameObject::Render(hDCFrameBuffer, pCamera);
}

void TankObject::OnCollision(std::shared_ptr<GameObject> pOther)
{
	if (auto p = dynamic_pointer_cast<TankPlayer>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<TankObject>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<BulletObject>(pOther)) {

	}
	//else if (auto p = dynamic_pointer_cast<class WallObject>(pOther)) {
	//
	//}
}
