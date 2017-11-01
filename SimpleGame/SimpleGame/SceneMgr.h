#pragma once
#include"Object.h"
#include <windows.h>
#include <stdlib.h>       //srand
#include <time.h>     //time

#define MaxObject 10


class SceneMgr
{
private:
	ObjectCC *object[MaxObject];
	int now;
	float prevTime;
	float mouseTime;

public:
	SceneMgr();
	~SceneMgr();
	void Update();
	void DrawObject();
	void MouseSet(int x, int y);
	void CollisionTest();
	bool Collision(float ix, float iy, float isize, float jx, float jy, float jsize);
	void ObjectTimeover();
	void ObjectLifeover();
};

