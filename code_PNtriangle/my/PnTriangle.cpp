#include <stdio.h>
#include <math.h>
#include <gl/glut.h>
#include <map>
#include <iostream>
#include "common.h"
using namespace std;

GLfloat AngleX;
GLfloat AngleY;
int numLine = 0;//线段的个数
int LOOPNUM = 1;
GLfloat divs = (GLfloat)LOOPNUM;//细分程度
multimap<int, int> mapP;//点到法线的映射
POINT_3D P[4], N[4];//input
POINT_3D  crossN[2];//1个顶点有2个法线N1，N2
POINT_3D B300, B030, B003, B210, B120, B021, B012, B102, B201, B111;//control point
POINT_3D b300, b030, b003, b210, b120, b021, b012, b102, b201, b111;//多法线时
POINT_3D E, V;
POINT_3D N200, N020, N002, N110, N011, N101, N111;//normal
POINT_3D N300, N030, N003, N210, N120, N021, N012, N102, N201, N131;//三次normal，目前效果不佳
POINT_3D drawB;//绘制点
POINT_3D PNpoint[fNum][NUM][NUM];//点
POINT_3D PNnormal[fNum][NUM][NUM];//法线
POINT_3D PNnormalLine[fNum][3];//如果三角形是一条线段的话，统一特殊处理
POINT_3D PNpointLine[fNum][3];
POINT_3D drawN;
extern GLfloat vnArr[vnNum][3];

void output3D(POINT_3D p)
{
	printf("(%f,%f,%f)\n", p.x, p.y, p.z);
}

POINT_3D pointValue(POINT_3D p, POINT_3D q) {
	p.x = q.x;  p.y = q.y;  p.z = q.z;
	return p;
}

POINT_3D pointAdd(POINT_3D p, POINT_3D q) {
	p.x += q.x;  p.y += q.y;  p.z += q.z;

	//-fym-//printf("pointAdd:\t");
	//-fym-//output3D(p);
	return p;
}

POINT_3D pointSub(POINT_3D p, POINT_3D q) {
	p.x -= q.x;  p.y -= q.y;  p.z -= q.z;

	//-fym-//printf("pointSub:\t");
	//-fym-//output3D(p);
	return p;
}

POINT_3D pointTimes(float c, POINT_3D p) {

	p.x *= c; p.y *= c; p.z *= c;
	//-fym-//printf("Ctimes:\t");
	//-fym-//output3D(p);
	return p;
}

GLfloat dotProduct(POINT_3D p, POINT_3D q) //点乘
{
	p.x *= q.x;  p.y *= q.y;  p.z *= q.z;
	//-fym-//printf("dotProduct:\t%f\n", p.x + p.y + p.z);
	return (p.x + p.y + p.z);
}

POINT_3D crossProduct(POINT_3D p, POINT_3D q) //叉乘
{
	POINT_3D n;
	n.x = p.y * q.z - p.z * q.y;
	n.y = p.z * q.x - p.x * q.z;
	n.z = p.x * q.y - p.y * q.x;
	return n;
}

GLfloat powDistance(POINT_3D n, POINT_3D p)//距离的平方的倒数
{
	GLfloat m = (pow((n.x - p.x), 2) + pow((n.y - p.y), 2) + pow((n.z - p.z), 2));
	//-fym-//printf("powDistance:\t%f\n", m);
	return 1 / m;
}

POINT_3D myNormal(POINT_3D p)//单位化,结果表明 自己单位化后并不能实现光照，必须开启自带的单位化//-fym-
{
	float sum = sqrt(pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2));
	//sum /= 30.0;//30times
	p.x /= sum;
	p.y /= sum;
	p.z /= sum;
	return p;
}

POINT_3D myNormal2(POINT_3D p, POINT_3D q)//单位化,结果表明 自己单位化后并不能实现光照，必须开启自带的单位化//-fym-
{
	p.x -= q.x;
	p.y -= q.y;
	p.z -= q.z;

	float sum = sqrt(pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2));
	//sum /= 30.0;//30times
	p.x /= sum;
	p.y /= sum;
	p.z /= sum;
	return p;
}

void inputTriangle(int i, GLfloat x, GLfloat y, GLfloat z)
{
	P[i + 1].x = x;
	P[i + 1].y = y;
	P[i + 1].z = z;

}

void inputNormals(int i, GLfloat x, GLfloat y, GLfloat z)//相对顶点的法向量
{
	N[i + 1].x = x;
	N[i + 1].y = y;
	N[i + 1].z = z;
	//output3D(myNormal2(N[i + 1], P[i + 1]));
}

POINT_3D wValue(int i, int j)//Wi
{
	return pointTimes(dotProduct(pointSub(P[j], P[i]), N[i]),N[i]);
}

POINT_3D func(int i, int j)
{
	//output3D(func(i, j));
	return pointTimes(0.3333333, pointSub(pointAdd(pointTimes(2.0, P[i]), P[j]), wValue(i, j)));
}

POINT_3D func2(int i, int j,int n)//n是点的索引号，k是P[k]
{
	int k;
	multimap<int, int>::iterator m;
	m = mapP.find(n);
	//for (k = 0; k != mapP.count(n); k++, m++)//此时 为 2
	for (k = 0; k != 2; k++, m++)//此时 为 2
	{
		crossN[k].x = vnArr[m->second][0];
		crossN[k].y = vnArr[m->second][1];
		crossN[k].z = vnArr[m->second][2];
	}
	//cout << m->first << "--" << m->second << endl;
	return pointAdd(P[i], pointTimes(dotProduct(pointSub(P[j], P[i]), crossProduct(crossN[0], crossN[1])) / 3, crossProduct(crossN[0], crossN[1])));
}

void controlPointGen()
{
	B300 = P[1];
	B030 = P[2];
	B003 = P[3];
	B210 = func(1, 2);
	B120 = func(2, 1);
	B021 = func(2, 3);
	B012 = func(3, 2);
	B102 = func(3, 1);
	B201 = func(1, 3);
	E = pointTimes(0.166667, pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(B210, B120), B021), B012), B102), B201));
	V = pointTimes(0.333333, pointAdd(pointAdd(P[1], P[2]), P[3]));
	B111 = pointAdd(E, pointTimes(0.5, pointSub(E, V)));

	//glPointSize(5.0f);
	//glBegin(GL_POINTS);
	//glColor3ub((GLubyte)255, (GLubyte)255, (GLubyte)255);//颜色设置为白色
	//glVertex3f(B210.x, B210.y, B210.z);
	//glVertex3f(B120.x, B120.y, B120.z);
	//glVertex3f(B021.x, B021.y, B021.z);
	//glVertex3f(B012.x, B012.y, B012.z);
	//glVertex3f(B102.x, B102.y, B102.z);
	//glVertex3f(B201.x, B201.y, B201.z);

	//glColor3d(1.0, 0.0, 1.0);//颜色设置为品红色
	//glVertex3f(B111.x, B111.y, B111.z);
	////glColor3ub(255u, 0u, 0u);//颜色设置为红色,重心
	////glVertex3f(V.x, V.y, V.z);
	//glEnd();
}

void controlPointGen2()//n是点的索引号，k是P[k]
{
	B300 = P[1];
	B030 = P[2];
	B003 = P[3];
	E = pointTimes(0.166667, pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(B210, B120), B021), B012), B102), B201));
	V = pointTimes(0.333333, pointAdd(pointAdd(P[1], P[2]), P[3]));
	B111 = pointAdd(E, pointTimes(0.5, pointSub(E, V)));
}

void sharpEdgeControlPointGen(int n, int k)//n是点的索引号，k是P[k]
{
	switch (k)
	{
	case 0:
		B210 = func2(1, 2, n);
		B201 = func2(1, 3, n);
		//cout << "B210 : " << B210.x << ',' << B210.y << ',' << B210.z<< endl;

		break;
	case 1:
		B120 = func2(2, 1, n);
		B021 = func2(2, 3, n);
		break;
	case 2:
		B102 = func2(3, 1, n);
		B012 = func2(3, 2, n);
		//cout <<n << "sharpB102 : " << B102.x << ',' << B102.y << ',' << B102.z << endl;
		break;
	}
}

void smoothEdgeControlPointGen(int n, int k)//n是点的索引号，k是P[k]
{
	switch (k)
	{
	case 0:
		B210 = func(1, 2);
		B201 = func(1, 3);
		break;
	case 1:
		B120 = func(2, 1);
		B021 = func(2, 3);
		break;
	case 2:
		B102 = func(3, 1);
		B012 = func(3, 2);
		//cout << n << "smoothB102 : " << B102.x << ',' << B102.y << ',' << B102.z << endl;

		break;
	}
}

void controlPointShow()
{
	//PN triangle
	glPointSize(5.0f);
	glColor3ub(255u, 255u, 0u);//颜色设置为黄色
	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES

	glVertex3f(B300.x, B300.y, B300.z);//1
	glVertex3f(B201.x, B201.y, B201.z);
	glVertex3f(B210.x, B210.y, B210.z);
	glEnd();


	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(B111.x, B111.y, B111.z);//2
	glVertex3f(B210.x, B210.y, B210.z);
	glVertex3f(B201.x, B201.y, B201.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(B111.x, B111.y, B111.z);//3
	glVertex3f(B210.x, B210.y, B210.z);
	glVertex3f(B120.x, B120.y, B120.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(B111.x, B111.y, B111.z);//4
	glVertex3f(B201.x, B201.y, B201.z);
	glVertex3f(B102.x, B102.y, B102.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(B120.x, B120.y, B120.z);
	glVertex3f(B030.x, B030.y, B030.z);
	glVertex3f(B021.x, B021.y, B021.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(B111.x, B111.y, B111.z);
	glVertex3f(B120.x, B120.y, B120.z);
	glVertex3f(B021.x, B021.y, B021.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(B111.x, B111.y, B111.z);
	glVertex3f(B012.x, B012.y, B012.z);
	glVertex3f(B021.x, B021.y, B021.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(B111.x, B111.y, B111.z);
	glVertex3f(B012.x, B012.y, B012.z);
	glVertex3f(B102.x, B102.y, B102.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(B003.x, B003.y, B003.z);
	glVertex3f(B012.x, B012.y, B012.z);
	glVertex3f(B102.x, B102.y, B102.z);

	glEnd();

}

POINT_3D patchB(GLfloat u, GLfloat v)//顶点插入公式
{
	GLfloat w = 1 - u - v;
	POINT_3D B = pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(
		pointTimes(pow(w, 3), B300), pointTimes(pow(u, 3), B030)),
		pointTimes(pow(v, 3), B003)),
		pointTimes(3 * u*pow(w, 2), B210)),
		pointTimes(3 * w*pow(u, 2), B120)),
		pointTimes(3 * v*pow(w, 2), B201)),
		pointTimes(3 * v*pow(u, 2), B021)),
		pointTimes(3 * w*pow(v, 2), B102)),
		pointTimes(3 * u*pow(v, 2), B012)),
		pointTimes(6 * u*v*w, B111));
	return B;
}

void PNpointGen(int num)
{
	int i, j;
	for (i = 0; i <= LOOPNUM; i++)
	{
		for (j = 0; j <= LOOPNUM; j++)
		{
			if (i + j > LOOPNUM)
				break;
			PNpoint[num][i][j] = patchB((GLfloat)i / divs, (GLfloat)j / divs);
		}
	}
}

void drawPN(int num)
{
	int i, j;
	glColor3f(0.0f, 0.0f, 1.0f);
	//glPointSize(5.0f);

	glBegin(GL_TRIANGLE_STRIP);//GL_POLYGON GL_QUADS  GL_LINE_LOOP GL_TRIANGLE_STRIP GL_POINTS

	for (i = 0; i <= LOOPNUM; i++)
	{
		for (j = 0; j <= LOOPNUM; j++)
		{
			drawB = PNpoint[num][i][j];
			drawN = PNnormal[num][i][j];
			glNormal3f(drawN.x, drawN.y, drawN.z);
			glVertex3f(drawB.x, drawB.y, drawB.z);

			if (i + j >= LOOPNUM)//-fym-//   单独剩下的最后一个点，跳出循环
				break;

			drawB = PNpoint[num][i + 1][j];
			drawN = PNnormal[num][i + 1][j];
			glNormal3f(drawN.x, drawN.y, drawN.z);
			glVertex3f(drawB.x, drawB.y, drawB.z);
		}
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);//GL_POLYGON GL_QUADS  GL_LINE_LOOP GL_TRIANGLE_STRIP
	}
	glEnd();
}

void halveTriangle()
{
	b300 = B300;
	b201 = pointTimes(0.5, pointAdd(B300, B201));
	b102 = pointTimes(0.25, pointAdd(B102, pointAdd(B300, pointTimes(2.0, B201))));
	b003 = pointTimes(0.125, pointAdd(pointAdd(pointAdd(B300, B003), pointTimes(3.0, B201)), pointTimes(3.0, B102)));
	b210 = B210;
	b111 = pointTimes(0.5, pointAdd(B210, B111));
	b012 = pointTimes(0.25, pointAdd(B012, pointAdd(B210, pointTimes(2.0, B111))));
	b120 = B120;
	b021 = pointTimes(0.5, pointAdd(B120, B021));
	b030 = B030;
}

void halveTriangleShow()
{
	//PN triangle
	glPointSize(5.0f);
	glColor3ub(255u, 0u, 0u);
	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES

	glVertex3f(b300.x, b300.y, b300.z);//1
	glVertex3f(b201.x, b201.y, b201.z);
	glVertex3f(b210.x, b210.y, b210.z);
	glEnd();


	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(b111.x, b111.y, b111.z);//2
	glVertex3f(b210.x, b210.y, b210.z);
	glVertex3f(b201.x, b201.y, b201.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(b111.x, b111.y, b111.z);//3
	glVertex3f(b210.x, b210.y, b210.z);
	glVertex3f(b120.x, b120.y, b120.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(b111.x, b111.y, b111.z);//4
	glVertex3f(b201.x, b201.y, b201.z);
	glVertex3f(b102.x, b102.y, b102.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(b120.x, b120.y, b120.z);
	glVertex3f(b030.x, b030.y, b030.z);
	glVertex3f(b021.x, b021.y, b021.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(b111.x, b111.y, b111.z);
	glVertex3f(b120.x, b120.y, b120.z);
	glVertex3f(b021.x, b021.y, b021.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(b111.x, b111.y, b111.z);
	glVertex3f(b012.x, b012.y, b012.z);
	glVertex3f(b021.x, b021.y, b021.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(b111.x, b111.y, b111.z);
	glVertex3f(b012.x, b012.y, b012.z);
	glVertex3f(b102.x, b102.y, b102.z);
	glEnd();

	glBegin(GL_LINE_LOOP);//GL_LINE_LOOP  GL_TRIANGLES
	glVertex3f(b003.x, b003.y, b003.z);
	glVertex3f(b012.x, b012.y, b012.z);
	glVertex3f(b102.x, b102.y, b102.z);

	glEnd();

}

GLfloat vValue(int i, int j)//Wi
{
	return 2.0 * dotProduct(pointSub(P[j], P[i]), pointAdd(N[i], N[j])) / dotProduct(pointSub(P[j], P[i]), pointSub(P[j], P[i]));
}

POINT_3D funcNormal(int i, int j)//计算法向量
{
	return pointSub(pointAdd(N[i], N[j]), pointTimes(vValue(i, j), pointSub(P[j], P[i])));//20160721
}

POINT_3D funcNormal2(int i, int j)//计算三次法向量
{
	return pointSub(pointTimes(0.3333333, pointAdd(pointTimes(2, N[i]), N[j])), pointTimes(vValue(i, j), pointSub(P[j], P[i])));//20160721
}

void normalGen()//生成6个法向控制点
{
	N200 = N[1];
	N020 = N[2];
	N002 = N[3];
	N110 = (funcNormal(1, 2));
	N011 = (funcNormal(2, 3));
	N101 = (funcNormal(3, 1));

	N111 = pointTimes(0.111111, pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(N200, N020), N002), N110), N110), N101), N011), N101), N011));


	//N200 = (pointSub(N[1], P[1]));
	//N020 = (pointSub(N[2], P[2]));
	//N002 = (pointSub(N[3], P[3]));
	//N110 = (funcNormal(1, 2));
	//N011 = (funcNormal(2, 3));
	//N101 = (funcNormal(3, 1));

	//N200 = myNormal(pointSub(N[1], P[1]));
	//N020 = myNormal(pointSub(N[2], P[2]));
	//N002 = myNormal(pointSub(N[3], P[3]));
	//N110 = myNormal(funcNormal(1, 2));
	//N011 = myNormal(funcNormal(2, 3));
	//N101 = myNormal(funcNormal(3, 1));
	//output3D(N101);

	//N200 = N[1];
	//N020 = N[2];
	//N002 = N[3];
	//N110 = pointAdd(funcNormal(1, 2), pointTimes(0.5, pointAdd(P[1], P[2])));
	//N011 = pointAdd(funcNormal(2, 3), pointTimes(0.5, pointAdd(P[2], P[3])));
	//N101 = pointAdd(funcNormal(3, 1), pointTimes(0.5, pointAdd(P[3], P[1])));

}

void normalGen2()//生成10个法向控制点
{
	N300 = N[1];
	N030 = N[2];
	N003 = N[3];
	N210 = funcNormal2(1, 2);
	N120 = funcNormal2(2, 1);
	N021 = funcNormal2(2, 3);
	N012 = funcNormal2(3, 2);
	N102 = funcNormal2(3, 1);
	N201 = funcNormal2(1, 3);
	E = pointTimes(0.166667, pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(N210, N120), N021), N012), N102), N201));
	V = pointTimes(0.333333, pointAdd(pointAdd(N[1], N[2]), N[3]));
	N111 = pointAdd(E, pointTimes(0.5, pointSub(E, V)));

}

void normalShow()
{
	//PN triangle  N200, N020, N002, N110, N011, N101
	glPointSize(5.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);//GL_LINE_LOOP  GL_TRIANGLES

	glVertex3f(N200.x, N200.y, N200.z);
	glVertex3f(N020.x, N020.y, N020.z);
	glVertex3f(N002.x, N002.y, N002.z);

	glVertex3f(N110.x, N110.y, N110.z);
	glVertex3f(N011.x, N011.y, N011.z);
	glVertex3f(N101.x, N101.y, N101.z);


	glEnd();

}

POINT_3D patchNormal(GLfloat u, GLfloat v)//法向量插入公式
{
	GLfloat w = 1 - u - v;
	POINT_3D N = pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(
		pointTimes(pow(w, 2), N200), pointTimes(pow(u, 2), N020)),
		pointTimes(pow(v, 2), N002)),
		pointTimes(w * u, N110)),
		pointTimes(u * v, N011)),
		pointTimes(w * v, N101));
	return N;
}

POINT_3D patchNormal22(GLfloat u, GLfloat v)//三次法向量插入公式，效果不如二次
{
	GLfloat w = 1 - u - v;
	POINT_3D N = pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(
		pointTimes(pow(w, 3), N200), pointTimes(pow(u, 3), N020)),
		pointTimes(pow(v, 3), N002)),
		pointTimes(3 * u*pow(w, 2), N110)),
		pointTimes(3 * w*pow(u, 2), N110)),
		pointTimes(3 * v*pow(w, 2), N101)),
		pointTimes(3 * v*pow(u, 2), N011)),
		pointTimes(3 * w*pow(v, 2), N101)),
		pointTimes(3 * u*pow(v, 2), N011)),
		pointTimes(6 * u*v*w, N111));
	return N;
}

POINT_3D patchNormal3(GLfloat u, GLfloat v)//三次
{
	GLfloat w = 1 - u - v;
	POINT_3D N = pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(pointAdd(
		pointTimes(pow(w, 3), N300), pointTimes(pow(u, 3), N030)),
		pointTimes(pow(v, 3), N003)),
		pointTimes(3 * u*pow(w, 2), N210)),
		pointTimes(3 * w*pow(u, 2), N120)),
		pointTimes(3 * v*pow(w, 2), N201)),
		pointTimes(3 * v*pow(u, 2), N021)),
		pointTimes(3 * w*pow(v, 2), N102)),
		pointTimes(3 * u*pow(v, 2), N012)),
		pointTimes(6 * u*v*w, N111));
	return N;
}

void PNnormalGen(int num)//法向量插入公式生成
{
	int i, j;
	for (i = 0; i <= LOOPNUM; i++)
	{
		for (j = 0; j <= LOOPNUM; j++)
		{
			if (i + j > LOOPNUM)
				break;
			PNnormal[num][i][j] = myNormal(patchNormal((GLfloat)i / divs, (GLfloat)j / divs));//单位化处理后
			//PNnormal[i][j] = (patchNormal((GLfloat)i / divs, (GLfloat)j / divs));//单位化处理后

		}
	}
}

void lineGen(int numLine, POINT_3D n1, POINT_3D n2, POINT_3D n3, POINT_3D b1, POINT_3D b2, POINT_3D b3)//存储-线段的控制点
{
	PNnormalLine[numLine][0] = n1;
	PNnormalLine[numLine][1] = n2;
	PNnormalLine[numLine][2] = n3;

	PNpointLine[numLine][0] = b1;
	PNpointLine[numLine][1] = b2;
	PNpointLine[numLine][2] = b3;

}

void drawLine(int numLine)
{
	int i;
	//glColor3f(0.0f, 0.0f, 1.0f);//颜色设置为绿色, P2
	//glPointSize(5.0f);

	glBegin(GL_LINE_LOOP);//GL_POLYGON GL_QUADS  GL_LINE_LOOP GL_TRIANGLE_STRIP GL_POINTS

	for (i = 0; i < 3; i++)
	{
		drawB = PNpointLine[numLine][i];
		drawN = PNnormalLine[numLine][i];
		glNormal3f(drawN.x, drawN.y, drawN.z);
		glVertex3f(drawB.x, drawB.y, drawB.z);
	}	
	glEnd();

}

void mydeBug()
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3ub((GLubyte)0, (GLubyte)255, (GLubyte)0);//颜色设置为绿色 ,P1
	glVertex3f(P[1].x, P[1].y, P[1].z);
	glColor3f(1.0f, 0.0f, 0.0f);//颜色设置为red色, P2
	glVertex3f(P[2].x, P[2].y, P[2].z);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	{
		glRotatef(AngleX, 1.0f, 0.0f, 0.0f);
		glRotatef(AngleY, 0.0f, 1.0f, 0.0f);

		//glEnable(GL_CULL_FACE);
		//Triangle();
		//normals();//input normals Ni

		//controlPointGen();//generate control points
		//PNpointGen();

		//normalGen();
		//PNnormalGen();

		//drawPN();


		//glPointSize(5.0f);
		//glBegin(GL_POINTS);
		//glColor3f(0.0f, 1.0f, 0.0f);//颜色设置为绿色, P2
		//PNpointGen();
		//drawB = PNpoint[0][5];
		//glVertex3f(drawB.x, drawB.y, drawB.z);
		///*drawB = PNpoint[0][1];
		//glVertex3f(drawB.x, drawB.y, drawB.z);
		//drawB = PNpoint[1][0];
		//glVertex3f(drawB.x, drawB.y, drawB.z);*/
		//glEnd();
	}
	glPopMatrix();
	glutSwapBuffers();
}

void key_board(GLint key, GLint x, GLint y)
{
	if (key == GLUT_KEY_UP)
	{
		AngleX -= 5.0f;

	}
	if (key == GLUT_KEY_DOWN)
	{
		AngleX += 5.0f;
	}
	if (key == GLUT_KEY_LEFT)
	{
		AngleY -= 5.0f;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		AngleY += 5.0f;
	}
	if (AngleX > 355.0f)
	{
		AngleX = 0.0f;
	}
	if (AngleX < 0.0f)
	{
		AngleX = 355.0f;
	}
	if (AngleY > 355.0f)
		AngleY = 0.0f;

	if (AngleY < 0.0f)
	{
		AngleY = 355.0f;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			
			glutPostRedisplay();
		}
		break;
	case GLUT_MIDDLE_BUTTON:
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void initLight(void)
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 1.0, 1.0 };


	glClearColor(0.0, 0.0, 0.0, 0.0);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);



	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}