/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "SceneMgr.h"

Renderer *g_Renderer = NULL;

SceneMgr *SceMgr=NULL;

DWORD g_prevTime = 0;


void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	DWORD currTime = timeGetTime();

	DWORD elapsedTime = currTime - g_prevTime;
	g_prevTime = currTime;


	SceMgr->Update((float)elapsedTime /1000.0);
	SceMgr->DrawObject();
	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		SceMgr->MouseSet(x,y);
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
	glutInitWindowSize(WIDTHSIMPLE, HEIGHTSIMPLE);
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

	SceMgr = new SceneMgr(WIDTHSIMPLE, HEIGHTSIMPLE);
	glutDisplayFunc(RenderScene);
	
	//glutReshapeFunc(Reshape);
	glutMouseFunc(MouseInput);
	glutIdleFunc(Idle);

	g_prevTime = timeGetTime();

	glutMainLoop();
	//delete g_Renderer;

    return 0;
}
