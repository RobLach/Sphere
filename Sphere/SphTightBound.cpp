#include <string.h>
#include "SphMath.h"
#include "SphRenderer.h"
#include "SphObject.h"
#include "SphTightBound.h"
#include "SphSphericBound.h"

//Create SphTightBound.
SphTightBound::SphTightBound(SphObject* object) : SphBound(object){
	this->m_boundType = Tight;
	this->m_looseBound = NULL;
}

//Render SphTightBound.
void SphTightBound::Render(){
	RenderIndexedVertices(m_indexCount, m_indices, m_originalVertices);
}

//Update this bound.
void SphTightBound::UpdateBound(){
	cfloat* quat = m_object->GetRotation();
	cfloat* translation = m_object->GetPosition();
	float theta = 2.0f * acos(quat[O_QW]);
	float cosTheta = Cos(theta);
	float sinTheta = Sin(theta);
	float axis[N_XYZ]; Set(axis, quat + O_QX);
	float magn = Magnitude(axis);
	Multiply(axis, 1.0f / magn);

	for(int n = 0; n < m_vertexCount; n++){
		float* result = m_vertices + n * N_XYZ;
		cfloat* vertex = m_originalVertices + n * N_XYZ;

		if(magn > BIG_EPSILON){
			//apply quaternion rotation
			float uXuDOTv[N_XYZ], temp[N_XYZ];
			Set(uXuDOTv, axis);
			Multiply(uXuDOTv, Dot(axis, vertex));
			Subtract(temp, vertex, uXuDOTv);
			VectorPlusFactorVector(result, uXuDOTv, cosTheta, temp);
			Cross(temp, axis, vertex);
			VectorPlusFactorVector(result, result, sinTheta, temp);
		}

		AddTo(result, translation);
	}

	if(m_looseBound != NULL)m_looseBound->UpdateBound();
}

//return - number of indices this bound has.
int SphTightBound::GetIndexCount(){
	return m_indexCount;	
}

//return - bound's vertices.
cfloat* SphTightBound::GetVertices(){
	return m_vertices;
}

//return - bound's indices.
cushort* SphTightBound::GetIndices(){
	return m_indices;
}

//Load SphTightBound.
//filePath - path to the file to load the bound from.
bool SphTightBound::Load(cchar* filePath){
	FILE* file = fopen(filePath, "r");
	bool success = false;
	if(file){
		LoadHead(file);
		LoadBody(file);
		fclose(file);

		success = true;
	}
	return success;
}

//Load the head of the file.
//file - used to read the info from.
void SphTightBound::LoadHead(FILE* const file){
	fscanf(file, "count %d\n", & m_vertexCount);
	m_vertices = new float[m_vertexCount * N_XYZ];
	m_originalVertices = new float[m_vertexCount * N_XYZ];

	int faces, submeshCount;
	fscanf(file, "submeshes %d\n", &submeshCount);//MUST BE 1
	fscanf(file, "faces %d\n", &faces);
	m_indexCount = faces * N_TRI;
	m_indices = new ushort[m_indexCount];
}

//Load the body of the file.
//file - used to read the info from.
void SphTightBound::LoadBody(FILE* const file){
	int vIndex = 0;
	int iIndex = 0;
	float garbageFloat = 0;
	float maxDistanceSq = 0;
	char prefix[32];

	while(fscanf(file, "%s ", prefix) != EOF){
		if(!strcmp(prefix, "v")){
			fscanf(file, "%f %f %f %f %f %f\n", m_vertices + vIndex + O_X, m_vertices + vIndex + O_Y, m_vertices + vIndex + O_Z, &garbageFloat, &garbageFloat, &garbageFloat);
			Set(m_originalVertices + vIndex, m_vertices + vIndex);
			maxDistanceSq = Max(maxDistanceSq, MagnitudeSq(m_originalVertices + vIndex));
			vIndex += N_XYZ;
		} else if(!strcmp(prefix, "face")){
			fscanf(file, "%d %d %d\n", m_indices + iIndex + 0, m_indices + iIndex + 1, m_indices + iIndex + 2);
			iIndex += N_TRI;
		}
	}

	m_looseBound = new SphSphericBound(m_object, sqrt(maxDistanceSq));
}

// Loads a chunk.
bool SphTightBound::loadChunk(float xs, float xf, float zs, float zf, float y1, float y2, float y3, float y4){

	m_vertexCount = 4 * N_XYZ;
	m_vertices = new float[m_vertexCount];
	m_originalVertices = new float[m_vertexCount];

	m_indexCount = 6;
	m_indices = new ushort[m_indexCount];

	int vIndex = 0, iIndex = 0;

	m_vertices[vIndex + O_X] = xs;
	m_vertices[vIndex + O_Y] = y1;
	m_vertices[vIndex + O_Z] = zs; 
	vIndex += 3;

	m_vertices[vIndex + O_X] = xs;
	m_vertices[vIndex + O_Y] = y2;
	m_vertices[vIndex + O_Z] = zf; 
	vIndex += 3;

	m_vertices[vIndex + O_X] = xf;
	m_vertices[vIndex + O_Y] = y3;
	m_vertices[vIndex + O_Z] = zf; 
	vIndex += 3;

	m_vertices[vIndex + O_X] = xf;
	m_vertices[vIndex + O_Y] = y4;
	m_vertices[vIndex + O_Z] = zs; 
	vIndex += 3;

	m_indices[0] = 0;
	m_indices[1] = 3;
	m_indices[2] = 1;
	m_indices[3] = 2;
	m_indices[4] = 3;
	m_indices[5] = 1;

	return true;
}