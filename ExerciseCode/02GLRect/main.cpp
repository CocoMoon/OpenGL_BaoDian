#include "gl/freeglut.h"

#pragma comment(lib, "freeglut.lib")

// ���Ƴ���
void RenderScene()
{
	// �õ�ǰ�����ɫ�������
	glClear(GL_COLOR_BUFFER_BIT);

	// ���û�ͼ��ɫ
	glColor3f(1.0f, 0.0f, 0.0f);

	// �õ�ǰ��ɫ����һ��������
	glRectf(-25.0f, 25.0f, 25.0f, -25.0f);

	// ˢ�»�ͼ����
	glFlush();
}

// ������Ⱦ״̬
void SetupRec()
{
	glClearColor(0.6f, 0.4f, 0.7f, 1.0f);
}

// �����ڸı�ʱ��GLUT���������
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;

	// ��ֹ��0����
	if (h == 0)
		h = 1;

	// ���ӿ�����Ϊ���ڵĴ�С
	glViewport(0, 0, w, h);

	// ��������ϵͳ
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// �����ü�����(���ҡ��ס���������Զ��
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h) 
		glOrtho (-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
	else 
		glOrtho (-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("GLRect");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	SetupRec();

	glutMainLoop();

	return 0;
}