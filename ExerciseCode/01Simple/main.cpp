#include "gl/freeglut.h"

#pragma comment(lib, "freeglut.lib")

// ���Ƴ���
void RenderScene()
{
	// �õ�ǰ�����ɫ�������
	glClear(GL_COLOR_BUFFER_BIT);

	// ˢ�»�ͼ����
	glFlush();
}

// ������Ⱦ״̬
void SetupRec()
{
	glClearColor(0.6f, 0.4f, 0.7f, 1.0f);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);

	SetupRec();

	glutMainLoop();

	return 0;
}