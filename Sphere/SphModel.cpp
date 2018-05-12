#include <cstring>
#include "SphOpenGL.h"
#include "SphRenderer.h"
#include "SphString.h"
#include "SphMath.h"
#include "SphModel.h"
#include "SphAction.h"
#include "SphBone.h"
#include "SphMesh.h"

//Create SphModel.
SphModel::SphModel(){
	this->m_bones = NULL;
}

//Load SphModel from a file.
//directory - containing the file.
//filename - of the model to load.
void SphModel::Load(cchar* directory, cchar* filename){
	char* filePath = ConcatNew(directory, filename);
	FILE* file = fopen(filePath, "r");

	fscanf(file, "bones %d\n", &m_boneCount);
	m_bones = new SphBone[m_boneCount];
	LoadBones(file, directory);
	LoadActions(file);

	fclose(file);
	delete [] filePath;
}

//action - index to play.
//offset - seconds into the action.
//loop - true if the action should loop.
//persistent - true if the action should still be active when it reaches the end.
//return - length of the action in seconds.
float SphModel::PlayAction(int action, float offset, bool loop, bool persistent){
	if(!m_actions[action].IsActive()){
		m_activeActions.push_front(&m_actions[action]);
	}
	return m_actions[action].Activate(offset, loop, persistent);
}

//Stop playing an action.
//action - index.
//immediately - if true, then the action will be stopped this instant. If false, then wait until it's current loop is done.
//return - seconds until the action is stopped.
float SphModel::StopAction(int action, bool immediately){
	if(immediately){
		m_activeActions.remove(&m_actions[action]);
	}
	return m_actions[action].Deactivate(immediately);
}

//return - action by the given action index.
SphAction* SphModel::GetAction(int action){
	return m_actions + action;
}

//Update SphModel.
//seconds - since the last call.
void SphModel::Update(float seconds){
	Set(m_offset, g_zeroes);
	SphActionList::iterator iter = m_activeActions.begin();
	for(SphActionList::iterator iter = m_activeActions.begin(); iter != m_activeActions.end(); iter++){
		SphAction* action = *iter;
		action->Update(seconds);
		action->AddToOffset(m_offset);
	}
}

//Render SphModel.
void SphModel::Render(){
	glPushMatrix();
	ApplyMatrix();
	glTranslatef(m_offset[O_X], m_offset[O_Y], m_offset[O_Z]);
	float curPos[N_XYZ], curQuat[N_QUAT];
	Set(curPos, m_offset);
	Set(curQuat, 1.0f, 0.0f, 0.0f, 0.0f);
	m_bones[0].Render(this, curPos, curQuat);
	glPopMatrix();
}

//boneIndex - bone whose rotation is affected.
//rotation - bone's rotation.
//return - number of actions that contributed to the rotation.
int SphModel::AddToRotation(int boneIndex, float* rotation){
	int actionCount = 0;
	for(SphActionList::iterator iter = m_activeActions.begin(); iter != m_activeActions.end(); iter++){
		if((*iter)->AddToRotation(boneIndex, rotation)){
			actionCount++;
		}
	}
	return actionCount;
}

//bone - index.
//mesh - new bone's mesh.
void SphModel::SetBoneMesh(int bone, SphMesh* mesh){
	m_bones[bone].m_mesh = mesh;
}

//bone - index.
//return - mesh used by the bone.
SphMesh* SphModel::GetBoneMesh(int bone){
	return m_bones[bone].m_mesh;
}

//return - bone's end in model's coordinates.
cfloat* SphModel::GetBoneEnd(int bone){
	return m_bones[bone].GetBoneEnd();
}

//return - bone's start in model's coordinates.
cfloat* SphModel::GetBoneStart(int bone){
	return m_bones[bone].GetBoneStart();
}

//Free SphModel and all associated resources.
void SphModel::Free(){
	for(int n = 0; n < m_boneCount; n++){
		m_bones[n].Free();
	}
}

//Load SphBones from file.
//file - to load from.
//directory - to search for meshes.
void SphModel::LoadBones(FILE* file, cchar* directory){
	int boneIndex, parentIndex;
	char meshName[64];
	SphMesh* mesh;

	for(int n = 0; n < m_boneCount; n++){
		fscanf(file, "bone %d %d %s\n", &boneIndex, &parentIndex, meshName);
		SphBone& bone = m_bones[boneIndex];
		bone.Load(file, boneIndex);
		if(parentIndex >= 0){
			m_bones[parentIndex].AddChild(&bone);
		}

		mesh = new SphMesh();
		if(mesh->Load(directory, meshName)){
			bone.m_mesh = mesh;
		} else {
			DELETE(mesh);
		}
	}
}

//Load actions from a file.
//file - to load from.
void SphModel::LoadActions(FILE* file){
	fscanf(file, "actions %d\n", &m_actionCount);
	m_actions = new SphAction[m_actionCount];

	//read all actions
	for(int n = 0; n < m_actionCount; n++){
		int actionIndex;
		fscanf(file, "action %d\n", &actionIndex);
		SphAction& action = m_actions[actionIndex];
		action.Load(file, m_boneCount);
	}
}

//SphModel destructor.
SphModel::~SphModel(){
}
