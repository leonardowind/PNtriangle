#include <gl/glut.h>
#include "common.h"
extern bool flagMap;

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(480, 480);
	glutCreateWindow("PN Triangle");
	initLight();
	init();
	glutReshapeFunc(reshape);

	//readfile("girl.obj");
	//readfile("Knight.obj");
	//readfile("剑.obj"); 
	//readfile("venusm.obj"); 
	readfileTea("teapot.obj");
	//readfileTea("triangle.obj");
	//readfile("rabbit_cym.obj");
	//readfile("Lara_Croft.obj");
	//readfile("767.obj");
	//readfile("bishop.obj");
	//readfile("cube4.obj"); 
	//readfile("heart.obj");
	//readfile("diamond2.obj"); 
	//readfile("teapot2.obj");
	//readfile("car.obj");

	if (flagMap)
	{
		initMultimap();
	}
	glutDisplayFunc(showPN);
	glutKeyboardFunc(keyfunc);//键盘回调函数
	glutSpecialFunc(key_board); //设置处理特殊击键的回调函数
	glutMouseFunc(processMouse);//鼠标按键
	glutMainLoop();
}