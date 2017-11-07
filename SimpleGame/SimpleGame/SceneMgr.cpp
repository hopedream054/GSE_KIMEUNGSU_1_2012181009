#include"stdafx.h"
#include"SceneMgr.h"


SceneMgr::SceneMgr(int width, int height)
{

	m_renderer = new Renderer(width, height);

	if (!m_renderer->IsInitialized())
	{
		std::cout << "SceneMgr::Renderer could not be initialized.. \n";
	}

	srand((unsigned)time(NULL));
	prevTime = (float)timeGetTime() /1000.0f;
	mouseTime = prevTime;
	bulletTime = prevTime;
	now = 1;
	n_bullet = -1;
	for (int i = 0; i < MaxObject; ++i)
	{
		object[i] = NULL;
	}
	for (int i = 0; i < MaxBullet; ++i)
	{
		buildingBullet[i] = NULL;
	}
	building = new ObjectCC(400, 400, OBJECT_BUILDING);
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
	for (int i = 0; i < MaxBullet; ++i)
	{
		if (buildingBullet[i]) buildingBullet[i]->Update(nowTime - prevTime);
	}
	if (building)
	{
		building->Update(nowTime - prevTime);
		if (nowTime - bulletTime > 0.5)
		{
			printf("12 \n");
			buildingBullet[(++n_bullet)%MaxBullet] = new ObjectCC(400, 400, OBJECT_BULLET);
			bulletTime = nowTime;
		}
	}

	CollisionTest();
	prevTime = nowTime;
	//ObjectTimeover();
	ObjectLifeover();
}

void SceneMgr::DrawObject()
{
	for (int i = 0; i < MaxObject; ++i)
	{
		if(object[i]) object[i]->DrawSence(m_renderer);
	}
	for (int i = 0; i < MaxBullet; ++i)
	{
		if (buildingBullet[i]) buildingBullet[i]->DrawSence(m_renderer);
	}
	if (building) building->DrawSence(m_renderer);
}

void SceneMgr::MouseSet(int x, int y)
{
	float nowTime = (float)timeGetTime() / 1000.0f;

	if (nowTime - mouseTime > 0.2)
	{
		now = (now + 1) % MaxObject;

		if (object[now] == NULL) object[now] = new ObjectCC (x, y, OBJECT_CHARACTER);
		else if (object[now]) object[now]->Set_xy(x, y);

		mouseTime = nowTime;
	}
}

void SceneMgr::CollisionTest()
{

	for (int i = 0; i < MaxObject; ++i)
	{
		bool check=false;
		if (object[i]!=NULL && building!=NULL &&Collision(object[i]->GetX(), object[i]->GetY(), object[i]->GetSize(), building->GetX(), building->GetY(), building->GetSize()))
		{
			building->SetLife(-object[i]->GetLife());
			object[i]->SetLife(-object[i]->GetLife());
		}
		else
		{
			for (int j = 0; j < MaxBullet; ++j)
			{
				if (object[i] != NULL && building != NULL &&buildingBullet[j] != NULL &&Collision(object[i]->GetX(), object[i]->GetY(), object[i]->GetSize(), buildingBullet[j]->GetX(), buildingBullet[j]->GetY(), buildingBullet[j]->GetSize()))
				{
					building->SetLife(-buildingBullet[j]->GetLife());
					buildingBullet[j]->SetLife(-buildingBullet[j]->GetLife());
				}
			}
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
		if (object[i] != NULL && object[i]->GetLife() <= 0)
		{
			delete object[i];
			object[i] = NULL;
		}
	}
	for (int i = 0; i < MaxBullet; ++i)
	{
		if (buildingBullet[i] != NULL && buildingBullet[i]->GetLife() <= 0)
		{
			delete buildingBullet[i];
			buildingBullet[i] = NULL;
		}
	}
	if (building!=NULL && building->GetLife() <= 0)
	{
		delete building;
		building = NULL;
	}
}