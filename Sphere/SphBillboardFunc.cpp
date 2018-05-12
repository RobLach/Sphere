#include "SphBillboardFunc.h"
#include "SphMath.h"
#include "SphOpenGL.h"
#include "SphGame.h"
#include "SphCamera.h"


/**
* All billboarding methods implemented using code from
*  the Lighthouse3D code samples at
* http://www.lighthouse3d.com/opengl/billboarding/
*
*/

// Begins billboard.
SphBillboardFunc::SphBillboardFunc(){
}

//This function does true billboarding using the camera's
// position relative to the object's position
void SphBillboardFunc::BeginCylindrical(cfloat* object_position)
{
	float lookAt[N_XYZ],objToCamProj[N_XYZ],upAux[N_XYZ];
	float camPos[N_XYZ]; Set(camPos, g_game.m_camera->GetPosition());
	float /*modelview[16], */ angleCosine;

	glPushMatrix();

	// objToCamProj is the vector in world coordinates from the 
	// local origin to the camera projected in the XZ plane
	objToCamProj[O_X] = camPos[O_X] - object_position[O_X];
	objToCamProj[O_Y] = 0;
	objToCamProj[O_Z] = camPos[O_Z] - object_position[O_Z];

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
void SphBillboardFunc::BeginCheatingCylindrical()
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


//This function does true spherical billboarding using the camera's
// position relative to the object's position
void SphBillboardFunc::BeginSpherical(cfloat* object_position)
{
	float lookAt[N_XYZ], objToCamProj[N_XYZ], upAux[N_XYZ], objToCam[N_XYZ];
	float camPos[N_XYZ]; Set(camPos, g_game.m_camera->GetPosition());
	float /*modelview[16], */angleCosine;

	glPushMatrix();

	// objToCamProj is the vector in world coordinates from the 
	// local origin to the camera projected in the XZ plane
	objToCamProj[O_X] = camPos[O_X] - object_position[O_X];
	objToCamProj[O_Y] = 0;
	objToCamProj[O_Z] = camPos[O_Z] - object_position[O_Z];

	// This is the original lookAt vector for the object 
	// in world coordinates
	Set(lookAt, 0.0f, 0.0f, 1.0f);


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
		  glRotatef(RadToDeg(Acos(angleCosine)),upAux[0], upAux[1], upAux[2]);	
      
	// so far it is just like the cylindrical billboard. The code for the 
	// second rotation comes now
	// The second part tilts the object so that it faces the camera
	
	// objToCam is the vector in world coordinates from 
	// the local origin to the camera
		objToCam[O_X] = camPos[O_X] - object_position[O_X];
		objToCam[O_Y] = camPos[O_Y] - object_position[O_Y];
		objToCam[O_Z] = camPos[O_Z] - object_position[O_Z];
	
	// Normalize to get the cosine afterwards
		Normalize(objToCam);
	
	// Compute the angle between objToCamProj and objToCam, 
	//i.e. compute the required angle for the lookup vector
	
		angleCosine = Dot(objToCamProj,objToCam);
	
	
	// Tilt the object. The test is done to prevent instability 
	// when objToCam and objToCamProj have a very small
	// angle between them
	
	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
	{
		if (objToCam[1] < 0)
			glRotatef(RadToDeg(acos(angleCosine)),1,0,0);	
		else
			glRotatef(RadToDeg(acos(angleCosine)),-1,0,0);
	}
}

//applies "cheating" (non-true) version of spherical billboards
// this version doesn't require the camera or object positions
void SphBillboardFunc::BeginCheatingSpherical()
{
	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
	{
		for( j=0; j<3; j++ ) {
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}
	}

	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);
}

// Sets the Up and Right vectors in parameter vectors.
void SphBillboardFunc::BillboardGetUpRightVector(float* up, float* right)
{
	float modelview[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	right[0] = modelview[0];
	right[1] = modelview[4];
	right[2] = modelview[8];

	up[0] = modelview[1];
	up[1] = modelview[5];
	up[2] = modelview[9];
}

// End billboarding. Returns to previous scene matrix.
void SphBillboardFunc::End()
{
	glPopMatrix();
}