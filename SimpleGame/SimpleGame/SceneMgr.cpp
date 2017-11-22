#include"stdafx.h"
#include"SceneMgr.h"


SceneMgr::SceneMgr(int width, int height)
{

	m_renderer = new Renderer(width, height);
	
	m_texBlueBuilding = m_renderer->CreatePngTexture("./Textures/Mario_Rumble.png");
	m_texRedBuilding= m_renderer->CreatePngTexture("./Textures/simpsons_PNG88.png");
	if (!m_renderer->IsInitialized())
	{
		std::cout << "SceneMgr::Renderer could not be initialized.. \n";
	}

	srand((unsigned)time(NULL));
	prevTime = (float)timeGetTime() /1000.0f;
	mouseTime =7;
	now = 1;
	n_bullet = -1;
	n_arrow = -1;
	createRedTime = 0;
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
	building[0] = new ObjectCC(width/4*1, 100, OBJECT_BUILDING,REDTEAM);
	building[1] = new ObjectCC(width / 4*2, 100, OBJECT_BUILDING, REDTEAM);
	building[2] = new ObjectCC(width / 4*3, 100, OBJECT_BUILDING, REDTEAM);
	building[3] = new ObjectCC(width / 4*1, 700, OBJECT_BUILDING, BLUETEAM);
	building[4] = new ObjectCC(width / 4*2, 700, OBJECT_BUILDING, BLUETEAM);
	building[5] = new ObjectCC(width / 4*3, 700, OBJECT_BUILDING, BLUETEAM);

	for (int i = 0; i < MaxBuilding; ++i) bulletTime[i] = 10;
	
};

SceneMgr::~SceneMgr()
{
	delete[] object;
}

void SceneMgr::Update(float elapsedTime)
{
	float nowTime = (float)timeGetTime() / 1000.0f;

	createRedTime += elapsedTime;

	if (createRedTime > 5) //북쪽 캐릭터 생성 5초마다 생성
	{
		for (int i = 0; i < MaxObject; ++i)
		{
			if (object[i]==NULL)
			{
				object[i]= new ObjectCC(rand()% WIDTHSIMPLE, rand()% HEIGHTSIMPLE/2, OBJECT_CHARACTER, REDTEAM);
				break;
			}
		}
		createRedTime = 0;
	}
	for (int i = 0; i < MaxObject; ++i) //캐릭터별 애로우 생성
	{
		if (object[i])
		{
			object[i]->Update(elapsedTime);
			characterTime[i] += elapsedTime;
			if (characterTime[i] > 3) //3초마다 생성
			{
				characterTime[i] = 0;
				n_arrow = (n_arrow + 1) % MaxArrow;
				arrowObject[n_arrow] = new ObjectCC(object[i]->GetX(), object[i]->GetY()
					, OBJECT_ARROW, object[i]->GetTeamType());
				arrowType[n_arrow] = i;
			}
		}
	}

	for (int i = 0; i < MaxArrow; ++i)
	{
		if (arrowObject[i])
		{
			arrowObject[i]->Update(elapsedTime);
		}
	}
	for (int i = 0; i < MaxBullet; ++i)
	{
		if (buildingBullet[i]) buildingBullet[i]->Update(elapsedTime);
	}

	for (int i = 0; i < MaxBuilding; ++i) //빌딩의 총알
	{
		if (building[i])
		{
			building[i]->Update(elapsedTime);
			bulletTime[i] += elapsedTime;
			if (bulletTime[i] > 10) //10초마다 발사
			{

				buildingBullet[(++n_bullet) % MaxBullet] = new ObjectCC(building[i]->GetX(), building[i]->GetY()
					, OBJECT_BULLET, building[i]->GetTeamType());
				bulletTime[i] = 0;
			}
		}
	}

	CollisionTest();
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
	for (int i = 0; i < MaxBuilding; ++i)
	{
		if (building[i])
		{
			if(building[i]->GetTeamType()==REDTEAM) building[i]->DrawTexturedRect(m_renderer, m_texRedBuilding);
			else if (building[i]->GetTeamType() == BLUETEAM) building[i]->DrawTexturedRect(m_renderer, m_texBlueBuilding);
		}
	}
	
}

void SceneMgr::MouseSet(int x, int y)
{
	float nowTime = (float)timeGetTime() / 1000.0f;

	if (nowTime - mouseTime > 7.0)
	{
		now = (now + 1) % MaxObject;
		int ttnow = 0;
		while (ttnow!=MaxObject)
		{
			if (object[ttnow] == NULL)
			{
				if (y > HEIGHTSIMPLE / 2)
				{
					object[ttnow] = new ObjectCC(x, y, OBJECT_CHARACTER, BLUETEAM);
					break;
					//object[ttnow] = new ObjectCC(x, y, OBJECT_CHARACTER,REDTEAM);
					//break;

					
				}
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
	int check;
	for (int i = 0; i < MaxObject; ++i)
	{
		check = 1;
		for (int j = 0; j < MaxBuilding; ++j)
		{
			if (object[i] != NULL && building[j] != NULL && object[i]->GetTeamType() != building[j]->GetTeamType()
				&& Collision(object[i]->GetX(), object[i]->GetY(), object[i]->GetSize(), building[j]->GetX(), building[j]->GetY(), building[j]->GetSize()))
			{
				check = 0;
				building[j]->SetLife(-object[i]->GetLife());
				object[i]->SetLife(-object[i]->GetLife());
				printf("%d번 캐릭터 , 빌딩 명중 \n", i);
			}
		}
		
		if(check)
		{
			for (int j = 0; j < MaxBullet; ++j)
			{
				if ( object[i] != NULL && buildingBullet[j] != NULL && object[i]->GetTeamType() != buildingBullet[j]->GetTeamType() 
					&&Collision(object[i]->GetX(), object[i]->GetY(), object[i]->GetSize(), buildingBullet[j]->GetX(), buildingBullet[j]->GetY(), buildingBullet[j]->GetSize()))
				{
					object[i]->SetLife(-buildingBullet[j]->GetLife());
					buildingBullet[j]->SetLife(-buildingBullet[j]->GetLife());
					printf("%d번 총알 , %d번 캐릭터에 명중 \n", j,i);
				}
			}
		}
	}
	for (int i = 0; i < MaxArrow; ++i)
	{
		for (int j = 0; j < MaxBuilding; ++j)
		{
			if (arrowObject[i] != NULL && building[j] != NULL && arrowObject[i]->GetTeamType() != building[j]->GetTeamType()
				&&Collision(arrowObject[i]->GetX(), arrowObject[i]->GetY(), arrowObject[i]->GetSize(), building[j]->GetX(), building[j]->GetY(), building[j]->GetSize()))
			{
				building[j]->SetLife(-arrowObject[i]->GetLife());
				arrowObject[i]->SetLife(-arrowObject[i]->GetLife());
				printf("%d번 Arrow , 빌딩에 명중 \n", i);
			}
		}
		for (int j = 0; j < MaxObject; ++j)
		{
			if (arrowType[i] != j && arrowObject[i] != NULL && object[j] != NULL && arrowObject[i]->GetTeamType() != object[j]->GetTeamType()
				&& Collision(arrowObject[i]->GetX(), arrowObject[i]->GetY(), arrowObject[i]->GetSize(), object[j]->GetX(), object[j]->GetY(), object[j]->GetSize()))
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
	for (int i = 0; i < MaxBuilding; ++i)
	{
		if (building[i] != NULL && building[i]->GetLife() <= 0)
		{
			delete building;
			building[i] = NULL;
		}
	}
	
}