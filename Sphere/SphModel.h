#pragma once
#include <stdio.h>
#include <list>
#include "SphInclude.h"
#include "SphObject.h"
using namespace std;

class SphBone;
class SphAction;
class SphMesh;

typedef list<SphAction*> SphActionList;

//SphModel stores a model skeleton, model animations, as well as links to necessary meshes.
class SphModel : virtual public SphObject {

protected:
	SphBone* m_bones;
	SphAction* m_actions;
	SphActionList m_activeActions;
	float m_offset[N_XYZ];//main bone's current offset
	int m_boneCount;
	int m_actionCount;

	void LoadBones(FILE* file, cchar* directory);
	void LoadActions(FILE* file);

public:
	SphModel();
	void Render();
	void Update(float seconds);
	float PlayAction(int action, float offset = 0.0f, bool loop = false, bool persistent = false);
	float StopAction(int action, bool immediately = false);
	SphAction* GetAction(int action);

	int  AddToRotation(int boneIndex, float* rotation);
	void SetBoneMesh(int bone, SphMesh* mesh);
	SphMesh* GetBoneMesh(int bone);
	cfloat* GetBoneEnd(int bone);
	cfloat* GetBoneStart(int bone);

	void Load(cchar* directory, cchar* filename);
	void Free();
	virtual ~SphModel();
};
