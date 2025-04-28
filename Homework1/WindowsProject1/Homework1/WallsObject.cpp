#include "stdafx.h"
#include "WallsObject.h"

WallsObject::WallsObject()
{
}

WallsObject::~WallsObject()
{
}

void WallsObject::Render(HDC hDCFrameBuffer, std::shared_ptr<Camera> pCamera)
{
	GameObject::Render(hDCFrameBuffer, m_pTransform->GetWorldMatrix(), m_pMesh);
}
