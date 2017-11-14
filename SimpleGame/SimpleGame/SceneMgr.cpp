#include"stdafx.h"
#include"SceneMgr.h"


SceneMgr::SceneMgr(int width, int height)
{

	m_renderer = new Renderer(width, height);
	dd = m_renderer->CreatePngTexture("./Textures/Mario_Rumble.png");
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
	n_arrow = -1;
	for (int i = 0; i < MaxObject; ++i)
	{
		object[i] = NULL;
		characterTime[i] = 0;
	}
	for (int i = 0; i < MaxBullet; ++i)
	{
		buildingBullet[i] = NULL;
	}
	for (int i = 0; i < MaxArrow; ++i)
	{
		arrowObject[i] = NULL;
		arrowType[i] = -1;
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
		if (object[i])
		{
			object[i]->Update(nowTime - prevTime);
			characterTime[i] += nowTime - prevTime;
			if (characterTime[i] > 0.5)
			{
				characterTime[i] = 0;
				arrowObject[(++n_arrow) % MaxArrow] = new ObjectCC(object[i]->GetX(), object[i]->GetY(), OBJECT_ARROW);
				arrowType[n_arrow % MaxArrow] = i;
			}
		}
	}
	for (int i = 0; i < MaxArrow; ++i)
	{
		if (arrowObject[i])
		{
			arrowObject[i]->Update(nowTime - prevTime);
		}
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
		if(object[i]) object[i]->DrawSolidRect(m_renderer);
	}
	for (int i = 0; i < MaxArrow; ++i)
	{
		if (arrowObject[i]) arrowObject[i]->DrawSolidRect(m_renderer);
	}
	for (int i = 0; i < MaxBullet; ++i)
	{
		if (buildingBullet[i]) buildingBullet[i]->DrawSolidRect(m_renderer);
	}
	if (building) building->DrawTexturedRect(m_renderer,dd);
}

void SceneMgr::MouseSet(int x, int y)
{
	float nowTime = (float)timeGetTime() / 1000.0f;

	if (nowTime - mouseTime > 0.2)
	{
		now = (now + 1) % MaxObject;
		int ttnow = 0;
		while (ttnow!=MaxObject)
		{
			if (object[ttnow] == NULL)
			{
				object[ttnow] = new ObjectCC(x, y, OBJECT_CHARACTER);
				break;
			}
			++ttnow;
			//else if (object[now]) object[now]->Set_xy(x, y);
		}
		now = (now +ttnow)%MaxObject;

		mouseTime = nowTime;
	}
}

void SceneMgr::CollisionTest()
{

	for (int i = 0; i < MaxObject; ++i)
	{
		if (object[i]!=NULL && building!=NULL &&Collision(object[i]->GetX(), object[i]->GetY(), object[i]->GetSize(), building->GetX(), building->GetY(), building->GetSize()))
		{
			building->SetLife(-object[i]->GetLife());
			object[i]->SetLife(-object[i]->GetLife());
			printf("%d번 캐릭터 , 빌딩 명중 \n", i);
		}
		else
		{
			for (int j = 0; j < MaxBullet; ++j)
			{
				if (object[i] != NULL && building != NULL &&buildingBullet[j] != NULL &&Collision(object[i]->GetX(), object[i]->GetY(), object[i]->GetSize(), buildingBullet[j]->GetX(), buildingBullet[j]->GetY(), buildingBullet[j]->GetSize()))
				{
					building->SetLife(-buildingBullet[j]->GetLife());
					buildingBullet[j]->SetLife(-buildingBullet[j]->GetLife());
					printf("%d번 총알 , %d번 캐릭터에 명중 \n", j,i);
				}
			}
		}
	}
	for (int i = 0; i < MaxArrow; ++i)
	{
		if (arrowObject[i] != NULL && building != NULL &&Collision(arrowObject[i]->GetX(), arrowObject[i]->GetY(), arrowObject[i]->GetSize(), building->GetX(), building->GetY(), building->GetSize()))
		{
			building->SetLife(-arrowObject[i]->GetLife());
			arrowObject[i]->SetLife(-arrowObject[i]->GetLife());
			printf("%d번 Arrow , 빌딩에 명중 \n",i);
		}
		for (int j = 0; j < MaxObject; ++j)
		{
			if (arrowType[i] != j && arrowObject[i] != NULL && object[j] != NULL && Collision(arrowObject[i]->GetX(), arrowObject[i]->GetY(), arrowObject[i]->GetSize(), object[j]->GetX(), object[j]->GetY(), object[j]->GetSize()))
			{
				object[j]->SetLife(-arrowObject[i]->GetLife());
				arrowObject[i]->SetLife(-arrowObject[i]->GetLife());
				printf("%d번이 쏜 %d번 Arrow ,  %d번 캐릭터 명중\n", arrowType[i],i,j);
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
	for (int i = 0; i < MaxArrow; ++i)
	{
		if (arrowObject[i]!=NULL &&arrowObject[i]->GetLife()<=0)
		{
			delete arrowObject[i];
			arrowObject[i] = NULL;
			arrowType[i] = -1;
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