#include "SphFountain.h"

ParticleContext_t SphFountain::P;

SphFountain::SphFountain(cchar* directory, cchar* filename)
{
    int particle_handle = P.GenParticleGroups(1, 500);
    P.CurrentGroup(particle_handle);

	char* filePath = ConcatNew(directory, filename);

	texture = g_openGL.LoadTexture(filePath);
}

// A fountain spraying up in the middle of the screen
void SphFountain::ComputeParticles()
{
	// Set up the state.
    P.Velocity(PDCylinder(pVec(0.0f, .001f, 0.0f), pVec(0.0f, 0.4f, 0.0f), 0.021f, 0.019f));
    P.Color(PDLine(pVec(0.8f, 0.9f, 1.0f), pVec(1.0f, 1.0f, 1.0f)));

    // Generate particles along a very small line in the nozzle.
    P.Source(400, PDLine(pVec(8.0f, 15.5f, 4.0f), pVec(0, .4f, 0)));

	// Gravity.
    P.Gravity(pVec(0, -0.01f, 0));

    // Bounce particles off a disc of radius 3.
    P.Bounce(-0.05f, 0.35f, 0, PDDisc(pVec(0, 0, 0), pVec(0, 1, 0), 3));

    // Kill particles below Z=-3.
    P.Sink(false, PDPlane(pVec(0,-3,0), pVec(0,1,0)));

    // Move particles to their new positions.
    P.Move(true, true);
	
}

// Draw as points using vertex arrays
// To draw as textured point sprites just call
// glEnable(GL_POINT_SPRITE_ARB) before calling this function.
// Pass in player position
void SphFountain::DrawGroupAsPoints(cfloat* pos)
{
	glEnable(GL_POINT_SPRITE_ARB);
	int cnt = P.GetGroupCount();
    float *ppos = new float[cnt * 3];
    int num_ret = P.GetParticles(0, cnt, ppos);

	glBindTexture(GL_TEXTURE_2D,texture);
  
        glPushMatrix();

		for(int n = 0; n < num_ret; n +=3){
		glBegin(GL_QUADS);
		  glTexCoord2f(0.0f,0.0f); glVertex3f(ppos[n] - 0.5f, ppos[n+1] - 0.5f, ppos[n+2] );
          glTexCoord2f(0.0f,1.0f); glVertex3f(ppos[n] - 0.5f, ppos[n+1] + 0.5f, ppos[n+2]);
          glTexCoord2f(1.0f,1.0f); glVertex3f(ppos[n] + 0.5f, ppos[n+1] + 0.5f, ppos[n+2]);
          glTexCoord2f(1.0f,0.0f); glVertex3f(ppos[n] + 0.5f, ppos[n+1] - 0.5f, ppos[n+2]);
		  }
         glEnd();

        glPopMatrix();

}