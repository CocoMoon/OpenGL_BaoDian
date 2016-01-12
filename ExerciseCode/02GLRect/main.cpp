#include "gl/freeglut.h"

#pragma comment(lib, "freeglut.lib")

// 绘制场景
void RenderScene()
{
	// 用当前清除颜色清除窗口
	glClear(GL_COLOR_BUFFER_BIT);

	// 设置绘图颜色
	glColor3f(1.0f, 0.0f, 0.0f);

	// 用当前颜色绘制一个填充矩形
	glRectf(-25.0f, 25.0f, 25.0f, -25.0f);

	// 刷新绘图命令
	glFlush();
}

// 设置渲染状态
void SetupRec()
{
	glClearColor(0.6f, 0.4f, 0.7f, 1.0f);
}

// 当窗口改变时由GLUT函数库调用
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;

	// 防止被0所除
	if (h == 0)
		h = 1;

	// 把视口设置为窗口的大小
	glViewport(0, 0, w, h);

	// 重置坐标系统
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 建立裁剪区域(左、右、底、顶、近、远）
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