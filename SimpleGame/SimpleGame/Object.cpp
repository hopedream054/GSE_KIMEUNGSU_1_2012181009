#include"stdafx.h"
#include"Object.h"



ObjectCC::ObjectCC()
{
	t_x = 0;
	t_y = 0;
	t_speed = 10;
	t_size = 10;
	direction_x = 1;
	direction_y = 1;
};
ObjectCC::ObjectCC(float x, float y, float size, float speed)
{
	t_x = x;
	t_y = y;
	t_size = size;
	t_speed = speed;
	
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
	DrawSolidRect(t_x, t_y, 0, t_size, 1, 1, 1, 1);
}

void ObjectCC::Update()
{
	if (t_x >= 800 || t_x <= 0)
	{
		direction_x = direction_x*-1;
	}
	if (t_y >= 800 || t_y <= 0)
	{
		direction_y = direction_y*-1;
	}
	t_x += direction_x*t_speed;
	t_y += direction_y*t_speed;
}
void ObjectCC::Set_xy(float x, float y)
{
	t_x = x;
	t_y = y;
}