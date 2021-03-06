#include "gl/freeglut.h"
#include "math3d.h"
#include <math.h>

#pragma comment(lib, "freeglut.lib")


// Draw a torus (doughnut), using the current 1D texture for light shading
void DrawTorus(M3DMatrix44f mTransform)
{
	GLfloat majorRadius = 0.35f;
	GLfloat minorRadius = 0.15f;
	GLint   numMajor = 40;
	GLint   numMinor = 20;
	M3DVector3f objectVertex;         // Vertex in object/eye space
	M3DVector3f transformedVertex;    // New Transformed vertex   
	double majorStep = 2.0f*M3D_PI / numMajor;
	double minorStep = 2.0f*M3D_PI / numMinor;
	int i, j;

	for (i=0; i<numMajor; ++i) 
	{
		double a0 = i * majorStep;
		double a1 = a0 + majorStep;
		GLfloat x0 = (GLfloat) cos(a0);
		GLfloat y0 = (GLfloat) sin(a0);
		GLfloat x1 = (GLfloat) cos(a1);
		GLfloat y1 = (GLfloat) sin(a1);

		glBegin(GL_TRIANGLE_STRIP);
		for (j=0; j<=numMinor; ++j) 
		{
			double b = j * minorStep;
			GLfloat c = (GLfloat) cos(b);
			GLfloat r = minorRadius * c + majorRadius;
			GLfloat z = minorRadius * (GLfloat) sin(b);

			// First point
			objectVertex[0] = x0*r;
			objectVertex[1] = y0*r;
			objectVertex[2] = z;
			m3dTransformVector3(transformedVertex, objectVertex, mTransform);
			glVertex3fv(transformedVertex);

			// Second point
			objectVertex[0] = x1*r;
			objectVertex[1] = y1*r;
			objectVertex[2] = z;
			m3dTransformVector3(transformedVertex, objectVertex, mTransform);
			glVertex3fv(transformedVertex);
		}
		glEnd();
	}
}
// Draw a torus (doughnut)  at z = fZVal... torus is in xy plane
void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor)
{
	M3DVector3f vNormal;
	double majorStep = 2.0f*M3D_PI / numMajor;
	double minorStep = 2.0f*M3D_PI / numMinor;
	int i, j;

	for (i=0; i<numMajor; ++i) 
	{
		double a0 = i * majorStep;
		double a1 = a0 + majorStep;
		GLfloat x0 = (GLfloat) cos(a0);
		GLfloat y0 = (GLfloat) sin(a0);
		GLfloat x1 = (GLfloat) cos(a1);
		GLfloat y1 = (GLfloat) sin(a1);

		glBegin(GL_TRIANGLE_STRIP);
		for (j=0; j<=numMinor; ++j) 
		{
			double b = j * minorStep;
			GLfloat c = (GLfloat) cos(b);
			GLfloat r = minorRadius * c + majorRadius;
			GLfloat z = minorRadius * (GLfloat) sin(b);

			// First point
			glTexCoord2f((float)(i)/(float)(numMajor), (float)(j)/(float)(numMinor));
			vNormal[0] = x0*c;
			vNormal[1] = y0*c;
			vNormal[2] = z/minorRadius;
			m3dNormalizeVector(vNormal);
			glNormal3fv(vNormal);
			glVertex3f(x0*r, y0*r, z);

			glTexCoord2f((float)(i+1)/(float)(numMajor), (float)(j)/(float)(numMinor));
			vNormal[0] = x1*c;
			vNormal[1] = y1*c;
			vNormal[2] = z/minorRadius;
			m3dNormalizeVector(vNormal);
			glNormal3fv(vNormal);
			glVertex3f(x1*r, y1*r, z);
		}
		glEnd();
	}
}

// Called to draw scene
void RenderScene(void)
{
	M3DMatrix44f   transformationMatrix;   // Storeage for rotation matrix
	static GLfloat yRot = 0.0f;         // Rotation angle for animation
	yRot += 0.5f;

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Build a rotation matrix
	m3dRotationMatrix44(transformationMatrix, m3dDegToRad(yRot), 0.0f, 1.0f, 0.0f);
	transformationMatrix[12] = 0.0f;
	transformationMatrix[13] = 0.0f;
	transformationMatrix[14] = -2.5f;
	
	//DrawTorus(transformationMatrix);

	glLoadMatrixf(transformationMatrix);
	gltDrawTorus(0.35f, 0.15f, 40, 20);

	// Do the buffer Swap
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	// Bluish background
	glClearColor(0.0f, 0.0f, .50f, 1.0f );

	// Draw everything as wire frame
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
{
	// Redraw the scene with new coordinates
	glutPostRedisplay();
	glutTimerFunc(33,TimerFunction, 1);
}



void ChangeSize(int w, int h)
{
	GLfloat fAspect;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the clipping volume
	gluPerspective(35.0f, fAspect, 1.0f, 50.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutCreateWindow("Manual Transformations Demo");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	SetupRC();
	glutTimerFunc(33, TimerFunction, 1);

	glutMainLoop();;

	return 0;
}