#include"stdafx.h"
#include"SceneMgr.h"


SceneMgr::SceneMgr()
{
	srand((unsigned)time(NULL));

	object = new ObjectCC [MaxObject];
	int xx, yy;
	for (int i = 0; i < MaxObject; ++i)
	{
		xx = rand() % 600;
		yy = rand() % 600;
		
		object[i].Set_xy(xx, yy);
	}
};

SceneMgr::~SceneMgr()
{
	delete[] object;
}

void SceneMgr::Update()
{
	for (int i = 0; i < MaxObject; ++i)
	{
		object[i].Update();
	}
}

void SceneMgr::DrawObject()
{
	for (int i = 0; i < MaxObject; ++i)
	{
		object[i].DrawSence();
	}
}

void SceneMgr::MouseSet(int x, int y)
{
	for (int i = 0; i < MaxObject; ++i)
	{
		object[i].Set_xy(x, y);
	}
}