#include "SphMath.h"
#include "SphOpenGL.h"
#include "SphRenderer.h"

//Render xyz-axis.
void RenderAxis(float axisSize){
	glLineWidth(5.0f);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDepthFunc(GL_ALWAYS);

	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(axisSize, 0.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, axisSize, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, axisSize);
		glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
}

//Render line.
void RenderLine(cfloat* point1, cfloat* point2){
	glLineWidth(5.0f);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDepthFunc(GL_ALWAYS);

	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3fv(point1);
		glVertex3fv(point2);
	glEnd();
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
}

//Render xyz-circles.
void RenderSphere(float radius){
	cfloat step = PI2 / 16.0f;
	glLineWidth(5.0f);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBegin(GL_LINE_LOOP);
		glColor3f(1.0f, 0.0f, 0.0f);
		for(float theta = 0.0f; theta < PI2; theta += step){
			glVertex3f(0.0f, radius * Cos(theta), radius * Sin(theta));
		}
	glEnd();
	glBegin(GL_LINE_LOOP);
		glColor3f(0.0f, 1.0f, 0.0f);
		for(float theta = 0.0f; theta < PI2; theta += step){
			glVertex3f(radius * Cos(theta), 0.0f, radius * Sin(theta));
		}
	glEnd();
	glBegin(GL_LINE_LOOP);
		glColor3f(0.0f, 0.0f, 1.0f);
		for(float theta = 0.0f; theta < PI2; theta += step){
			glVertex3f(radius * Cos(theta), radius * Sin(theta), 0.0f);
		}
	glEnd();
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
}

#include <stdio.h>

//Render indexed vertices.
void RenderIndexedVertices(int indexCount, cushort* m_indices, cfloat* m_vertices){
	glLineWidth(5.0f);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		for(int i = 0; i < indexCount; i += N_TRI){
			cfloat* v0 = m_vertices + m_indices[i + 0] * N_XYZ;
			cfloat* v1 = m_vertices + m_indices[i + 1] * N_XYZ;
			cfloat* v2 = m_vertices + m_indices[i + 2] * N_XYZ;
			glVertex3f(v0[O_X], v0[O_Y], v0[O_Z]); glVertex3f(v1[O_X], v1[O_Y], v1[O_Z]);
			glVertex3f(v1[O_X], v1[O_Y], v1[O_Z]); glVertex3f(v2[O_X], v2[O_Y], v2[O_Z]);
			glVertex3f(v2[O_X], v2[O_Y], v2[O_Z]); glVertex3f(v0[O_X], v0[O_Y], v0[O_Z]);
		}
	glEnd();
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
}

//color - of the line.
//(x1, y1), (x2, y2) - points defining the line.
void RenderGuiLine(cfloat* color, float x1, float y1, float x2, float y2, float lineWidth){
	glBindTexture(GL_TEXTURE_2D, 0);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
		glColor4fv(color);
		glVertex3f(x1, y1, 1.0f);
		glVertex3f(x2, y2, 1.0f);
	glEnd();
}

//Render a GUI image.
//texture - to use.
//leftXY, rightXY - point coordinates defining the rectangle.
void RenderGuiImage(uint texture, cfloat* leftXY, cfloat* rightXY){
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(leftXY[O_X],  rightXY[O_Y], 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(rightXY[O_X], rightXY[O_Y], 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(rightXY[O_X], leftXY[O_Y],  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(leftXY[O_X],  leftXY[O_Y],  1.0f);
	glEnd();
}

//Render a frame around GUI object.
//leftXY, rightXY - point coordinates defining the rectangle.
void RenderGuiFrame(cfloat* color, cfloat* leftXY, cfloat* rightXY, float lineWidth){
	glBindTexture(GL_TEXTURE_2D, 0);
	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
		glColor4fv(color);
		glVertex3f(leftXY[O_X],  leftXY[O_Y],  1.0f);
		glVertex3f(leftXY[O_X],  rightXY[O_Y], 1.0f);
		glVertex3f(rightXY[O_X], rightXY[O_Y], 1.0f);
		glVertex3f(rightXY[O_X], leftXY[O_Y],  1.0f);
	glEnd();
}
