#include <stdio.h>
#include <math.h>
#include <iostream>
#include <gl/glut.h>
using namespace std;

const int LOOPNUM2 = 50;//LOD���ֵ
const int NUM = LOOPNUM2 + 1;

//������һЩģ�͵ĵ㡢������ĸ����������֪��������ֵ����������һ���ϴ����ֵ��������ڴ�Ҳ��������
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

//const int vNum = 3084;//knight-��
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
} POINT_3D;// 3D���d�ṹ
void output3D(POINT_3D p);//���-3D��
POINT_3D pointValue(POINT_3D p, POINT_3D q);//��ֵ-3D��
POINT_3D pointAdd(POINT_3D p, POINT_3D q);//��-3D��
POINT_3D pointSub(POINT_3D p, POINT_3D q);//��-3D��
POINT_3D pointTimes(float c, POINT_3D p);//��-3D��
GLfloat dotProduct(POINT_3D p, POINT_3D q);//���-3D��
POINT_3D crossProduct(POINT_3D p, POINT_3D q); //���
GLfloat powDistance(POINT_3D n, POINT_3D p);//����-3D��
void inputTriangle(int i, GLfloat x, GLfloat y, GLfloat z);//����-����
void inputNormals(int i, GLfloat x, GLfloat y, GLfloat z);//����-���㷨����
POINT_3D wValue(int i, int j);//w���Ӽ���-������Ƶ�
POINT_3D func(int i, int j);//������Ƶ���㹫ʽ
void controlPointGen();//����-������Ƶ�
void controlPointGen2();//����-������Ƶ�-1��������2��������
void sharpEdgeControlPointGen(int n, int k);//2���������ıߣ����Ƶ�����
void smoothEdgeControlPointGen(int n, int k);//1���������ıߣ����Ƶ�����
void controlPointShow();//����-������Ƶ�
POINT_3D patchB(GLfloat u, GLfloat v);//������빫ʽ
void PNpointGen(int num);//�洢-ִ�ж�����빫ʽ��Ľ��
void drawPN(int num);//����-ִ�ж�����빫ʽ��Ľ��
POINT_3D myNormal(POINT_3D p);//��������λ��
GLfloat vValue(int i, int j);//v���Ӽ���-������Ƶ�
POINT_3D funcNormal(int i, int j);//������Ƶ���㹫ʽ
void normalGen();//����-������Ƶ�
POINT_3D patchNormal(GLfloat u, GLfloat v);//���������빫ʽ
void PNnormalGen(int num);//�洢-ִ�з��������빫ʽ��Ľ��
void lineGen(int num, POINT_3D n1, POINT_3D n2, POINT_3D n3, POINT_3D b1, POINT_3D b2, POINT_3D b3);
void drawLine(int numLine);//���ߣ�������������һ���߶�
void normalShow();//����-ִ�з��������빫ʽ��Ľ��
void mydeBug();//����-������
void display(void);
void reshape(int w, int h);
void key_board(GLint key, GLint x, GLint y);
void mouse(int button, int state, int x, int y);
void initLight(void);//���ճ�ʼ��
void keyfunc(unsigned char ch, int x, int y);
void halveTriangle();//���ȷ�������
void halveTriangleShow();//����-���ȷ�������
void readfile(string addrstr);//����obj ����ʽ��f 1/1/1 2/2/2 3/3/3
void readfileTea(string addrstr);//����obj ����ʽ��f 1 2 3
bool isLine(int a, int b, int c);//�ж�������������Ƿ���һ���߶�
bool isRepeat(int fv, int fn);//��ͷ���ӳ���Ƿ��ظ�
void input1();//������һ����������
void initMultimap();//��ʼ��-ÿ�����м�������
void input();//һ����������
void showPN();
void reshape(int w, int h);
void init();
void keyfunc(unsigned char ch, int x, int y);
void processMouse(int button, int state, int x, int y);