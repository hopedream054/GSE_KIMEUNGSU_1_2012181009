#include"stdafx.h"
#include"SceneMgr.h"


SceneMgr::SceneMgr()
{
	srand((unsigned)time(NULL));
	prevTime = (float)timeGetTime() /1000.0f;
	mouseTime = prevTime;

	now = 1;

	for (int i = 0; i < MaxObject; ++i)
	{
		object[i] = NULL;
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
		if (object[i]) object[i]->Update(nowTime- prevTime);
	}
	CollisionTest();
	prevTime = nowTime;
	ObjectTimeover();
	ObjectLifeover();
	//delete object[1]
}

void SceneMgr::DrawObject()
{
	for (int i = 0; i < MaxObject; ++i)
	{
		if(object[i]) object[i]->DrawSence();
	}
}

void SceneMgr::MouseSet(int x, int y)
{
	float nowTime = (float)timeGetTime() / 1000.0f;

	if (nowTime - mouseTime > 0.2)
	{
		now = (now + 1) % MaxObject;

		if (object[now] == NULL) object[now] = new ObjectCC(x, y, rand() % 50 + 20, rand() % 10 + 10);
		else if (object[now]) object[now]->Set_xy(x, y);

		mouseTime = nowTime;
	}
}

void SceneMgr::CollisionTest()
{

	for (int i = 0; i < MaxObject; ++i)
	{
		bool check=false;
		for (int j = 0; j < MaxObject; ++j)
		{
			if (object[i]!=NULL && object[j] != NULL && i != j && Collision(object[i]->GetX(), object[i]->GetY(), object[i]->GetSize(), object[j]->GetX(), object[j]->GetY(), object[j]->GetSize()))
			{
				check = true;
			}
		}
		if (object[i])
		{
			if (check) object[i]->Set_RGB(1, 0, 0);
			else object[i]->Set_RGB(1, 1, 1);
		}
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

void  SceneMgr::ObjectTimeover()
{
	for (int i = 0; i < MaxObject; ++i)
	{
		if (object[i] != NULL && object[i]->GetLifeTime() < 0)
		{
			delete object[i];
			object[i] = NULL;
		}
	}
}

void  SceneMgr::ObjectLifeover()
{
	for (int i = 0; i < MaxObject; ++i)
	{
		if (object[i] != NULL && object[i]->GetLife() < 0)
		{
			delete object[i];
			object[i] = NULL;
		}
	}
}