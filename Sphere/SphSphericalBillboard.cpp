#include "SphSphericalBillboard.h"
#include "SphBillboardFunc.h"
#include "SphOpenGL.h"
#include "SphString.h"
#include "SphMath.h"

//Create SphObject.
SphSphericalBillboard::SphSphericalBillboard(){
	
}

//Renders the billboard
void SphSphericalBillboard::Render()
{
	glPushMatrix();
	this->ApplyMatrix();

	SphBillboardFunc::BeginSpherical(this->GetPosition());
	//SphBillboardFunc.BeginCheatingSpherical(); //use the "non-true" version of billboarding
	
	//APPLY SCALING HERE IF YOU'RE USING CHEATING VERSION OF BILLBOARDING
	//float scale[N_XYZ];
	//Set(scale, this->GetScale());
	//glScalef(scale[O_X], scale[O_Y], scale[O_Z]);

	//RenderAxis();
	//if(SphDebug && m_bound != NULL){
	//	m_bound->Render();
	//}

	this->RenderTexturedSquare();

	SphBillboardFunc::End();

	glPopMatrix();
}