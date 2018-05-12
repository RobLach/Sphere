#include <cstring>
#include "SphMath.h"
#include "SphString.h"
#include "SphOpenGL.h"
#include "SphMesh.h"
#include "SphMaterial.h"
#include "SphGame.h"

//SphTempMeshData stores temporary data until buffers are created.
struct SphTempMeshData{
	int vertexCount;
	float* vertices;
	float* normals;
	float* uvs;
	ushort** indices;
	SphTempMeshData() : vertices(NULL), normals(NULL), uvs(NULL), indices(NULL){}
};

//Create SphMesh.
SphMesh::SphMesh(cchar* directory, cchar* filename){
	if(directory != NULL){
		Load(directory, filename);
	}
}

//Load SphMesh from a file.
//directory - containing the file.
//filename - of the mesh to load.
//return - true if the mesh was loaded successfully.
bool SphMesh::Load(cchar* directory, cchar* filename){
	char* filePath = ConcatNew(directory, filename);
	FILE* file = fopen(filePath, "r");
	bool success = false;
	if(file){
		SphTempMeshData tempData;

		LoadHead(file, &tempData);
		LoadBody(file, directory, &tempData);
		CreateBuffers(&tempData);

		DELETE_ARRAY(tempData.vertices);
		DELETE_ARRAY(tempData.normals);
		DELETE_ARRAY(tempData.uvs);
		DELETE_ARRAY2(tempData.indices, m_submeshCount);

		fclose(file);
		success = true;
	}
	delete filePath;
	return success;
}

//Render the mesh.
 void SphMesh::Render(){
	//grassFieldList = glGenLists(1);
	//glNewList(grassFieldList, GL_COMPILE);
	glLineWidth(1.0f);

	for(int n = 0; n < m_submeshCount; n++){
		m_materials[n].ActivateMaterial();

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glVertexPointer(N_XYZ, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
		glNormalPointer(GL_FLOAT, 0, NULL);
		if(m_textured){
			glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
			glTexCoordPointer(N_UV, GL_FLOAT, 0, NULL);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[n]);
		

		glDrawElements(GL_TRIANGLES, m_indexCounts[n], GL_UNSIGNED_SHORT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	//glEndList();
}

//Free the SphMesh and all associated resources.
void SphMesh::Free(){
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_normalBuffer);
	if(m_textured){
		glDeleteBuffers(1, &m_uvBuffer);
	}
	for(int n = 0; n < m_submeshCount; n++){
		glDeleteBuffers(1, &m_indexBuffers[n]);
	}
	DELETE_ARRAY(m_indexCounts);
}

//Create necessary buffers for this mesh.
//tempData - temporary structure used to store mesh data.
void SphMesh::CreateBuffers(SphTempMeshData* tempData){
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, tempData->vertexCount * N_XYZ * sizeof(float), tempData->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, tempData->vertexCount * N_XYZ * sizeof(float), tempData->normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if(m_textured){
		glGenBuffers(1, &m_uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, tempData->vertexCount * N_UV * sizeof(float), tempData->uvs, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	for(int n = 0; n < m_submeshCount; n++){
		glGenBuffers(1, &m_indexBuffers[n]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[n]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCounts[n] * sizeof(ushort), tempData->indices[n], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

//Load the head of the file.
//file - used to read the info from.
//tempData - temporary structure used to store mesh data.
void SphMesh::LoadHead(FILE* const file, SphTempMeshData* tempData){
	fscanf(file, "count %d\n", & tempData->vertexCount);
	tempData->vertices = new float[tempData->vertexCount * N_XYZ];
	tempData->normals = new float[tempData->vertexCount * N_XYZ];
	m_textured = false;
	if(fscanf(file, "uvs\n") != EOF){
		m_textured = true;
		tempData->uvs = new float[tempData->vertexCount * N_UV];
	}

	fscanf(file, "submeshes %d\n", &m_submeshCount);
	m_indexCounts = new int[m_submeshCount];
	m_materials = new SphMaterial[m_submeshCount];
	m_indexBuffers = new uint[m_submeshCount];
	tempData->indices = new ushort*[m_submeshCount];
	for(int n = 0; n < m_submeshCount; n++){
		int faces;
		fscanf(file, "faces %d\n", &faces);
		m_indexCounts[n] = faces * N_TRI;
		tempData->indices[n] = new ushort[m_indexCounts[n]];
	}
}

//Load the body of the file.
//file - used to read the info from.
//directory - directory to search for textures.
//tempData - temporary structure used to store mesh data.
void SphMesh::LoadBody(FILE* const file, cchar* directory, SphTempMeshData* tempData){
	int index = 0;
	int iIndex = 0;
	int activeMaterial = -1;
	char prefix[32];
	while(fscanf(file, "%s ", prefix) != EOF){
		if(!strcmp(prefix, "v")){
			int vIndex = index * N_XYZ;
			int uvIndex = index * N_UV;
			if(m_textured){
				fscanf(file, "%f %f %f %f %f %f %f %f\n",
						 tempData->vertices + vIndex + O_X, tempData->vertices + vIndex + O_Y, tempData->vertices + vIndex + O_Z,
						 tempData->normals + vIndex + O_X, tempData->normals + vIndex + O_Y, tempData->normals + vIndex + O_Z,
						 tempData->uvs + uvIndex + O_U, tempData->uvs + uvIndex + O_V);
			} else {
				fscanf(file, "%f %f %f %f %f %f\n",
						 tempData->vertices + vIndex + O_X, tempData->vertices + vIndex + O_Y, tempData->vertices + vIndex + O_Z,
						 tempData->normals + vIndex + O_X, tempData->normals + vIndex + O_Y, tempData->normals + vIndex + O_Z);
			}
			index++;
		} else if(!strcmp(prefix, "face")){
			ushort* activeIndices = tempData->indices[activeMaterial];
			int tempIndices[N_TRI];
			fscanf(file, "%d %d %d\n", tempIndices + 0, tempIndices + 1, tempIndices + 2);
			Set(activeIndices + iIndex, tempIndices);
			iIndex += 3;
		} else if(!strcmp(prefix, "use")){
			fscanf(file, "%d\n", &activeMaterial);
			iIndex = 0;
		} else if(!strcmp(prefix, "material")){
			fscanf(file, "%d\n", &activeMaterial);
			SphMaterial& material = m_materials[activeMaterial];
			material.Load(file);
		} else if(!strcmp(prefix, "texture")){
			fscanf(file, "%s\n", prefix);
			char* texturePath = ConcatNew(directory, prefix);
			m_materials[activeMaterial].LoadTexture(texturePath);
			delete [] texturePath;
		}
	}
}
