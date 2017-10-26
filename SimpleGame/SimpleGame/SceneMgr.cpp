#include"stdafx.h"
#include"SceneMgr.h"


SceneMgr::SceneMgr()
{
	srand((unsigned)time(NULL));
	prevTime = (float)timeGetTime() /1000.0f;

	
	float xx, yy;
	now = 1;

	for (int i = 0; i < MaxObject; ++i)
	{
		object[i] = new ObjectCC;
		xx = rand() % 600;
		yy = rand() % 600;
		
		object[i]->Set_xy(xx, yy);
	}
};

SceneMgr::~SceneMgr()
{
	delete[] object;
}

void SceneMgr::Update()
{
	float nowTime= (float)timeGetTime() / 1000.0f;

	for (int i = 0; i < MaxObject; ++i)
	{
		object[i]->Update(prevTime-nowTime);
	}
	CollisionTest();
	prevTime = nowTime;
	
	//delete object[1]
}

void SceneMgr::DrawObject()
{
	for (int i = 0; i < MaxObject; ++i)
	{
		object[i]->DrawSence();
	}
}

void SceneMgr::MouseSet(int x, int y)
{
	now = (now + 1) % MaxObject;
	
	object[now]->Set_xy(x, y);
}

void SceneMgr::CollisionTest()
{

	for (int i = 0; i < MaxObject; ++i)
	{
		bool check=false;
		for (int j = 0; j < MaxObject; ++j)
		{
			if (i != j && Collision(object[i]->GetX(), object[i]->GetY(), object[i]->GetSize(), object[j]->GetX(), object[j]->GetY(), object[j]->GetSize()))
			{
				check = true;
			}
		}
		if (check) object[i]->Set_RGB(1, 0, 0);
		else object[i]->Set_RGB(1, 1, 1);
	}
}


bool SceneMgr::Collision(float ix, float iy, float isize, float jx, float jy, float jsize )
{
	if (ix<jx + jsize && iy<jy + jsize && ix>jx && iy>jy) return true;
	if (ix+isize>jx && ix + isize<jx+jsize && iy<jy + jsize && iy>jy) return true;
	if (ix + isize>jx && ix + isize<jx + jsize && iy+isize<jy + jsize && iy + isize>jy) return true;
	if (ix>jx && ix<jx + jsize && iy + isize<jy + jsize && iy + isize>jy) return true;

	return false;
}