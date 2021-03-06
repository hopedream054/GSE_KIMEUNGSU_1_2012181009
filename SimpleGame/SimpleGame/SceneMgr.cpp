#include"stdafx.h"
#include"SceneMgr.h"


SceneMgr::SceneMgr(int width, int height)
{
	
	m_renderer = new Renderer(width, height);
	
	m_texBlueBuilding = m_renderer->CreatePngTexture("./Textures/building1.png");
	m_texBackground= m_renderer->CreatePngTexture("./Textures/background.png");
	m_texSprite= m_renderer->CreatePngTexture("./Textures/eirp1.png");
	m_texSpriteReverse = m_renderer->CreatePngTexture("./Textures/eirp2.png");
	m_texParticleRed = m_renderer->CreatePngTexture("./Textures/particlered.png");
	m_texParticleBlue = m_renderer->CreatePngTexture("./Textures/particleblue.png");
	m_texRainParticle= m_renderer->CreatePngTexture("./Textures/rain.png");
	m_texBoss= m_renderer->CreatePngTexture("./Textures/boss.png");
	m_texBulletRed = m_renderer->CreatePngTexture("./Textures/bulletred.png");
	m_texBulletBlue = m_renderer->CreatePngTexture("./Textures/bulletblue.png");

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
	building[0] = new ObjectCC(width/2, 160, OBJECT_BUILDING,REDTEAM);
	building[1] = new ObjectCC(width / 4*1, 700, OBJECT_BUILDING, BLUETEAM);
	building[2] = new ObjectCC(width / 4*2, 700, OBJECT_BUILDING, BLUETEAM);
	building[3] = new ObjectCC(width / 4*3, 700, OBJECT_BUILDING, BLUETEAM);
	
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

	saveTime += elapsedTime;//��ƼŬ������ �ð�����
	flowTime = elapsedTime;
	if (createRedTime > 0.8 && building[0]) //���� ĳ���� ���� 0.5�ʸ��� ����
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
	for (int i = 0; i < MaxObject; ++i) //ĳ���ͺ� �ַο� ����
	{
		if (object[i])
		{
			object[i]->Update(elapsedTime);
			characterTime[i] += elapsedTime;
			if (characterTime[i] > 1) //1�ʸ��� ����
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

	for (int i = 0; i < MaxBuilding; ++i) //������ �Ѿ�
	{
		if (building[i])
		{
			building[i]->Update(elapsedTime);
			bulletTime[i] += elapsedTime;



			if (building[i]->GetTeamType() == BLUETEAM)
			{
				if (bulletTime[i] >1) //0.5�ʸ��� �߻�
				{

					buildingBullet[(++n_bullet) % MaxBullet] = new ObjectCC(building[i]->GetX(), building[i]->GetY()
						, OBJECT_BULLET, building[i]->GetTeamType());
					bulletTime[i] = 0;
				}
			}
			else if (building[i]->GetTeamType() == REDTEAM)
			{
				if (bulletTime[i] >0.4) //0.5�ʸ��� �߻�
				{

					buildingBullet[(++n_bullet) % MaxBullet] = new ObjectCC(building[i]->GetX()+105, building[i]->GetY()+110
						, OBJECT_BULLET, building[i]->GetTeamType());
					buildingBullet[(++n_bullet) % MaxBullet] = new ObjectCC(building[i]->GetX() - 105, building[i]->GetY()+110
						, OBJECT_BULLET, building[i]->GetTeamType());
					bulletTime[i] = 0;
				}
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
		if (arrowObject[i])
		{
			if (arrowObject[i]->GetTeamType() == BLUETEAM)
			{
				arrowObject[i]->DrawTexturedRect(m_renderer, m_texBulletBlue);

			}
			else if (arrowObject[i]->GetTeamType() == REDTEAM)
			{
				arrowObject[i]->DrawTexturedRect(m_renderer, m_texBulletRed);
			}
			
		}
	}
	for (int i = 0; i < MaxBullet; ++i)
	{
		if (buildingBullet[i])
		{
			if (buildingBullet[i]->GetTeamType() == BLUETEAM)
			{
				buildingBullet[i]->DrawSolidBullet(m_renderer, m_texParticleBlue, flowTime);
			}
			else if (buildingBullet[i]->GetTeamType() == REDTEAM)
			{
				buildingBullet[i]->DrawSolidBullet(m_renderer, m_texParticleRed, flowTime);
			}
			
		}
	}
	for (int i = 0; i < MaxBuilding; ++i)
	{
		if (building[i])
		{
			if(building[i]->GetTeamType()==REDTEAM) building[i]->DrawTexturedRect(m_renderer, m_texBoss);
			else if (building[i]->GetTeamType() == BLUETEAM) building[i]->DrawTexturedRect(m_renderer, m_texBlueBuilding);
		}
	}
	m_renderer->DrawText(-60, 370, GLUT_BITMAP_TIMES_ROMAN_24,0, 0, 0, "Red Team");
	m_renderer->DrawText(-60, -370, GLUT_BITMAP_TIMES_ROMAN_24, 0, 0, 0, "Blue Team");

	m_renderer->DrawParticleClimate(0, 0, 0, 1, 1, 1, 1, 1, -0.3, -0.3, m_texRainParticle, saveTime, 0.01);
	//DrawArrays  m_ParticleVertexCount��  6�ǹ��
	//���ǹ��׸���
}

void SceneMgr::MouseSet(int x, int y)
{
	float nowTime = (float)timeGetTime() / 1000.0f;

	int ccc = 0;
	for (int i = 1; i < 4;++i)
	{
		if (building[i]) ++ccc;//�Ʊ� �ǹ��� ����ִ��� üũ
	}
	if (ccc == 0) return;

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
				printf("%d�� ĳ���� , ���� ���� \n", i);
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
				printf("%d�� �Ѿ� , %d�� ĳ���Ϳ� ���� \n", j, i);
			}
		}
		//�̹����� ���¿� ��ġ�� - �����Ϸ�
		//���񿡵��� �ߵǴ°�
		//����������Ʈ�� ���� �浹
		//������������ ����
		//��Ÿ������
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
					printf("%d�� �Ѿ� , %d�� ĳ���Ϳ� ���� \n", j, i);
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
				printf("%d�� Arrow , ������ ���� \n", i);
			}
		}
		for (int j = 0; j < MaxObject; ++j)
		{
			if (arrowType[i] != j && arrowObject[i] != NULL && object[j] != NULL && arrowObject[i]->GetTeamType() != object[j]->GetTeamType()
				&& Collision(arrowObject[i]->GetX(), arrowObject[i]->GetY(), arrowObject[i]->GetSize(), object[j]->GetX(), object[j]->GetY(), object[j]->GetSize()))
			{
				object[j]->SetLife(-arrowObject[i]->GetLife());
				arrowObject[i]->SetLife(-arrowObject[i]->GetLife());
				printf("%d���� �� %d�� Arrow ,  %d�� ĳ���� ����\n", arrowType[i],i,j);
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