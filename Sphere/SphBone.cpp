#include "SphRenderer.h"
#include "SphOpenGL.h"
#include "SphMath.h"
#include "SphBone.h"
#include "SphModel.h"
#include "SphMesh.h"

//Create SphBone.
SphBone::SphBone(){
	Set4(this->m_boneRot, g_zeroes);
	Set4(this->m_meshRot, g_zeroes);
	Set(this->m_start, g_zeroes);
	Set(this->m_end, g_zeroes);
	this->m_offset = 0.0f;
	this->m_sibling = NULL;
	this->m_child = NULL;
	this->m_mesh = NULL;
}

//file - to load SphBone from.
//boneIndex - bone index assigned to this bone.
void SphBone::Load(FILE* file, int boneIndex){
	m_index = boneIndex;
	fscanf(file, "quat %f %f %f %f\n", m_boneQuat + O_QW, m_boneQuat + O_QX, m_boneQuat + O_QY, m_boneQuat + O_QZ);
	fscanf(file, "meshQuat %f %f %f %f\n", m_meshRot + O_QW, m_meshRot + O_QX, m_meshRot + O_QY, m_meshRot + O_QZ);
	fscanf(file, "offset %f\n", &m_offset);

	QuatToRotation(m_boneRot, m_boneQuat);
	QuatToRotation(m_meshRot, m_meshRot);
}

//Render this bone and its children.
//model - model whose actions can affect this bone.
//curPos - bone's origin point.
//curQuat - bone's current rotation.
void SphBone::Render(SphModel* model, float* curPos, float* curQuat){
	float copyPos[N_XYZ]; Set(copyPos, curPos);
	float copyQuat[N_QUAT]; Set4(copyQuat, curQuat);
	if(m_sibling){
		m_sibling->Render(model, copyPos, copyQuat);
	}

	//set the bone's default rotation relative to parent bone
	glPushMatrix();
	if(m_boneRot[O_ANG] != 0.0f){
		glRotatef(m_boneRot[O_ANG], m_boneRot[O_AX], m_boneRot[O_AY], m_boneRot[O_AZ]);
		QuatMultiplyBy(curQuat, m_boneQuat);
	}

	//see how many actions affect this bone
	float quat[N_QUAT];
	Set4(quat, g_zeroes);
	int actionCount = model->AddToRotation(m_index, quat);
	//apply rotation from action(s)
	if(actionCount > 0){
		//Multiply4(quat, 1.0f / (float)actionCount);
		float angle = QuatExp(quat, quat);
		if(MagnitudeSq(quat + O_QX) > EPSILON){
			glRotatef(RadToDeg(angle), quat[O_QX], quat[O_QY], quat[O_QZ]);
			QuatMultiplyBy(curQuat, quat);
		}
	}

	//y-axis points in the direction along the bone away from the parent (in Blender)
	glTranslatef(0.0f, m_offset, 0.0f);

	if(m_mesh){
		glPushMatrix();
		//set the meshes's default rotation
		if(m_meshRot[O_ANG] != 0.0f){
			glRotatef(m_meshRot[O_ANG], m_meshRot[O_AX], m_meshRot[O_AY], m_meshRot[O_AZ]);
		}
		m_mesh->Render();
		glPopMatrix();
	}

	//compute start/end positions
	Set(m_start, curPos);
	Set(curPos, 0.0f, m_offset, 0.0f);
	QuatApplyTo(curPos, curPos, curQuat);
	AddTo(curPos, m_start);
	Set(m_end, curPos);

	if(m_child){
		m_child->Render(model, curPos, curQuat);
	}
	glPopMatrix();
}

//Add a child to this SphBone.
//child - to add.
void SphBone::AddChild(SphBone* child){
	//try to add as direct child
	if(m_child == NULL){
		m_child = child;
		return;
	}

	//add as a sibling to some child
	SphBone* sibling = m_child;
	while(sibling->m_sibling != NULL){
		sibling = sibling->m_sibling;
	}
	sibling->m_sibling = child;
}

//return - bone's end in model's coordinates.
cfloat* SphBone::GetBoneEnd(){
	return m_end;
}

//return - bone's start in model's coordinates.
cfloat* SphBone::GetBoneStart(){
	return m_start;
}

//Free SphBone and all associated resources.
void SphBone::Free(){
	if(m_mesh != NULL){
		m_mesh->Free();
	}
	DELETE(m_mesh);
}
