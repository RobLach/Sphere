#pragma once
#include <stdio.h>
#include "SphInclude.h"
#include "SphObject.h"

struct SphTempMeshData;
class SphMaterial;

//SphMesh contains information about a .drkMesh file.
class SphMesh : public SphObject {

private:
	int m_submeshCount;
	int* m_indexCounts;
	bool m_textured;

	uint m_vertexBuffer;
	uint m_normalBuffer;
	uint m_uvBuffer;
	uint* m_indexBuffers;

	void LoadHead(FILE* const file, SphTempMeshData* tempData);
	void LoadBody(FILE* const file, cchar* directory, SphTempMeshData* tempData);
	void CreateBuffers(SphTempMeshData* tempData);

protected:
	SphMaterial* m_materials;

public:
	SphMesh(cchar* directory = NULL, cchar* filename = NULL);
	bool Load(cchar* directory, cchar* filename);
	virtual void Render();
	virtual void Free();
};
