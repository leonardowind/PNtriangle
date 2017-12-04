#include <iostream>
#include <fstream>
#include<sstream>
#include <string>
#include <gl/glut.h>
#include "common.h"
#include <map>
using namespace std;

GLfloat nRange = 100.0f;//reshape��һ��ʼʱ�����ķ�Χ
extern GLfloat AngleX;//��ת�Ƕ�
extern GLfloat AngleY;
GLfloat Z = 1.0f;//���ų߶�
GLfloat coordinateX = 0.0f, coordinateY = 0.0f, coordinateZ = 0.0f;//ƽ������
extern int numLine;//�߶εĸ���
extern int LOOPNUM;//�߶εĸ���
extern GLfloat divs;
GLfloat vArr[vNum][3];//�����
GLfloat vnArr[vnNum][3];//�������
int fvArr[fNum][3];//����һ����ĵ�����
int fnArr[fNum][3];//����һ����ĵ�ķ�������
string s0;//����obj
GLfloat f1, f2, f3;//����obj
bool flag = true;//���-��ת֮ǰֻ����һ��
int va = 0, vb = 0, vc = 0;//ͳ�Ƶ㡢������ĸ���
int vf = 0;//vc�ĸ���
bool flagMap = true;//��ǵ㷨��ӳ���mapֻ����һ��
extern multimap<int, int> mapP;

void readfile(string addrstr)//����obj ����ʽ��f 1/1/1 2/2/2 3/3/3
{
	int i = 0, j = 0, ii = 0;

	ifstream infile(addrstr.c_str());
	string sline;//ÿһ��
	while (getline(infile, sline))
	{
		if (sline[0] == 'v' && sline[1] == ' ')//v
		{
			istringstream sin(sline);
			sin >> s0 >> f1 >> f2 >> f3;

			//vArr[i][0] = f1 + 1.6;//girl
			//vArr[i][1] = f2 - 3.5;
			//vArr[i][2] = f3 + 13.6;

			//vArr[i][0] = f1 - 5.;//knight
			//vArr[i][1] = f2 + 5.;
			//vArr[i][2] = f3 - 13.;

			//vArr[i][0] = f1 + 10;//knight-��
			//vArr[i][1] = f2 + 2.;
			//vArr[i][2] = f3 - 8.;

			vArr[i][0] = f1;//��׼
			vArr[i][1] = f2;
			vArr[i][2] = f3;

			//vArr[i][0] = f1;//Lara_Croft
			//vArr[i][1] = f2 - 40.;
			//vArr[i][2] = f3;

			++i;
		}
		else if (sline[0] == 'v' && sline[1] == 'n')//vn
		{
			istringstream sin(sline);
			sin >> s0 >> f1 >> f2 >> f3;
			vnArr[j][0] = f1;
			vnArr[j][1] = f2;
			vnArr[j][2] = f3;
			++j;
		}
		else if (sline[0] == 'f')//f
		{
			int a = 0;
			int k;
			istringstream sin(sline);
			sin >> s0;//ǰ׺f

			for (int t = 0; t < 3; t++)
			{
				sin >> s0;
				//cout << s0 << endl;

				a = 0;
				for (k = 0; s0[k] != '/'; k++)//��������
				{
					a = a * 10 + (s0[k] - '0');
				}
				fvArr[ii][t] = a;

				for (k = k + 1; s0[k] != '/'; k++)//��������
				{
				}

				a = 0;
				for (k = k + 1; s0[k]; k++)//��������
				{
					a = a * 10 + (s0[k] - '0');
				}
				fnArr[ii][t] = a;

			}
			++ii;

		}
	}
	va = i;
	vb = j;
	vc = ii;
	vf = vc;//��¼vc
	cout << va << endl << vb << endl << vc << endl;
}

void readfileTea(string addrstr)//����obj ����ʽ��f 1 2 3
{
	int i = 0, j = 0, ii = 0;

	ifstream infile(addrstr.c_str());
	string sline;//ÿһ��
	while (getline(infile, sline))
	{
		if (sline[0] == 'v' && sline[1] == ' ')//v
		{
			istringstream sin(sline);
			sin >> s0 >> f1 >> f2 >> f3;
			//vArr[i][0] = f1 + 1.;
			//vArr[i][1] = f2 - 3.5;
			//vArr[i][2] = f3 + 13.;
			vArr[i][0] = f1;
			vArr[i][1] = f2;
			vArr[i][2] = f3;
			++i;
		}
		else if (sline[0] == 'v' && sline[1] == 'n')//vn
		{
			istringstream sin(sline);
			sin >> s0 >> f1 >> f2 >> f3;
			vnArr[j][0] = f1;
			vnArr[j][1] = f2;
			vnArr[j][2] = f3;
			++j;
		}
		else if (sline[0] == 'f')//f
		{
			int a = 0;
			istringstream sin(sline);
			sin >> s0 >> f1 >> f2 >> f3;
			fvArr[ii][0] = f1;
			fvArr[ii][1] = f2;
			fvArr[ii][2] = f3;

			fnArr[ii][0] = f1;
			fnArr[ii][1] = f2;
			fnArr[ii][2] = f3;

			++ii;
		}
	}
	va = i;
	vb = j;
	vc = ii;
	vf = vc;//��¼vc
	cout << va << endl << vb << endl << vc << endl;

	return;

}

bool isLine(int a, int b, int c)
{
	if ((a - b)*(a - c)*(b - c) == 0)
		return true;
	else
		return false;
}

bool isRepeat(int fv, int fn)//��ͷ���ӳ���Ƿ��ظ�
{
	int k;
	multimap<int, int>::iterator m;
	m = mapP.find(fv);
	for (k = 0; k != mapP.count(fv); k++, m++)
	{
		if (m->second == fn)
			return true;
	}
	//cout << m->first << "--" << m->second << endl;
	return false;
}

void input1()//������һ����������
{
	cout << "input: LOOPNUM: " << LOOPNUM - 1 << endl;
	int a, b, c;
	for (int i = 0; i < vc; i++)
	{
		inputTriangle(0, vArr[fvArr[i][0] - 1][0], vArr[fvArr[i][0] - 1][1], vArr[fvArr[i][0] - 1][2]);
		inputNormals(0, vnArr[fnArr[i][0] - 1][0], vnArr[fnArr[i][0] - 1][1], vnArr[fnArr[i][0] - 1][2]);

		inputTriangle(1, vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1], vArr[fvArr[i][1] - 1][2]);
		inputNormals(1, vnArr[fnArr[i][1] - 1][0], vnArr[fnArr[i][1] - 1][1], vnArr[fnArr[i][1] - 1][2]);

		inputTriangle(2, vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1], vArr[fvArr[i][2] - 1][2]);
		inputNormals(2, vnArr[fnArr[i][2] - 1][0], vnArr[fnArr[i][2] - 1][1], vnArr[fnArr[i][2] - 1][2]);

		a = fvArr[i][0] - 1;
		b = fvArr[i][1] - 1;
		c = fvArr[i][2] - 1;

		if (isLine(a, b, c))//�ǡ����߶�
		{
			POINT_3D n1 = { vnArr[fnArr[i][0] - 1][0], vnArr[fnArr[i][0] - 1][1], vnArr[fnArr[i][0] - 1][2] };
			POINT_3D n2 = { vnArr[fnArr[i][1] - 1][0], vnArr[fnArr[i][1] - 1][1], vnArr[fnArr[i][1] - 1][2] };
			POINT_3D n3 = { vnArr[fnArr[i][2] - 1][0], vnArr[fnArr[i][2] - 1][1], vnArr[fnArr[i][2] - 1][2] };

			POINT_3D b1 = { vArr[fvArr[i][0] - 1][0], vArr[fvArr[i][0] - 1][1], vArr[fvArr[i][0] - 1][2] };
			POINT_3D b2 = { vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1], vArr[fvArr[i][1] - 1][2] };
			POINT_3D b3 = { vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1], vArr[fvArr[i][2] - 1][2] };
			lineGen(numLine, n1, n2, n3, b1, b2, b3);
			++numLine;
		}
		else//���ǡ����ѷ��
		{
			controlPointGen();//generate control points
			PNpointGen(i - numLine);

			//halveTriangle();//���ȷֱ���������
			//halveTriangleShow();

			normalGen();
			PNnormalGen(i - numLine);

		}

	}

}

void initMultimap()//��ʼ��-ÿ�����м�������
{
	flagMap = false;
	int a, b, c;
	for (int i = 0; i < vc; i++)
	{
		inputTriangle(0, vArr[fvArr[i][0] - 1][0], vArr[fvArr[i][0] - 1][1], vArr[fvArr[i][0] - 1][2]);
		inputNormals(0, vnArr[fnArr[i][0] - 1][0], vnArr[fnArr[i][0] - 1][1], vnArr[fnArr[i][0] - 1][2]);

		inputTriangle(1, vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1], vArr[fvArr[i][1] - 1][2]);
		inputNormals(1, vnArr[fnArr[i][1] - 1][0], vnArr[fnArr[i][1] - 1][1], vnArr[fnArr[i][1] - 1][2]);

		inputTriangle(2, vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1], vArr[fvArr[i][2] - 1][2]);
		inputNormals(2, vnArr[fnArr[i][2] - 1][0], vnArr[fnArr[i][2] - 1][1], vnArr[fnArr[i][2] - 1][2]);

		a = fvArr[i][0] - 1;
		b = fvArr[i][1] - 1;
		c = fvArr[i][2] - 1;

		if (isLine(a, b, c))//�ǡ����߶�
		{
		}
		else//���ǡ����ѷ��
		{
			//��ͷ��� �� ����map
			for (int p = 0; p < 3; p++)
			{
				//cout << i << endl;
				if (!isRepeat(fvArr[i][p] - 1, fnArr[i][p] - 1))
					mapP.insert(pair<int, int>(fvArr[i][p] - 1, fnArr[i][p] - 1));//���ظ��������
			}
		}
	}

}

void input()//һ����������
{
	cout << "input: LOOPNUM: " << LOOPNUM - 1 << endl;
	int a, b, c;
	for (int i = 0; i < vc; i++)
	{
		inputTriangle(0, vArr[fvArr[i][0] - 1][0], vArr[fvArr[i][0] - 1][1], vArr[fvArr[i][0] - 1][2]);
		inputNormals(0, vnArr[fnArr[i][0] - 1][0], vnArr[fnArr[i][0] - 1][1], vnArr[fnArr[i][0] - 1][2]);

		inputTriangle(1, vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1], vArr[fvArr[i][1] - 1][2]);
		inputNormals(1, vnArr[fnArr[i][1] - 1][0], vnArr[fnArr[i][1] - 1][1], vnArr[fnArr[i][1] - 1][2]);

		inputTriangle(2, vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1], vArr[fvArr[i][2] - 1][2]);
		inputNormals(2, vnArr[fnArr[i][2] - 1][0], vnArr[fnArr[i][2] - 1][1], vnArr[fnArr[i][2] - 1][2]);

		a = fvArr[i][0] - 1;
		b = fvArr[i][1] - 1;
		c = fvArr[i][2] - 1;

		

		if (isLine(a, b, c))//�ǡ����߶�
		{
			POINT_3D n1 = { vnArr[fnArr[i][0] - 1][0], vnArr[fnArr[i][0] - 1][1], vnArr[fnArr[i][0] - 1][2] };
			POINT_3D n2 = { vnArr[fnArr[i][1] - 1][0], vnArr[fnArr[i][1] - 1][1], vnArr[fnArr[i][1] - 1][2] };
			POINT_3D n3 = { vnArr[fnArr[i][2] - 1][0], vnArr[fnArr[i][2] - 1][1], vnArr[fnArr[i][2] - 1][2] };

			POINT_3D b1 = { vArr[fvArr[i][0] - 1][0], vArr[fvArr[i][0] - 1][1], vArr[fvArr[i][0] - 1][2] };
			POINT_3D b2 = { vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1], vArr[fvArr[i][1] - 1][2] };
			POINT_3D b3 = { vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1], vArr[fvArr[i][2] - 1][2] };
			lineGen(numLine, n1, n2, n3, b1, b2, b3);
			++numLine;
		}
		else//���ǡ����ѷ��
		{
			for (int k = 0; k < 3; k++)
			{
				if (mapP.count(fvArr[i][k] - 1) >= 2)//1����������������
				{					
					//cout << "aaa" << endl;
					sharpEdgeControlPointGen(fvArr[i][k] - 1, k);
				}
				else
				{
					smoothEdgeControlPointGen(fvArr[i][k] - 1, k);
				}
				//cout << i << endl;

			}
			controlPointGen2();

			//for (auto &w : mapP)
			//	cout << w.first << "->" << w.second << endl;

			//controlPointGen();//generate control points
			PNpointGen(i - numLine);

			//halveTriangle();//���ȷֱ���������
			//halveTriangleShow();

			normalGen();
			PNnormalGen(i - numLine);

		}

	}

}

void showPN()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	{
		//cout << vc << endl << numLine << endl;
		//glTranslatef(0.0f, 0.0f, Z);
		glScalef(Z, Z, Z);//����
		glTranslatef(coordinateX, coordinateY, coordinateZ);
		//cout << "xyz: " << coordinateX << ',' << coordinateY << ',' << coordinateZ << endl;
		//glTranslatef(0.0f, 0.0f, Z);
		glRotatef(AngleX, 0.01f, 0.0f, 0.0f);
		glRotatef(AngleY, 0.0f, 0.01f, 0.0f);

		//if (flag == true)
		{
			input();
			flag = false;
		}

		cout << "fNum: " << vc << endl;
		for (int i = 0; i < (vc - numLine); i++)//fNum
		{
			//controlPointShow();
			//normalShow();
			drawPN(i);
		}

		for (int i = 0; i < numLine; i++)
		{
			//controlPointShow();
			//normalShow();
			drawLine(i);
		}
		numLine = 0;
		//mydeBug();

	}
	glPopMatrix();
	glutSwapBuffers();

}

void reshape(int w, int h)
{
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	//nRange = 100.f;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //����ǰ����ָ��ΪͶӰģʽ
	glLoadIdentity();
	//������άͶӰ��
	if (w <= h)
	{
		glOrtho(-nRange, nRange, -nRange * aspect, nRange * aspect, -nRange, nRange);
	}
	else
	{
		glOrtho(-nRange, nRange, -nRange / aspect, nRange / aspect, -nRange, nRange);
	}
}

void init()
{
	AngleX = 0.0f;
	AngleY = 0.0f;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DITHER);
	glShadeModel(GL_SMOOTH);
}

void keyfunc(unsigned char ch, int x, int y)//wasd�������ң�q�Ŵ�e��С��1����������Ƭ��2����������Ƭ
{
	switch (ch)
	{
	case 'w':
		coordinateY += nRange / 20; break;
	case 's':
		coordinateY -= nRange / 20; break;
	case 'a':
		coordinateX -= nRange / 20; break;
	case 'd':
		coordinateX += nRange / 20; break;
	case 'q':
		Z += 0.2f;
		break;
	case 'e':
		Z -= 0.2f;
		break;
	case '1':
		vc += 1;
		if (vc > vf)
			vc = vf;
		flag = true;
		break;
	case '2':
		vc -= 1;
		if (vc < 1)
			vc = 1;
		flag = true;
		break;
	default:
		break;
	}
	glutPostRedisplay();

}

void processMouse(int button, int state, int x, int y)//���������ϸ�����ӣ��Ҽ�������ϸ������
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			LOOPNUM += 1;
		if (LOOPNUM >= LOOPNUM2)
			LOOPNUM = LOOPNUM2;

		divs = (GLfloat)LOOPNUM;
		flag = true;//LOD���Ӹı䣬���¼�����빫ʽ
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			LOOPNUM -= 1;

		if (LOOPNUM <= 0)
			LOOPNUM = 1;

		divs = (GLfloat)LOOPNUM;
		flag = true;//LOD���Ӹı䣬���¼�����빫ʽ
		break;
	default:
		break;
	}

	glutPostRedisplay();

}