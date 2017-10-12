/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "Object.h"

Renderer *g_Renderer = NULL;

ObjectCC call;

GLvoid Reshape(int w, int h);
void Timerfunction(int value);

void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	call.DrawSence();
	glutSwapBuffers();
}
void Update();
void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		call.Set_xy(x, y);
	}
	RenderScene();
	
}

void KeyInput(unsigned char key, int x, int y)
{
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	
	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseInput);
	glutTimerFunc(30, Timerfunction, 1);
	glutMainLoop();
	//delete g_Renderer;

    return 0;
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0.0, 500.0, 500.0, 0.0, -1.0, 1.0);
}

void Timerfunction(int value)
{
	Update();
	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(30, Timerfunction, 1); // 타이머함수 재 설정
}

void Update()
{
	call.Update();
}