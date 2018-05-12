#pragma once
#include <stdio.h>
#include "SphInclude.h"

class SphMesh;
class SphModel;

//SphBone is simple skeleton bone, corresponding to a mesh.
class SphBone{

private:
	SphBone* m_sibling;		//points to the next bone that is a child of this bone's parent (essentially linked list).
	SphBone* m_child;			//points to the first bone that is a child of this bone (allows for tree structure).
	int m_index;				//bone's index
	float m_offset;			//bone's length
	float m_boneQuat[N_QUAT];//bone's rotation relative to parent bone (in quaternion form)
	float m_boneRot[N_ROT];	//bone's rotation relative to parent bone
	float m_meshRot[N_ROT];	//bone's rotation for the mesh
	float m_start[N_XYZ];	//global position of the bone's start
	float m_end[N_XYZ];		//global position of the bone's end

public:
	SphMesh* m_mesh;

	SphBone();
	void Render(SphModel* model, float* curPos, float* curQuat);
	void Load(FILE* file, int boneIndex);
	void AddChild(SphBone* child);
	cfloat* GetBoneEnd();
	cfloat* GetBoneStart();
	void Free();
};
