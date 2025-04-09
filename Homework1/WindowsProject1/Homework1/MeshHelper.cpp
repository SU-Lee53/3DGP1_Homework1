#include "stdafx.h"
#include <fstream>
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

void MeshHelper::AddCircleMeshToPolygonBuffer(std::vector<std::shared_ptr<struct Polygon>> pPolygons, float fRadius, int nVertices, const XMFLOAT2& center)
{
	float stride = nVertices / 360;

	for (const int& deg : std::views::iota(1, nVertices) | std::views::transform([stride](int level) {return level * stride; })) {

		float xCircle = fRadius * std::cos(deg);
		float yCircle = fRadius * std::sin(deg);




	}
}

BOOL MeshHelper::CreateMeshFromOBJFiles(std::shared_ptr<Mesh> pMesh, std::wstring_view wstrObjPath)
{
	std::ifstream in{ wstrObjPath.data()};

	if (!in) return FALSE;

	std::vector<XMFLOAT3> LoadedVertices;
	std::vector<UINT> LoadedIndices;
	while (in) {




	}


}

void MeshHelper::CreateRollercoasterRailMesh(std::shared_ptr<Mesh> pMesh, float fWidth, float fCourseRadius, int nControlPoints, int nInterpolateBias)
{
	assert(nControlPoints != 0);

	std::vector<XMFLOAT3> ControlPoints(nControlPoints);
	std::vector<XMFLOAT3> Tangents(nControlPoints);
	
	// 1. Generate control points
	auto GenerateControlPoint = [fCourseRadius, nControlPoints](int idx) {
		XMFLOAT3 v;
		v.x = fCourseRadius * XMScalarCos(XMConvertToRadians((360.0f / nControlPoints) * idx));
		v.y = fCourseRadius * XMScalarSin(XMConvertToRadians((360.0f / nControlPoints) * idx));
		v.z = RandomGenerator::GenerateRandomFloatInRange(5.0f, 30.0f);
		return v;
	};

	for (int i = 0; i < nControlPoints; ++i) {
		ControlPoints[i] = GenerateControlPoint(i);
	}


	// 2. Generate tangent automatically
	XMStoreFloat3(&Tangents[0], XMVectorZero());
	XMStoreFloat3(&Tangents[nControlPoints - 1], XMVectorZero());

	int count = 1;
	for (auto& [v1, v2, v3] : ControlPoints | std::views::adjacent<3>) {
		// Calculate tangent of v2
		XMVECTOR xmvCP1 = XMLoadFloat3(&v1);
		XMVECTOR xmvCP3 = XMLoadFloat3(&v3);

		XMStoreFloat3(&Tangents[count++], XMVectorScale(XMVectorSubtract(xmvCP1, xmvCP3), 0.5f));
	}

	// 3. Generate spline vertex

	std::vector<Vertex> SplineVertices;

	for (int i = 0; i < nControlPoints - 1; ++i) {
		XMVECTOR xmvControlPoint1 = XMLoadFloat3(&ControlPoints[i]);
		XMVECTOR xmvControlPoint2 = XMLoadFloat3(&ControlPoints[i + 1]);
		XMVECTOR xmvTangent1 = XMLoadFloat3(&Tangents[i]);
		XMVECTOR xmvTangent2 = XMLoadFloat3(&Tangents[i + 1]);

		for (int j = 0; j < nInterpolateBias; j++) {
			float t = (float)j / nInterpolateBias;
			XMVECTOR xmvPoint = XMVectorHermite(xmvControlPoint1, xmvTangent1, xmvControlPoint2, xmvTangent2, t);
			XMFLOAT3 xmf3Point;
			XMStoreFloat3(&xmf3Point, xmvPoint);
			SplineVertices.emplace_back(xmf3Point);
		}
	}

	// add last ~ first spline 
	{
		XMVECTOR xmvControlPoint1 = XMLoadFloat3(&ControlPoints.back());
		XMVECTOR xmvControlPoint2 = XMLoadFloat3(&ControlPoints.front());
		XMVECTOR xmvTangent1 = XMLoadFloat3(&Tangents.back());
		XMVECTOR xmvTangent2 = XMLoadFloat3(&Tangents.front());

		for (int j = 0; j <= nInterpolateBias; j++) {
			float t = (float)j / nInterpolateBias;
			XMVECTOR xmvPoint = XMVectorHermite(xmvControlPoint1, xmvTangent1, xmvControlPoint2, xmvTangent2, t);
			XMFLOAT3 xmf3Point;
			XMStoreFloat3(&xmf3Point, xmvPoint);
			SplineVertices.emplace_back(xmf3Point);
		}
	}

	// For test : Draw Spline like line strip
	std::shared_ptr<struct Polygon> pLineStrip = std::make_shared<struct Polygon>(SplineVertices.size());
	for (const auto& [index, point] : std::views::enumerate(SplineVertices)) {
		pLineStrip->SetVertex(index, point);
	}

	pMesh->SetPolygon(1, pLineStrip);


	// 이후 계획

	/*

		  v0 ? ----------- ? v3
	         |             |    
		p[i] ? ----------- ? p[i+1]
	         |             |
		  v1 ? ----------- ? v2

		  v0, v1 : normalize(cross(p[i+1] - p[i], up)) 하여 방향을 찾고 너비만큼의 간격으로 점을 찾으면 됨 (+- 방향 * (width / 2))
		  v2, v3 : normalize(cross(p[i+2] - p[i+1], up)) 하여 방향을 찾고 적절한 너비만큼의 간격으로 점을 찾으면 됨 (+- 방향 * (width / 2))

	*/


}
