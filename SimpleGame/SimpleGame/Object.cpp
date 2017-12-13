#include"stdafx.h"
#include"Object.h"



ObjectCC::ObjectCC()
{
	object_x = 0;
	object_y = 0;
	object_speed = 100;
	object_size = 50;
	direction_x = 1;
	direction_y = 1;
	object_R = 1;  object_G = 1; object_B = 1;
	lifeTime = 5.0;
	life = 5.0;
};
ObjectCC::ObjectCC(float x, float y, int type,int teamtype)
{
	objectAngle = rand() % 360;

	switch (type)
	{
	case OBJECT_BUILDING:
		object_size = 100;
		object_speed = 0;
		life = 800.0;
		object_level = LEVEL_SKY;
		object_speedX = (int(cos(objectAngle)*object_speed)) % 700;
		object_speedY = (int(sin(objectAngle)*object_speed)) % 700;
		break;
	case OBJECT_CHARACTER:
		object_size = 30;
		object_speed = 300;
		life = 40.0;
		object_level = LEVEL_GROUND;
		object_speedX = (int(cos(objectAngle)*object_speed)) % 700;
		object_speedY = (int(sin(objectAngle)*object_speed)) % 700;
		

		if (teamtype == REDTEAM)
		{
			if (rand() % 5 == 0)
			{
				unitType = 1;
				object_R = 1;  object_G = 0; object_B = 0;
				object_speedY = -10;
				object_speedX = 0;
				object_size = 80;
				life = 250.0;
			}
			else
			{
				unitType = 2;
				object_R = 1;  object_G = 0; object_B = 0;
				if (object_speedY > 0) object_speedY = object_speedY*-1;
			}
		}
		else if (teamtype == BLUETEAM)
		{
			if (rand() % 15 == 0)
			{
				unitType = 1;
				object_R = 0;  object_G = 0; object_B = 1;
				object_speedY = 10;
				object_speedX = 0;
				object_size = 80;
				life = 250.0;
			}
			else
			{
				unitType = 2;
				object_R = 0;  object_G = 0; object_B = 1;
				if (object_speedY < 0) object_speedY = object_speedY*-1;

			}
		}
		

		break;
	case OBJECT_BULLET:
		object_size = 4;
		object_speed = 100;
		life = 15.0;
		object_level = LEVEL_UNDERGROUND;
		object_speedX = (int(cos(objectAngle)*object_speed)) % 700;
		object_speedY = (int(sin(objectAngle)*object_speed)) % 700;
		if (teamtype == REDTEAM)
		{
			object_R = 1;  object_G = 0; object_B = 0;
			if (object_speedY > 0) object_speedY = object_speedY*-1;
		}
		else if (teamtype == BLUETEAM)
		{
			object_R = 0;  object_G = 0; object_B = 1;
			if (object_speedY < 0) object_speedY = object_speedY*-1;
		}
		break;
	case OBJECT_ARROW:
		object_size = 4;
		object_speed = 100;
		life = 10.0;
		object_level = LEVEL_UNDERGROUND;
		object_speedX = (int(cos(objectAngle)*object_speed)) % 700;
		object_speedY = (int(sin(objectAngle)*object_speed)) % 700;
		if (teamtype == REDTEAM)
		{
			object_R = 0.5;  object_G = 0.2; object_B = 0.7;
			if (object_speedY > 0) object_speedY = object_speedY*-1;
		}
		else if (teamtype == BLUETEAM)
		{
			object_R = 1;  object_G = 1; object_B = 0	;
			if (object_speedY < 0) object_speedY = object_speedY*-1;
		}
		break;
	}
	
	
	
	object_type = type;
	object_x = x-WIDTHSIMPLE/2;
	object_y = HEIGHTSIMPLE / 2 -y;
	objectTeam_type = teamtype;

	direction_x = 1;
	direction_y = 1;
	
	lifeTime = 5.0;
	aniTime = -1;
}
ObjectCC::~ObjectCC()
{
	
}
void ObjectCC::DrawSolidBullet(Renderer *m_renderer, GLuint Image,float saveTime)
{
	m_renderer->DrawSolidRect(object_x, object_y, 0, object_size, object_R, object_G, object_B, 1, object_level);
	//각도추가할것
	//파일정리할것(정크파일들이 너무많음)
	//PNG좀 그럴듯한걸로 찾을것
	//이걸 부디 숙지할것.
	int particleDirX=0, particleDirY=0;
	if (object_speedY >40) {
		particleDirX = 0;
		particleDirY = -5;
	}
	if (object_speedY <-40) {
		particleDirX = 0;
		particleDirY = 5;
	}
	m_renderer->DrawParticle(object_x, object_y, 0, 10, 1, 1, 1, 0.5, particleDirX, particleDirY, Image, saveTime/5);
}
void ObjectCC::DrawSolidRect(Renderer *m_renderer)
{
	m_renderer->DrawSolidRect(object_x, object_y, 0, object_size, object_R, object_G, object_B, 1, object_level);
}

void ObjectCC::DrawTexturedRect(Renderer *m_renderer, GLuint Image)
{
	if (object_type == OBJECT_BUILDING)
	{
		if (objectTeam_type == BLUETEAM) m_renderer->DrawSolidRectGauge(object_x, object_y + object_size / 2, 0, object_size, 10, 0, 0, 1, 1, (float)life / 800, object_level);
		else if (objectTeam_type == REDTEAM) m_renderer->DrawSolidRectGauge(object_x, object_y + object_size / 2, 0, object_size, 10, 1, 0, 0, 1, (float)life / 800, object_level);
		m_renderer->DrawTexturedRect(object_x, object_y, 0, object_size, 1, 1, 1, 1, Image, object_level);
	}
	else if (object_type == OBJECT_CHARACTER)
	{
		if (objectTeam_type == BLUETEAM)
		{
			if (unitType == 2)
			{
				aniTime = (++aniTime) % 4;
				m_renderer->DrawSolidRectGauge(object_x, object_y + object_size / 2 + 5, 0, object_size, 4, 0, 0, 1, 1, (float)life / 40, object_level);
				m_renderer->DrawTexturedRectSeq(object_x, object_y, 0, object_size, 1, 1, 1, 1, Image, (aniTime), 0, 4, 1, object_level);
			}
			else if (unitType == 1)
			{
				aniTime = (++aniTime) % 4;
				m_renderer->DrawSolidRectGauge(object_x, object_y + object_size / 2 + 5, 0, object_size, 4, 0, 0, 1, 1, (float)life / 250, object_level);
				m_renderer->DrawTexturedRectSeq(object_x, object_y, 0, object_size, 1, 1, 1, 1, Image, (aniTime), 0, 4, 1, object_level);
			}
		}
		else if (objectTeam_type == REDTEAM)
		{
			if (unitType == 2)
			{
				aniTime = (++aniTime) % 4;
				m_renderer->DrawSolidRectGauge(object_x, object_y + object_size / 2 + 5, 0, object_size, 4, 1, 0, 0, 1, (float)life / 40, object_level);
				m_renderer->DrawTexturedRectSeq(object_x, object_y, 0, object_size, 1, 1, 1, 1, Image, (aniTime), 0, 4, 1, object_level);
			}
			else if (unitType == 1)
			{
				aniTime = (++aniTime) % 4;
				m_renderer->DrawSolidRectGauge(object_x, object_y + object_size / 2 + 5, 0, object_size, 4, 1, 0, 0, 1, (float)life / 250, object_level);
				m_renderer->DrawTexturedRectSeq(object_x, object_y, 0, object_size, 1, 1, 1, 1, Image, (aniTime), 0, 4, 1, object_level);
			}
		}
	}
	
}
int ObjectCC::Update(float timeget)
{
	

	if (object_x+ direction_x*object_speedX*timeget >= WIDTHSIMPLE /2 ||
		object_x+ direction_x*object_speedX*timeget <= -WIDTHSIMPLE/2)
	{
		if (object_type == OBJECT_ARROW || object_type == OBJECT_BULLET) return 1;
		direction_x = direction_x*-1;
	}
	if (object_y+ direction_y*object_speedY*timeget >= HEIGHTSIMPLE/2 ||
		object_y+ direction_y*object_speedY*timeget <= -HEIGHTSIMPLE/2)
	{
		if(object_type== OBJECT_ARROW || object_type == OBJECT_BULLET) return 1;
		direction_y = direction_y*-1;
	}
	object_x += direction_x*object_speedX*timeget;
	object_y += direction_y*object_speedY*timeget;
	lifeTime -= timeget;
	return 0;
}
void ObjectCC::Set_xy(float x, float y)
{
	object_x = x- WIDTHSIMPLE/2;
	object_y = HEIGHTSIMPLE / 2 -y;

}
void ObjectCC::Set_RGB(float R, float G, float B)
{
	object_R = R;  object_G = G; object_B = B;
}
float ObjectCC::GetX()
{
	return object_x+ WIDTHSIMPLE / 2;
}
float ObjectCC::GetY()
{
	return HEIGHTSIMPLE/2-object_y;
}
float ObjectCC::RealGetX()
{
	return object_x;
}
float ObjectCC::RealGetY()
{
	return object_y;
}
float ObjectCC::GetSize()
{
	return object_size;
}
float ObjectCC::GetLifeTime()
{
	return lifeTime;
}

float ObjectCC::GetLife()
{
	return life;
}
float ObjectCC::SetLife(float damage)
{
	life += damage;
	return life;
}
int ObjectCC::GetTeamType()
{
	return objectTeam_type;
}