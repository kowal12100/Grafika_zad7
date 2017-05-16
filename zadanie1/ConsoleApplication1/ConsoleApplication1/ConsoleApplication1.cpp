//Damian Moska�a

//Program 1 - Ostros�up prawid�owy o podstawie 6-k�ta z kul� wewn�trz i kwadratem u podstawy
#include "stdafx.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdlib.h>
#include "colors.h"

const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near_ = 3.0;
const GLdouble far_ = 7.0;

enum
{
	CUTTING_PLANE,
	POLYGON_OFFSET,
	EXIT
};

GLfloat angle = 0.0;
GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;
bool cutting_plane = true;
bool polygon_offset = true;
int button_state = GLUT_UP;
int button_x, button_y;
const int N = 9;
const int VertexNumber = N + 2;
const int TrianglesNumber = N * 2;
GLfloat vertex[VertexNumber * 3];
GLint triangles[TrianglesNumber * 3];

void GenerateVerticles(GLfloat* vertices, int Sides)
{
	for (auto i = 0; i < Sides; ++i)
	{
		vertices[i * 3 + 0] = sinf(i / static_cast<float>(Sides) * 2 * M_PI);
		vertices[i * 3 + 1] = -1.0f;
		vertices[i * 3 + 2] = cosf(i / static_cast<float>(Sides) * 2 * M_PI);
	}

	int l = Sides * 3;
	vertices[l + 0] = 0.f;
	vertices[l + 1] = -1.0f;
	vertices[l + 2] = 0.f;
	vertices[l + 3] = 0.f;
	vertices[l + 4] = 1.0f;
	vertices[l + 5] = 0.f;
}

void GenerateTriangles(GLint* triangles, int Sides)
{
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < Sides; ++j)
		{
			triangles[i*Sides * 3 + j * 3] = j;
			triangles[i*Sides * 3 + j * 3 + 1] = (j + 1) % Sides;
			triangles[i*Sides * 3 + j * 3 + 2] = Sides + i;
		}
	}
}

void Normal(GLfloat *n, int i)
{
	GLfloat v1[3], v2[3];
	v1[0] = vertex[3 * triangles[3 * i + 1] + 0] - vertex[3 * triangles[3 * i + 0] + 0];
	v1[1] = vertex[3 * triangles[3 * i + 1] + 1] - vertex[3 * triangles[3 * i + 0] + 1];
	v1[2] = vertex[3 * triangles[3 * i + 1] + 2] - vertex[3 * triangles[3 * i + 0] + 2];
	v2[0] = vertex[3 * triangles[3 * i + 2] + 0] - vertex[3 * triangles[3 * i + 1] + 0];
	v2[1] = vertex[3 * triangles[3 * i + 2] + 1] - vertex[3 * triangles[3 * i + 1] + 1];
	v2[2] = vertex[3 * triangles[3 * i + 2] + 2] - vertex[3 * triangles[3 * i + 1] + 2];
	n[0] = v1[1] * v2[2] - v1[2] * v2[1];
	n[1] = v1[2] * v2[0] - v1[0] * v2[2];
	n[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void SetColor(int i)
{
	i %= 5;

	switch (i)
	{
	case 0:
	{
		glColor3fv(Green);
	}
	break;

	case 1:
	{
		glColor3fv(Magenta);
	}
	break;

	case 2:
	{
		glColor3fv(Yellow);
	}
	break;

	case 3:
	{
		glColor3fv(Cyan);
	}
	break;

	case 4:
	{
		glColor3fv(OrangeRed);
	}
	break;

	default:
		break;
	}
}

void DrawPyramid()
{
	for (int i = 0; i < TrianglesNumber; i++)
	{
		GLfloat n[3];
		Normal(n, i);
		glNormal3fv(n);
		SetColor(i);
		glVertex3fv(&vertex[3 * triangles[3 * i + 0]]);
		SetColor(i + 1);
		glVertex3fv(&vertex[3 * triangles[3 * i + 1]]);
		SetColor(i + 2);
		glVertex3fv(&vertex[3 * triangles[3 * i + 2]]);
	}
}

void Display()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -(near_ + far_) / 2);
	glRotatef(rotatex, 1.0, 0, 0);
	glRotatef(rotatey, 0, 1.0, 0);
	glScalef(1.15, 1.15, 1.15);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	angle += 0.2;
	glRotatef(angle, 1.0, 1.0, 0.0);
	glColor3fv(Yellow);

	if (polygon_offset)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
	}

	glPolygonOffset(1.0, 1.0);
	glutSolidSphere(0.2, 10, 10);
	glColor3fv(Black);
	glutWireSphere(0.2, 10, 10);

	if (polygon_offset)
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	glPopMatrix();

	if (cutting_plane)
	{
		glDrawBuffer(GL_NONE);
		glBegin(GL_QUADS);
		glVertex3f(-0.5, -1.002f, -0.5);
		glVertex3f(0.5, -1.002f, -0.5);
		glVertex3f(0.5, -1.002f, 0.5);
		glVertex3f(-0.5, -1.002f, 0.5);
		glEnd();
		glDrawBuffer(GL_BACK);
	}

	glBegin(GL_TRIANGLES);
	DrawPyramid();
	glEnd();
	glFlush();
	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left, right, bottom, top, near_, far_);
	Display();
}

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
	{
		rotatey -= 1;
	}
	break;

	case GLUT_KEY_UP:
	{
		rotatex -= 1;
	}
	break;

	case GLUT_KEY_RIGHT:
	{
		rotatey += 1;
	}
	break;

	case GLUT_KEY_DOWN:
	{
		rotatex += 1;
	}
	break;
	}

	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		button_state = state;

		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}

void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		rotatey += 30 * (right - left) / glutGet(GLUT_WINDOW_WIDTH) *(x - button_x);
		button_x = x;
		rotatex -= 30 * (top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) *(button_y - y);
		button_y = y;
		glutPostRedisplay();
	}
}

void Menu(int value)
{
	switch (value)
	{
	case CUTTING_PLANE:
	{
		cutting_plane = !cutting_plane;
		Display();
	}
	break;

	case POLYGON_OFFSET:
	{
		polygon_offset = !polygon_offset;
		Display();
	}
	break;

	case EXIT:
		exit(0);
	}
}


int main(int argc, char * argv[])
{
	GenerateVerticles(vertex, N);
	GenerateTriangles(triangles, N);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Z-bufor");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(SpecialKeys);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutCreateMenu(Menu);
	glutCreateMenu(Menu);
	glutAddMenuEntry("P�aszczyzna przekroju: rysowana/nierysowana", CUTTING_PLANE);
	glutAddMenuEntry("Przesuni�cie warto�ci g��bi: w��cz/wy��cz", POLYGON_OFFSET);
	glutAddMenuEntry("Wyj�cie", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutIdleFunc(Display);
	glutMainLoop();
	return 0;
}
