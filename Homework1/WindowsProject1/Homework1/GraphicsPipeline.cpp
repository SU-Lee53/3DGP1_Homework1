#include "stdafx.h"
#include "GraphicsPipeline.h"

XMFLOAT4X4* GraphicsPipeline::m_pxmf4x4World = nullptr;
XMFLOAT4X4* GraphicsPipeline::m_pxmf4x4ViewProject = nullptr;
Viewport* GraphicsPipeline::m_pViewport = nullptr;

XMFLOAT3 GraphicsPipeline::ScreenTransform(const XMFLOAT3& xmf3Project)
{
	XMFLOAT3 f3Screen = xmf3Project;

	float fHalfWidth = m_pViewport->m_nWidth * 0.5f;
	float fHalfHeight = m_pViewport->m_nHeight * 0.5f;
	f3Screen.x = m_pViewport->m_nLeft + (xmf3Project.x * fHalfWidth) + fHalfWidth;
	f3Screen.y = m_pViewport->m_nTop + (-xmf3Project.y * fHalfHeight) + fHalfHeight;

	return f3Screen;
}

XMFLOAT3 GraphicsPipeline::Project(const XMFLOAT3& xmf3Model)
{
	XMFLOAT4X4 xmf4x4Transform = Matrix4x4::Multiply(*m_pxmf4x4World, *m_pxmf4x4ViewProject);
	XMFLOAT3 xmf3Project = Vector3::TransformCoord(xmf3Model, xmf4x4Transform);
	return xmf3Project;
}

XMFLOAT3 GraphicsPipeline::Transfrom(const XMFLOAT3& xmf3Model)
{
	XMFLOAT3 xmf3Project = Project(xmf3Model);
	XMFLOAT3 f3Screen = ScreenTransform(xmf3Project);
	return f3Screen;
}

void GraphicsPipeline::Draw2DLine(HDC hDCFrameBuffer, const XMFLOAT3& f3PreviousProject, const XMFLOAT3& f3CurrentProject)
{
	XMFLOAT3 f3Previous = ScreenTransform(f3PreviousProject);
	XMFLOAT3 f3Current = ScreenTransform(f3CurrentProject);
	::MoveToEx(hDCFrameBuffer, (long)f3Previous.x, (long)f3Previous.y, NULL);
	::LineTo(hDCFrameBuffer, (long)f3Current.x, (long)f3Current.y);
}
