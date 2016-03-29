#include "gl/freeglut.h"
#include "math3d.h"
#include "glFrame.h"

// Light and material Data
GLfloat fLightPos[4]   = { -100.0f, 100.0f, 50.0f, 1.0f };  // Point source
GLfloat fLightPosMirror[4] = { -100.0f, -100.0f, 50.0f, 1.0f };
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

static GLfloat yRot = 0.0f;         // Rotation angle for animation 

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

///////////////////////////////////////////////////////////
// Draw the ground as a series of triangle strips. The 
// shading model and colors are set such that we end up 
// with a black and white checkerboard pattern.
void DrawGround(void)
{
	GLfloat fExtent = 20.0f;
	GLfloat fStep = 0.5f;
	GLfloat y = 0.0f;
	GLfloat fColor;
	GLfloat iStrip, iRun;
	GLint iBounce = 0;

	glShadeModel(GL_FLAT);
	for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
		{
			if((iBounce %2) == 0)
				fColor = 1.0f;
			else
				fColor = 0.0f;

			glColor4f(fColor, fColor, fColor, 0.5f);
			glVertex3f(iStrip, y, iRun);
			glVertex3f(iStrip + fStep, y, iRun);

			iBounce++;
		}
		glEnd();
	}
	glShadeModel(GL_SMOOTH);
}

///////////////////////////////////////////////////////////////////////
// Draw random inhabitants and the rotating torus/sphere duo
void DrawWorld(void)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(0.0f, 0.5f, -3.5f);

	glPushMatrix();
		glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(1.0f, 0.0f, 0.0f);
		glutSolidSphere(0.1f, 17, 9);
	glPopMatrix();


	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	gltDrawTorus(0.35, 0.15, 61, 37);

	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////        
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	// Move light under floor to light the "reflected" world
	glLightfv(GL_LIGHT0, GL_POSITION, fLightPosMirror);
	glPushMatrix();
	glFrontFace(GL_CW);             // geometry is mirrored, swap orientation

	glScalef(1.0f, -1.0f, 1.0f);
	DrawWorld();
	glFrontFace(GL_CCW);
	glPopMatrix();

	// Draw the ground transparently over the reflection
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawGround();
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);

	// Restore correct lighting and draw the world correctly
	glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
	DrawWorld();
	glPopMatrix();

	// Do the buffer Swap
	glutSwapBuffers();
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
	glTranslatef(0.0f, -0.4f, 0.0f);
}

///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
{
	yRot += 1.0f;   // Update Rotation

	// Redraw the scene
	glutPostRedisplay();

	// Reset timer
	glutTimerFunc(1,TimerFunction, 1);
}

//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	// Grayish background
	glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);

	// Cull backs of polygons
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Setup light parameters
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Mostly use material tracking
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL Blending and Transparenco");

	glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
	glutTimerFunc(10, TimerFunction, 1);

	SetupRC();
    glutMainLoop();

	return 0;
}
