#include "gl/freeglut.h"
#include "stdio.h"
#pragma comment(lib, "freeglut.lib")

// Initial square position and size
GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat rsize = 25;

// Step size in x and y directions
// (number of pixels to move each time)
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

// Keep track of windows changing width and height
GLfloat windowWidth;
GLfloat windowHeight;

// 绘制场景
void RenderScene()
{
	// 用当前清除颜色清除窗口
	glClear(GL_COLOR_BUFFER_BIT);

	// 设置绘图颜色
	glColor3f(1.0f, 0.0f, 0.0f);

	// 用当前颜色绘制一个填充矩形
	glRectf(x, y, x + rsize, y - rsize);

	// 刷新绘图命令
	glutSwapBuffers();
}

///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
{
	// Reverse direction when you reach left or right edge
	if(x > windowWidth-rsize || x < -windowWidth)
		xstep = -xstep;

	// Reverse direction when you reach top or bottom edge
	if(y > windowHeight || y < -windowHeight + rsize)
		ystep = -ystep;

	// Actually move the square
	x += xstep;
	y += ystep;

	// Check bounds. This is in case the window is made
	// smaller while the rectangle is bouncing and the 
	// rectangle suddenly finds itself outside the new
	// clipping volume
	if(x > (windowWidth-rsize + xstep))
		x = windowWidth-rsize-1;
	else if(x < -(windowWidth + xstep))
		x = -windowWidth -1;

	if(y > (windowHeight + ystep))
		y = windowHeight-1; 
	else if(y < -(windowHeight - rsize + ystep))
		y = -windowHeight + rsize - 1;

	// Redraw the scene with new coordinates
	glutPostRedisplay();
	glutTimerFunc(33,TimerFunction, 1);
}

// 设置渲染状态
void SetupRC()
{
	// Set clear color to blue
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

// 当窗口改变时由GLUT函数库调用
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;

	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h) 
	{
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho (-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
	}
	else 
	{
		windowWidth = 100 * aspectRatio;
		windowHeight = 100;
		glOrtho (-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutCreateWindow("Bounce");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunction, 1);

	SetupRC();

	const char* str = (const char*)glGetString(GL_EXTENSIONS);
	printf("%s", str);
	glutMainLoop();

	return 0;
}