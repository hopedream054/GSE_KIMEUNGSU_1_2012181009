#pragma once
#include"Object.h"
#include <stdlib.h>       //srand
#include <time.h>     //time

#define MaxObject 50


class SceneMgr
{
private:
	ObjectCC *object;
public:
	SceneMgr();
	~SceneMgr();
	void Update();
	void DrawObject();
	void MouseSet(int x, int y);
};

