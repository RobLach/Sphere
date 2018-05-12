#include "SphGroundPoof.h"
#include "SphOpenGL.h"
#include "SphBillboardFunc.h"

ParticleContext_t SphGroundPoof::P;


SphGroundPoof::SphGroundPoof()
{
    int particle_handle = P.GenParticleGroups(1, 500);
    P.CurrentGroup(particle_handle);
	ppos = new float[1000 * N_XYZ];

	texture = g_openGL.LoadTexture("../Art/Particle_Textures/snowPoof.tga");

}

void SphGroundPoof::ResetPuff(cfloat* pos)
{
    P.Velocity(PDCone(pVec(0.0f, 0.005f, 0.0f), pVec(0.0f, 0.006f, 0.0f), 0.5f, 0.49f));
    P.Source(400, PDCone(pVec(pos[0], pos[1]+.01f, pos[2]), pVec(pos[0], pos[1]+.04f, pos[2]), 0.021f, 0.019f));
}

// A Tornado
void SphGroundPoof::ComputeParticles()
{
    // Gravity.
    P.Gravity(pVec(0.0f, -0.015f, 0.0f));

	// Avoid a sphere inside of the cone
	P.Avoid(2,15,10,PDSphere(pVec(0.0f, 0.15f, 0.0f), 0.1f, 0.0f));

    // Kill particles below 
    P.Sink(false, PDPlane(pVec(0,-1.5,0), pVec(0,12,0)));

    // Move particles to their new positions.
    P.Move(true, true);
	
}

// Draw
void SphGroundPoof::DrawGroupAsPoints()
{	
	int cnt = P.GetGroupCount();
    int num_ret = P.GetParticles(0, cnt, ppos);
	
	float up[N_XYZ], right[N_XYZ];
	SphBillboardFunc::BillboardGetUpRightVector(up, right);

	float x0 = right[O_X] - up[O_X];
	float x1 = right[O_X] + up[O_X];

	float y0 = right[O_Y] - up[O_Y];
	float y1 = right[O_Y] + up[O_Y];

	float z0 = right[O_Z] - up[O_Z];
	float z1 = right[O_Z] + up[O_Z];

	int radius = 1;
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture( GL_TEXTURE_2D, texture );
	glDepthMask(false);

	glPushMatrix();
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		for(int n = 0; n < num_ret; n +=3){
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(ppos[n + O_X] - x1*radius, ppos[n + O_Y] - y1*radius, ppos[n + O_Z] - z1*radius);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(ppos[n + O_X] + x0*radius, ppos[n + O_Y] + y0*radius, ppos[n + O_Z] + z0*radius);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(ppos[n + O_X] + x1*radius, ppos[n + O_Y] + y1*radius, ppos[n + O_Z] + z1*radius);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(ppos[n + O_X] - x0*radius, ppos[n + O_Y] - y0*radius, ppos[n + O_Z] - z0*radius);
		}
	glEnd();
	glPopMatrix();

	glDepthMask(true);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}