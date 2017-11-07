#pragma once
#include"Object.h"
#include"Renderer.h"
#include <windows.h>
#include <stdlib.h>       //srand
#include <time.h>     //time

#define MaxObject 10
#define MaxBullet 50
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
	int n_building,n_bullet;
	int now;
	float prevTime;
	float mouseTime;
	float bulletTime;

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

