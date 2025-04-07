#include "stdafx.h"
#include "MeshHelper.h"
#include "Mesh.h"

using namespace std;

void MeshHelper::CreateCubeMesh(std::shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth)
{
	pMesh->m_pPolygons.resize(6);

	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	std::shared_ptr<struct Polygon> pFrontFace = std::make_shared<struct Polygon>(4);
	pFrontFace->SetVertex(0, Vertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(1, Vertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(2, Vertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(3, Vertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pMesh->SetPolygon(0, pFrontFace);

	std::shared_ptr<struct Polygon> pTopFace = std::make_shared<struct Polygon>(4);
	pTopFace->SetVertex(0, Vertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pTopFace->SetVertex(1, Vertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pTopFace->SetVertex(2, Vertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pTopFace->SetVertex(3, Vertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pMesh->SetPolygon(1, pTopFace);

	std::shared_ptr<struct Polygon> pBackFace = std::make_shared<struct Polygon>(4);
	pBackFace->SetVertex(0, Vertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(1, Vertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(2, Vertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(3, Vertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pMesh->SetPolygon(2, pBackFace);

	std::shared_ptr<struct Polygon> pBottomFace = std::make_shared<struct Polygon>(4);
	pBottomFace->SetVertex(0, Vertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pBottomFace->SetVertex(1, Vertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pBottomFace->SetVertex(2, Vertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBottomFace->SetVertex(3, Vertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pMesh->SetPolygon(3, pBottomFace);

	std::shared_ptr<struct Polygon> pLeftFace = std::make_shared<struct Polygon>(4);
	pLeftFace->SetVertex(0, Vertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pLeftFace->SetVertex(1, Vertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pLeftFace->SetVertex(2, Vertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pLeftFace->SetVertex(3, Vertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pMesh->SetPolygon(4, pLeftFace);

	std::shared_ptr<struct Polygon> pRightFace = std::make_shared<struct Polygon>(4);
	pRightFace->SetVertex(0, Vertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pRightFace->SetVertex(1, Vertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pRightFace->SetVertex(2, Vertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pRightFace->SetVertex(3, Vertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pMesh->SetPolygon(5, pRightFace);

	pMesh->m_xmOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

void MeshHelper::CreateWallMesh(std::shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth, int nSubRects)
{
	pMesh->m_pPolygons.resize((4 * nSubRects * nSubRects) + 2);

	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;
	float fCellWidth = fWidth * (1.0f / nSubRects);
	float fCellHeight = fHeight * (1.0f / nSubRects);
	float fCellDepth = fDepth * (1.0f / nSubRects);

	int k = 0;
	shared_ptr<struct Polygon> pLeftFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pLeftFace = make_shared<struct Polygon>(4);
			pLeftFace->SetVertex(0, Vertex(-fHalfWidth, -fHalfHeight + (i * fCellHeight), -fHalfDepth + (j * fCellDepth)));
			pLeftFace->SetVertex(1, Vertex(-fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight), -fHalfDepth + (j * fCellDepth)));
			pLeftFace->SetVertex(2, Vertex(-fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight), -fHalfDepth + ((j + 1) * fCellDepth)));
			pLeftFace->SetVertex(3, Vertex(-fHalfWidth, -fHalfHeight + (i * fCellHeight), -fHalfDepth + ((j + 1) * fCellDepth)));
			pMesh->SetPolygon(k++, pLeftFace);
		}
	}

	shared_ptr<struct Polygon> pRightFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pRightFace = make_shared<struct Polygon>(4);
			pRightFace->SetVertex(0, Vertex(+fHalfWidth, -fHalfHeight + (i * fCellHeight), -fHalfDepth + (j * fCellDepth)));
			pRightFace->SetVertex(1, Vertex(+fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight), -fHalfDepth + (j * fCellDepth)));
			pRightFace->SetVertex(2, Vertex(+fHalfWidth, -fHalfHeight + ((i + 1) * fCellHeight), -fHalfDepth + ((j + 1) * fCellDepth)));
			pRightFace->SetVertex(3, Vertex(+fHalfWidth, -fHalfHeight + (i * fCellHeight), -fHalfDepth + ((j + 1) * fCellDepth)));
			pMesh->SetPolygon(k++, pRightFace);
		}
	}

	shared_ptr<struct Polygon> pTopFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pTopFace = make_shared<struct Polygon>(4);
			pTopFace->SetVertex(0, Vertex(-fHalfWidth + (i * fCellWidth), +fHalfHeight, -fHalfDepth + (j * fCellDepth)));
			pTopFace->SetVertex(1, Vertex(-fHalfWidth + ((i + 1) * fCellWidth), +fHalfHeight, -fHalfDepth + (j * fCellDepth)));
			pTopFace->SetVertex(2, Vertex(-fHalfWidth + ((i + 1) * fCellWidth), +fHalfHeight, -fHalfDepth + ((j + 1) * fCellDepth)));
			pTopFace->SetVertex(3, Vertex(-fHalfWidth + (i * fCellWidth), +fHalfHeight, -fHalfDepth + ((j + 1) * fCellDepth)));
			pMesh->SetPolygon(k++, pTopFace);
		}
	}

	shared_ptr<struct Polygon> pBottomFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pBottomFace = make_shared<struct Polygon>(4);
			pBottomFace->SetVertex(0, Vertex(-fHalfWidth + (i * fCellWidth), -fHalfHeight, -fHalfDepth + (j * fCellDepth)));
			pBottomFace->SetVertex(1, Vertex(-fHalfWidth + ((i + 1) * fCellWidth), -fHalfHeight, -fHalfDepth + (j * fCellDepth)));
			pBottomFace->SetVertex(2, Vertex(-fHalfWidth + ((i + 1) * fCellWidth), -fHalfHeight, -fHalfDepth + ((j + 1) * fCellDepth)));
			pBottomFace->SetVertex(3, Vertex(-fHalfWidth + (i * fCellWidth), -fHalfHeight, -fHalfDepth + ((j + 1) * fCellDepth)));
			pMesh->SetPolygon(k++, pBottomFace);
		}
	}

	shared_ptr<struct Polygon> pFrontFace = make_shared<struct Polygon>(4);
	pFrontFace->SetVertex(0, Vertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(1, Vertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(2, Vertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(3, Vertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pMesh->SetPolygon(k++, pFrontFace);

	shared_ptr<struct Polygon> pBackFace = make_shared<struct Polygon>(4);
	pBackFace->SetVertex(0, Vertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(1, Vertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(2, Vertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(3, Vertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pMesh->SetPolygon(k++, pBackFace);

	pMesh->m_xmOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

void MeshHelper::CreateAirplaneMesh(std::shared_ptr<Mesh> pMesh, float fWidth, float fHeight, float fDepth)
{
	pMesh->m_pPolygons.resize(24);

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;

	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);
	int i = 0;

	//Upper Plane
	shared_ptr<struct Polygon> pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, Vertex(+x1, -y1, -fz));
	pFace->SetVertex(2, Vertex(0.0f, 0.0f, -fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, Vertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(2, Vertex(-x1, -y1, -fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(+x2, +y2, -fz));
	pFace->SetVertex(1, Vertex(+fx, -y3, -fz));
	pFace->SetVertex(2, Vertex(+x1, -y1, -fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(-x2, +y2, -fz));
	pFace->SetVertex(1, Vertex(-x1, -y1, -fz));
	pFace->SetVertex(2, Vertex(-fx, -y3, -fz));
	pMesh->SetPolygon(i++, pFace);

	//Lower Plane
	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, Vertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, Vertex(+x1, -y1, +fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, Vertex(-x1, -y1, +fz));
	pFace->SetVertex(2, Vertex(0.0f, 0.0f, +fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(+x2, +y2, +fz));
	pFace->SetVertex(1, Vertex(+x1, -y1, +fz));
	pFace->SetVertex(2, Vertex(+fx, -y3, +fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(-x2, +y2, +fz));
	pFace->SetVertex(1, Vertex(-fx, -y3, +fz));
	pFace->SetVertex(2, Vertex(-x1, -y1, +fz));
	pMesh->SetPolygon(i++, pFace);

	//Right Plane
	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, Vertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, Vertex(+x2, +y2, -fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(+x2, +y2, -fz));
	pFace->SetVertex(1, Vertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, Vertex(+x2, +y2, +fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(+x2, +y2, -fz));
	pFace->SetVertex(1, Vertex(+x2, +y2, +fz));
	pFace->SetVertex(2, Vertex(+fx, -y3, -fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(+fx, -y3, -fz));
	pFace->SetVertex(1, Vertex(+x2, +y2, +fz));
	pFace->SetVertex(2, Vertex(+fx, -y3, +fz));
	pMesh->SetPolygon(i++, pFace);

	//Back/Right Plane
	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(+x1, -y1, -fz));
	pFace->SetVertex(1, Vertex(+fx, -y3, -fz));
	pFace->SetVertex(2, Vertex(+fx, -y3, +fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(+x1, -y1, -fz));
	pFace->SetVertex(1, Vertex(+fx, -y3, +fz));
	pFace->SetVertex(2, Vertex(+x1, -y1, +fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, Vertex(+x1, -y1, -fz));
	pFace->SetVertex(2, Vertex(+x1, -y1, +fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, Vertex(+x1, -y1, +fz));
	pFace->SetVertex(2, Vertex(0.0f, 0.0f, +fz));
	pMesh->SetPolygon(i++, pFace);

	//Left Plane
	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, Vertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(2, Vertex(-x2, +y2, -fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, Vertex(-x2, +y2, -fz));
	pFace->SetVertex(2, Vertex(-x2, +y2, +fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(-x2, +y2, +fz));
	pFace->SetVertex(1, Vertex(-x2, +y2, -fz));
	pFace->SetVertex(2, Vertex(-fx, -y3, -fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(-x2, +y2, +fz));
	pFace->SetVertex(1, Vertex(-fx, -y3, -fz));
	pFace->SetVertex(2, Vertex(-fx, -y3, +fz));
	pMesh->SetPolygon(i++, pFace);

	//Back/Left Plane
	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, Vertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, Vertex(-x1, -y1, +fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, Vertex(-x1, -y1, +fz));
	pFace->SetVertex(2, Vertex(-x1, -y1, -fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(-x1, -y1, -fz));
	pFace->SetVertex(1, Vertex(-x1, -y1, +fz));
	pFace->SetVertex(2, Vertex(-fx, -y3, +fz));
	pMesh->SetPolygon(i++, pFace);

	pFace = make_shared<struct Polygon>(3);
	pFace->SetVertex(0, Vertex(-x1, -y1, -fz));
	pFace->SetVertex(1, Vertex(-fx, -y3, +fz));
	pFace->SetVertex(2, Vertex(-fx, -y3, -fz));
	pMesh->SetPolygon(i++, pFace);

	pMesh->m_xmOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fx, fy, fz), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}
