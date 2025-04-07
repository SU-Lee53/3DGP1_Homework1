#pragma once

class Mesh;

class MeshHelper {
public:
	static void CreateCubeMesh(std::shared_ptr<Mesh> pMesh,
		float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);

	static void CreateWallMesh(std::shared_ptr<Mesh> pMesh,
		float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f, int nSubRects = 20);

	static void CreateAirplaneMesh(std::shared_ptr<Mesh> pMesh,
		float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);

};

