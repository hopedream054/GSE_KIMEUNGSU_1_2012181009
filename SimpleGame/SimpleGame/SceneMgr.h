#pragma once
#include"Object.h"
#include"Renderer.h"
#include <windows.h>
#include <stdlib.h>       //srand
#include <time.h>     //time

#define MaxObject 10
#define MaxBullet 50
#define MaxArrow 100
#define OBJECT_BUILDING 1
#define OBJECT_CHARACTER 2
#define OBJECT_BULLET 3
#define OBJECT_ARROW 4

class SceneMgr
{
private:
	ObjectCC *object[MaxObject];
	ObjectCC *building;
	ObjectCC *buildingBullet[MaxBullet];
	ObjectCC *arrowObject[MaxArrow];
	int arrowType[MaxArrow];
	float characterTime[MaxObject];
	int n_building,n_bullet,n_arrow;
	int now;
	float prevTime;
	float mouseTime;
	float bulletTime;
	GLuint dd;
	Renderer *m_renderer;

public:
	SceneMgr(int width, int height);
	~SceneMgr();
	void Update();
	void DrawObject();
	void MouseSet(int x, int y);
	void CollisionTest();
	bool Collision(float ix, float iy, float isize, float jx, float jy, float jsize);
	void ObjectTimeover();
	void ObjectLifeover();
};

