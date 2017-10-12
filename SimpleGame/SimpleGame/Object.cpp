#include"stdafx.h"
#include"Object.h"



ObjectCC::ObjectCC()
{
	t_x = 0;
	t_y = 0;
	t_speed = 10;
};

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
	DrawSolidRect(t_x, t_y, 0, 250, 1, 0, 1, 1);
}
void ObjectCC::Update()
{
	if (t_x > 400 || t_x < 0)
	{
		t_speed = t_speed*-1;
	}
	if (t_x == 200)
	{

		t_x = 200;
	}
	t_x += t_speed;
}
void ObjectCC::Set_xy(float x, float y)
{
	t_x = x;
	t_y = y;
}