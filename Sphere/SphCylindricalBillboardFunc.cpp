#include "SphCylindricalBillboardFunc.h"
#include "SphMath.h"
#include "SphOpenGL.h"

SphCylindricalBillboardFunc g_cylinderBillboardFunc;

SphCylindricalBillboardFunc::SphCylindricalBillboardFunc(){
}

//This function does true billboarding using the camera's
// position relative to the object's position
void SphCylindricalBillboardFunc::Begin(cfloat* object_position)
{
	float lookAt[N_XYZ],objToCamProj[N_XYZ],upAux[N_XYZ];
	float /*modelview[16], */ angleCosine;

	glPushMatrix();

	// objToCamProj is the vector in world coordinates from the 
	// local origin to the camera projected in the XZ plane
	objToCamProj[O_X] = m_camPos[O_X] - object_position[O_X];
	objToCamProj[O_Y] = 0;
	objToCamProj[O_Z] = m_camPos[O_Z] - object_position[O_Z];

	// This is the original lookAt vector for the object 
	// in world coordinates
	lookAt[O_X] = 0;
	lookAt[O_Y] = 0;
	lookAt[O_Z] = 1;


	// normalize both vectors to get the cosine directly afterwards
	Normalize(objToCamProj);

	// easy fix to determine wether the angle is negative or positive
	// for positive angles upAux will be a vector pointing in the 
	// positive y direction, otherwise upAux will point downwards
	// effectively reversing the rotation.

	Cross(upAux, lookAt, objToCamProj);

	// compute the angle
	angleCosine = Dot(lookAt, objToCamProj);

	// perform the rotation. The if statement is used for stability reasons
	// if the lookAt and objToCamProj vectors are too close together then 
	// |angleCosine| could be bigger than 1 due to lack of precision
	//if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
	{
		glRotatef(RadToDeg(acos(angleCosine)), upAux[0], upAux[1], upAux[2]);
	}
}

//applies "cheating" (non-true) version of cylindrical billboards
// this version doesn't require the camera or object positions
void SphCylindricalBillboardFunc::CheatingBegin()
{
	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
	
	// The only difference now is that
	// the i variable will jump over the
	// up vector, 2nd column in OpenGL convention
	
	for( i=0; i<3; i+=2 )
	{
		for( j=0; j<3; j++ ) {
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}
	}
	
	// set the modelview matrix with the 
	// up vector unchanged
	glLoadMatrixf(modelview);
}

void SphCylindricalBillboardFunc::End()
{
	glPopMatrix();
}