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
};
ObjectCC::ObjectCC(float x, float y, float size, float speed)
{
	object_x = x;
	object_y = y;
	object_size = size;
	object_speed = speed;
	object_R = 1;  object_G = 1; object_B = 1;
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
void ObjectCC::DrawSence()
{
	DrawSolidRect(object_x, object_y, 0, object_size, object_R, object_G, object_B, 1);
}

void ObjectCC::Update(float timeget)
{
	if (object_x+ direction_x*object_speed*timeget >= 800 ||
		object_x+ direction_x*object_speed*timeget <= 0)
	{
		direction_x = direction_x*-1;
	}
	if (object_y+ direction_y*object_speed*timeget >= 800 ||
		object_y+ direction_y*object_speed*timeget <= 0)
	{
		direction_y = direction_y*-1;
	}
	object_x += direction_x*object_speed*timeget;
	object_y += direction_y*object_speed*timeget;
	lifeTime -= timeget;
}
void ObjectCC::Set_xy(float x, float y)
{
	object_x = x;
	object_y = y;
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