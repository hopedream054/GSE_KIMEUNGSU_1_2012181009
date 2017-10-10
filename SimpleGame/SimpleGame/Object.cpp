#include"stdafx.h"
#include"Object.h"



ObjectCC::ObjectCC()
{
	
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