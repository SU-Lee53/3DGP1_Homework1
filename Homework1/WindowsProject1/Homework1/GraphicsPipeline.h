#pragma once

#include "Camera.h"

class GraphicsPipeline {
public:
	static XMFLOAT3 ScreenTransform(const XMFLOAT3& xmf3Project);
	static XMFLOAT3 Project(const XMFLOAT3& xmf3Model);
	static XMFLOAT3 Transfrom(const XMFLOAT3& xmf3Model);
	static void Draw2DLine(HDC hDCFrameBuffer, const XMFLOAT3& f3PreviousProject, const XMFLOAT3& f3CurrentProject);

	static void SetWorldTransform(XMFLOAT4X4& pxmf4x4World) {
		m_pxmf4x4World = &pxmf4x4World;
	}

	static void SetViewPerspectiveProjectTransform(XMFLOAT4X4& pxmf4x4ViewPerspectiveProject) {
		m_pxmf4x4ViewProject = &pxmf4x4ViewPerspectiveProject;
	}
	
	static void SetViewOrthographicProjectTransform(XMFLOAT4X4& pxmf4x4OrthographicProject) {
		m_pxmf4x4ViewProject = &pxmf4x4OrthographicProject;
	}
	
	static void SetViewport(Viewport* pViewport) { m_pViewport = pViewport; }


private:
	static XMFLOAT4X4*	m_pxmf4x4World;
	static XMFLOAT4X4*	m_pxmf4x4ViewProject;
	static Viewport*	m_pViewport;
};

