#include"stdafx.h"
#include"SceneMgr.h"


SceneMgr::SceneMgr(int width, int height)
{
	
	m_renderer = new Renderer(width, height);
	
	m_texBlueBuilding = m_renderer->CreatePngTexture("./Textures/building1.png");
	m_texRedBuilding= m_renderer->CreatePngTexture("./Textures/building2.png");
	m_texBackground= m_renderer->CreatePngTexture("./Textures/background.png");
	m_texSprite= m_renderer->CreatePngTexture("./Textures/eirp1.png");
	m_texSpriteReverse = m_renderer->CreatePngTexture("./Textures/eirp2.png");
	m_texParticle = m_renderer->CreatePngTexture("./Textures/particle.png");
	m_texRainParticle= m_renderer->CreatePngTexture("./Textures/rain.png");
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
	
	saveTime = 0;
	flowTime = 0;

	for (int i = 0; i < MaxBuilding; ++i) bulletTime[i] = 10;
	
	m_sound = new Sound();

	soundBG = m_sound->CreateSound("./Dependencies/SoundSamples/MF-W-90.XM");
	m_sound->PlaySound(soundBG, true, 0.2f);

};

SceneMgr::~SceneMgr()
{
	delete[] object;
}

void SceneMgr::Update(float elapsedTime)
{
	float nowTime = (float)timeGetTime() / 1000.0f;
	
	createRedTime += elapsedTime;

	saveTime += elapsedTime;//파티클떄문에 시간지속
	flowTime = elapsedTime;
	if (createRedTime > 0.8) //북쪽 캐릭터 생성 0.5초마다 생성
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
			if (characterTime[i] > 1) //1초마다 생성
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
			if (arrowObject[i]->Update(elapsedTime)) arrowObject[i]= NULL;
		}
	}
	for (int i = 0; i < MaxBullet; ++i)
	{
		if (buildingBullet[i])
		{
			if (buildingBullet[i]->Update(elapsedTime)) buildingBullet[i] = NULL;
		}
	}

	for (int i = 0; i < MaxBuilding; ++i) //빌딩의 총알
	{
		if (building[i])
		{
			building[i]->Update(elapsedTime);
			bulletTime[i] += elapsedTime;
			if (bulletTime[i] >1) //0.5초마다 발사
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
	m_renderer->DrawTexturedRect(0, 0, 0, 800, 1, 1, 1, 1, m_texBackground, 0.99);

	for (int i = 0; i < MaxObject; ++i)
	{
		if (object[i])
		{
			
			
			if (object[i]->GetTeamType() == REDTEAM) object[i]->DrawTexturedRect(m_renderer, m_texSpriteReverse);
			else if (object[i]->GetTeamType() == BLUETEAM) object[i]->DrawTexturedRect(m_renderer, m_texSprite);
		}
	}
	for (int i = 0; i < MaxArrow; ++i)
	{
		if (arrowObject[i]) arrowObject[i]->DrawSolidRect(m_renderer);
	}
	for (int i = 0; i < MaxBullet; ++i)
	{
		if (buildingBullet[i])
		{
			buildingBullet[i]->DrawSolidBullet(m_renderer, m_texParticle, flowTime);
		}
	}
	for (int i = 0; i < MaxBuilding; ++i)
	{
		if (building[i])
		{
			if(building[i]->GetTeamType()==REDTEAM) building[i]->DrawTexturedRect(m_renderer, m_texRedBuilding);
			else if (building[i]->GetTeamType() == BLUETEAM) building[i]->DrawTexturedRect(m_renderer, m_texBlueBuilding);
		}
	}
	m_renderer->DrawText(-60, 370, GLUT_BITMAP_TIMES_ROMAN_24,0, 0, 0, "Red Team");
	m_renderer->DrawText(-60, -370, GLUT_BITMAP_TIMES_ROMAN_24, 0, 0, 0, "Blue Team");

	m_renderer->DrawParticleClimate(0, 0, 0, 1, 1, 1, 1, 1, -0.3, -0.3, m_texRainParticle, saveTime, 0.01);
	//DrawArrays  m_ParticleVertexCount를  6의배수
	//조건문그리면
}

void SceneMgr::MouseSet(int x, int y)
{
	float nowTime = (float)timeGetTime() / 1000.0f;

	if (nowTime - mouseTime > 0.3)
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
		for (int j = 0; j < MaxBuilding; ++j)
		{
			if (object[i] != NULL && building[j] != NULL && object[i]->GetTeamType() != building[j]->GetTeamType()
				&& Collision(object[i]->GetX(), object[i]->GetY(), object[i]->GetSize(), building[j]->GetX(), building[j]->GetY(), building[j]->GetSize()))
			{
				building[j]->SetLife(-object[i]->GetLife());
				object[i]->SetLife(-object[i]->GetLife());
				printf("%d번 캐릭터 , 빌딩 명중 \n", i);
			}
		}

	}
	for (int i = 0; i < MaxBullet; ++i)
	{
		check = 1;
		for (int j = 0; j < MaxObject; ++j)
		{
			if (buildingBullet[i] != NULL && object[j] != NULL && buildingBullet[i]->GetTeamType() != object[j]->GetTeamType()
				&& Collision(buildingBullet[i]->GetX(), buildingBullet[i]->GetY(), buildingBullet[i]->GetSize(), object[j]->GetX(), object[j]->GetY(), object[j]->GetSize()))
			{
				check = 0;
				object[j]->SetLife(-buildingBullet[i]->GetLife());
				buildingBullet[i]->SetLife(-buildingBullet[i]->GetLife());
				printf("%d번 총알 , %d번 캐릭터에 명중 \n", j, i);
			}
		}
		//이미지가 상좌에 배치됨 - 수정완료
		//제목에따라 잘되는가
		//데미지리스트에 따라 충돌
		//렌더링순위에 따라
		//쿨타임자유
		if (check)
		{
			for (int j = 0; j < MaxBuilding; ++j)
			{
				if (buildingBullet[i] != NULL && building[j] != NULL && buildingBullet[i]->GetTeamType() != building[j]->GetTeamType()
					&& Collision(buildingBullet[i]->GetX(), buildingBullet[i]->GetY(), buildingBullet[i]->GetSize(), building[j]->GetX(), building[j]->GetY(), building[j]->GetSize()))
				{
					check = 0;
					building[j]->SetLife(-buildingBullet[i]->GetLife());
					buildingBullet[i]->SetLife(-buildingBullet[i]->GetLife());
					printf("%d번 총알 , %d번 캐릭터에 명중 \n", j, i);
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

	if (ix - isize / 2 < jx + jsize / 2 && iy + isize / 2 > jy - jsize / 2 && ix + isize / 2 > jx - jsize / 2 && iy - isize / 2 < jy + jsize / 2) return true;

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
			delete building[i];
			building[i] = NULL;
		}
	}
	
}