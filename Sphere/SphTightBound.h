#pragma once
#include <stdio.h>
#include "SphInclude.h"
#include "SphBound.h"

//SphTightBound is a SphBound with a tight bound.
class SphTightBound : public SphBound {

protected:
	int m_vertexCount;
	int m_indexCount;
	float* m_vertices;
	float* m_originalVertices;
	ushort* m_indices;

	void LoadHead(FILE* const file);
	void LoadBody(FILE* const file);

public:
	SphBound* m_looseBound;

	SphTightBound(SphObject* object);
	bool loadChunk(float xs, float xf, float zs, float zf, float y1, float y2, float y3, float y4);
	void Render();
	void UpdateBound();
	bool Load(cchar* filePath);
	int GetIndexCount();
	cfloat* GetVertices();
	cushort* GetIndices();
};
