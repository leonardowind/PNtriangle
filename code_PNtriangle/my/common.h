#include <stdio.h>
#include <math.h>
#include <iostream>
#include <gl/glut.h>
using namespace std;

const int LOOPNUM2 = 50;//LOD最大值
const int NUM = LOOPNUM2 + 1;

//下面是一些模型的点、法向、面的个数，如果不知道具体数值，可以设置一个较大的数值，申请的内存也就增加了
const int vNum = 530;//teapot
const int vnNum = 530;
const int fNum = 1024;

//const int vNum = 1332;//girl
//const int vnNum = 1363;
//const int fNum = 2407;

//const int vNum = 3;//triangle
//const int vnNum = 3;
//const int fNum = 1;

//const int vNum = 3084;//knight
//const int vnNum = 3367;
//const int fNum = 6043;

//const int vNum = 3084;//knight-剑
//const int vnNum = 3367;
//const int fNum = 82;

//const int vNum = 19755;//venusm
//const int vnNum = 22153;
//const int fNum = 43357;

//const int vNum = 20000;//hand
//const int vnNum = 30000;
//const int fNum = 3000;

//const int vNum = 4460;//Lara_Croft
//const int vnNum = 4460;
//const int fNum = 7138;

//const int vNum = 15130;//767
//const int vnNum = 15130;
//const int fNum = 16910;

//const int vNum = 25000;//bishop
//const int vnNum = 25000;
//const int fNum = 40960;

//const int vNum = 4336;//rabbit
//const int vnNum = 4336;
//const int fNum = 8400;

//const int vNum = 4360;//rabbit
//const int vnNum = 4306;
//const int fNum = 8400;

typedef struct point_3d {      
	float x, y, z;
} POINT_3D;// 3D点的d结构
void output3D(POINT_3D p);//输出-3D点
POINT_3D pointValue(POINT_3D p, POINT_3D q);//赋值-3D点
POINT_3D pointAdd(POINT_3D p, POINT_3D q);//加-3D点
POINT_3D pointSub(POINT_3D p, POINT_3D q);//减-3D点
POINT_3D pointTimes(float c, POINT_3D p);//乘-3D点
GLfloat dotProduct(POINT_3D p, POINT_3D q);//点乘-3D点
POINT_3D crossProduct(POINT_3D p, POINT_3D q); //叉乘
GLfloat powDistance(POINT_3D n, POINT_3D p);//距离-3D点
void inputTriangle(int i, GLfloat x, GLfloat y, GLfloat z);//输入-顶点
void inputNormals(int i, GLfloat x, GLfloat y, GLfloat z);//输入-顶点法向量
POINT_3D wValue(int i, int j);//w因子计算-顶点控制点
POINT_3D func(int i, int j);//顶点控制点计算公式
void controlPointGen();//生成-顶点控制点
void controlPointGen2();//生成-顶点控制点-1个顶点有2个法向量
void sharpEdgeControlPointGen(int n, int k);//2个法向量的边，控制点生成
void smoothEdgeControlPointGen(int n, int k);//1个法向量的边，控制点生成
void controlPointShow();//绘制-顶点控制点
POINT_3D patchB(GLfloat u, GLfloat v);//顶点插入公式
void PNpointGen(int num);//存储-执行顶点插入公式后的结果
void drawPN(int num);//绘制-执行顶点插入公式后的结果
POINT_3D myNormal(POINT_3D p);//法向量单位化
GLfloat vValue(int i, int j);//v因子计算-法向控制点
POINT_3D funcNormal(int i, int j);//法向控制点计算公式
void normalGen();//生成-法向控制点
POINT_3D patchNormal(GLfloat u, GLfloat v);//法向量插入公式
void PNnormalGen(int num);//存储-执行法向量插入公式后的结果
void lineGen(int num, POINT_3D n1, POINT_3D n2, POINT_3D n3, POINT_3D b1, POINT_3D b2, POINT_3D b3);
void drawLine(int numLine);//画线，输入三角形是一条线段
void normalShow();//绘制-执行法向量插入公式后的结果
void mydeBug();//绘制-辅助点
void display(void);
void reshape(int w, int h);
void key_board(GLint key, GLint x, GLint y);
void mouse(int button, int state, int x, int y);
void initLight(void);//光照初始化
void keyfunc(unsigned char ch, int x, int y);
void halveTriangle();//二等分三角形
void halveTriangleShow();//绘制-二等分三角形
void readfile(string addrstr);//导入obj ，格式：f 1/1/1 2/2/2 3/3/3
void readfileTea(string addrstr);//导入obj ，格式：f 1 2 3
bool isLine(int a, int b, int c);//判断输入的三角形是否是一条线段
bool isRepeat(int fv, int fn);//点和法向映射是否重复
void input1();//不考虑一个点多个法向
void initMultimap();//初始化-每个点有几个法向
void input();//一个点多个法向
void showPN();
void reshape(int w, int h);
void init();
void keyfunc(unsigned char ch, int x, int y);
void processMouse(int button, int state, int x, int y);