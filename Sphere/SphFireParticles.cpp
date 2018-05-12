#include "SphFireParticles.h"
#include "SphGame.h"
#include "SphAvatar.h"
#include "SphMath.h"
#include "SphCamera.h"

ParticleContext_t SphFireParticles::P;

SphFireParticles::SphFireParticles(cchar* directory, cchar* filename, cfloat* position)
{
    int particle_handle = P.GenParticleGroups(1, 500);
    P.CurrentGroup(particle_handle);
	P.Seed(Randi(200));
	char* filePath = ConcatNew(directory, filename);
	
	texture = g_openGL.LoadTexture(filePath);
}

// A fountain spraying up in the middle of the screen
void SphFireParticles::ComputeParticles(cfloat* position, cfloat* direction)
{
	P.KillOld(5.0f, false);
	    // Generate particles along a very small line in the nozzle.
	P.Source(40, PDCylinder(pVec(position[O_X], position[O_Y], position[O_Z]),pVec(position[O_X], position[O_Y]+0.1f, position[O_Z]),2.0f,1.0f));
	
	// Set up the state.
    //P.Velocity(PDCylinder(pVec(0.0f, .001f, 0.0f), pVec(0.0f, 0.001f, 0.0f), 0.00f, 0.00f));
	P.Velocity(PDLine(pVec(0.0f,0.0f,0.0f),pVec(0.0f,0.0001f,0.0f)));
    //P.Color(PDLine(pVec(0.8f, 0.9f, 1.0f), pVec(1.0f, 1.0f, 1.0f)));
	P.RotVelocity(pVec(0.1f,10.0f, 0.1f));
	P.TargetColor(pVec(1.0f,1.0f,1.0f), 0.0f, 0.5f);
    // Gravity.
    P.Gravity(pVec(0, -0.1f, 0));
    // Bounce particles off a disc of radius 3.
    //P.Bounce(-0.05f, 0.35f, 0, PDDisc(pVec(0, 0, 0), pVec(0, 1, 0), 3));

    // Kill particles below Z=-3.
    //P.Sink(false, PDPlane(pVec(0,position[O_Y]-3,0), pVec(0,1,0)));
	
	//P.RandomDisplace(PDCylinder(pVec(0, 0, 0),pVec(0, 0.1f, 0),2.0f+Randf(),1.0f));
    // Move particles to their new positions.
    P.Move(true, true);

	
}

// Draw as points using vertex arrays
// To draw as textured point sprites just call
void SphFireParticles::Render()
{
	int cnt = P.GetGroupCount();
    float *ppos = new float[cnt * 3];
    int num_ret = P.GetParticles(0, cnt, ppos);

        glPushMatrix();
		glEnable(GL_BLEND);
		glAlphaFunc ( GL_GREATER,(GLclampf) 0.0001 ) ;
		glEnable ( GL_ALPHA_TEST ) ;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D,texture);		
		
		for(int n = 0; n < num_ret; n +=3){
			glPushMatrix();
			//glDepthMask(false);
			glTranslatef(ppos[n],ppos[n+1],ppos[n+2]);
			glRotatef(  
				atan2(g_game.m_camera->GetForward()[O_X],
                g_game.m_camera->GetForward()[O_Z]) * 57.29f, 
                0.0f, 1.0f, 0.0f);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); 
			glVertex3f(0 - 0.5f, 0 - 0.5f, 0);
			glTexCoord2f(0.0f,1.0f); 
			glVertex3f(0 - 0.5f, 0 + 0.5f, 0);
			glTexCoord2f(1.0f,1.0f); 
			glVertex3f(0 + 0.5f, 0 + 0.5f, 0);
			glTexCoord2f(1.0f,0.0f); 
			glVertex3f(0 + 0.5f, 0 - 0.5f, 0);
			glEnd();
			//glDepthMask(true);
			glPopMatrix();
			
		  }
		
		glDisable(GL_BLEND);
		glDisable ( GL_ALPHA_TEST ) ;
        glPopMatrix();
}