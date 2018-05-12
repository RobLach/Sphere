#include "SphCylindricalBillboard.h"
#include "SphBillboardFunc.h"
#include "SphOpenGL.h"
#include "SphString.h"
#include "SphMath.h"

//Create SphObject.
SphCylindricalBillboard::SphCylindricalBillboard(){
	
}

//Renders the billboard
void SphCylindricalBillboard::Render()
{
	glPushMatrix();
	this->ApplyMatrix();

	SphBillboardFunc::BeginCylindrical(this->GetPosition());
	//SphBillboardFunc::BeginCheatingCylindrical(); //use the "non-true" version of billboarding
	
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