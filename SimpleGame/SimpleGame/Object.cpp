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
ObjectCC::ObjectCC(float x, float y, int type)
{

	switch (type)
	{
	case OBJECT_BUILDING:
		object_size = 50;
		object_speed = 0;
		life = 30.0;
		object_R = 1;  object_G = 1; object_B = 0;
		break;
	case OBJECT_CHARACTER:
		object_size = 10;
		object_speed = 300;
		life = 10.0;
		object_R = 1;  object_G = 1; object_B = 1;
		break;
	case OBJECT_BULLET:
		object_size = 2;
		object_speed = 600;
		life = 20.0;
		object_R = 1;  object_G = 1; object_B = 1;
		break;
	case OBJECT_ARROW:
		object_size = 2;
		object_speed = 100;
		life = 10.0;
		object_R = 1;  object_G = 1; object_B = 1;
		break;
	}
	int objectAngle=rand() % 360;
	object_speedX = ( int(cos(objectAngle)*object_speed))%700;
	object_speedY= (int(tan(objectAngle)*object_speed))%700;
	object_type = type;
	object_x = x-400;
	object_y = 400-y;

	direction_x = 1;
	direction_y = 1;
	
	lifeTime = 5.0;
	
}
ObjectCC::~ObjectCC()
{
	
}

void ObjectCC::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	glBegin(GL_POLYGON);
	glColor3f(r, g , b);
	glVertex3f(x, y ,z);
	glVertex3f(x+size, y ,z);
	glVertex3f(x + size, y+size,z);
	glVertex3f(x , y+size,z);
	glEnd();
}
void ObjectCC::DrawSence(Renderer *m_renderer)
{
	m_renderer->DrawSolidRect(object_x, object_y, 0, object_size, object_R, object_G, object_B, 1);
}

void ObjectCC::Update(float timeget)
{
	if (object_x+ direction_x*object_speedX*timeget >= 400 ||
		object_x+ direction_x*object_speedX*timeget <= -400)
	{
		direction_x = direction_x*-1;
	}
	if (object_y+ direction_y*object_speedY*timeget >= 400 ||
		object_y+ direction_y*object_speedY*timeget <= -400)
	{
		direction_y = direction_y*-1;
	}
	object_x += direction_x*object_speedX*timeget;
	object_y += direction_y*object_speedY*timeget;
	lifeTime -= timeget;

}
void ObjectCC::Set_xy(float x, float y)
{
	object_x = x-400;
	object_y = 400-y;
}
void ObjectCC::Set_RGB(float R, float G, float B)
{
	object_R = R;  object_G = G; object_B = B;
}
float ObjectCC::GetX()
{
	return object_x;
}
float ObjectCC::GetY()
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