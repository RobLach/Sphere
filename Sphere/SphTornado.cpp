#include "SphTornado.h"


ParticleContext_t SphTornado::P;

/*
char* SphTornado::addAlphaChannel(Image* image, Image* alphaChannel) {
    char* pixels = new char[image->width * image->height * 4];
    for(int y = 0; y < image->height; y++) {
        for(int x = 0; x < image->width; x++) {
            for(int j = 0; j < 3; j++) {
                pixels[4 * (y * image->width + x) + j] =
                    image->pixels[3 * (y * image->width + x) + j];
            }
            pixels[4 * (y * image->width + x) + 3] =
                alphaChannel->pixels[3 * (y * image->width + x)];
        }
    }
    
    return pixels;
}
*/
SphTornado::SphTornado()
{
    int particle_handle = P.GenParticleGroups(1, 1000);
    P.CurrentGroup(particle_handle);
	 ppos = new float[1000 * N_XYZ];

	texture = g_openGL.LoadTexture("../Art/Particle_Textures/iceTornado1.tga");
}

// A Tornado
void SphTornado::ComputeParticles()
{

/*
        P.Vortex(
            pVec(10.0f,0.0f,2.5f), ///< tip of the vortex
            pVec(0.0f,.01f,0.0f), ///< the ray along the center of the vortex
            1.0f, ///< like a Phong exponent that gives a curve to the vortex silhouette; 1.0 is a cone; greater than 1.0 curves inward.
            3.0f, ///< defines the radius at the top of the vortex and the infinite cylinder of influence. No particle further than max_radius from the axis is affected.
            0.1f, ///< inward acceleration of particles OUTSIDE the vortex
            01.1f, ///< vertical acceleration of particles INSIDE the vortex. Can be negative to apply gravity.
            0.1f ///< acceleration around vortex of particles INSIDE the vortex.
            );
*/

	// Set up the state.
    P.Velocity(PDCone(pVec(0.0f, 0.01f, 0.0f), pVec(0.0f, 0.4f, 0.0f), 0.21f, 0.019f));
   // P.Color(PDLine(pVec(0.8f, 0.9f, 1.0f), pVec(1.0f, 1.0f, 1.0f)));

	// Change Rotational Velocities
	P.TargetRotVelocity(pVec(0.2f, 0.0f, 0.0f),25.0f);

    // Generate particles along a very small cone in the nozzle.
    P.Source(400, PDCone(pVec(0.0f, -0.04f, 0.0f), pVec(0.0f, -0.034f, 0.0f), 0.21f, 0.019f));

    // Gravity.
    P.Gravity(pVec(0.0f, 0.01f, 0.0f));

	// Avoid a sphere inside of the cone
	P.Avoid(2,4,3,PDSphere(pVec(0.0f, 1.5f, 0.0f), 0.15f, 0.0f));

    // Kill particles above 10
    P.Sink(true, PDPlane(pVec(0,10,0), pVec(0,12,0)));

    // Move particles to their new positions.
    P.Move(true, true);
	
}

// Draw
void SphTornado::DrawGroupAsPoints()
{	
	int cnt = P.GetGroupCount();
    int num_ret = P.GetParticles(0, cnt, ppos);
	
	float matrix[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

	float x0 = matrix[0] - matrix[1];
	float x1 = matrix[0] + matrix[1];

	float y0 = matrix[4] - matrix[5];
	float y1 = matrix[4] + matrix[5];

	float z0 = matrix[8] - matrix[9];
	float z1 = matrix[8] + matrix[9];

	int radius = 1;
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture( GL_TEXTURE_2D, texture );
	glDepthMask(false);

	glPushMatrix();
	glTranslatef(5.0f, 0.0f, 0.0f);//spawn position
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