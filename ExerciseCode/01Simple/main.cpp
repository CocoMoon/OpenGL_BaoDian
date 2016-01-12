#include "gl/freeglut.h"

#pragma comment(lib, "freeglut.lib")

// 绘制场景
void RenderScene()
{
	// 用当前清除颜色清除窗口
	glClear(GL_COLOR_BUFFER_BIT);

	// 刷新绘图命令
	glFlush();
}

// 设置渲染状态
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