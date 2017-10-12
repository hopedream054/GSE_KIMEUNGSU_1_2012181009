#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include<math.h>

#include "Dependencies\glew.h"


class ObjectCC
{
private:
	float t_x, t_y;
	float t_speed;
public:
	ObjectCC();
	~ObjectCC();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void Update();
	void DrawSence();
	void ObjectCC::Set_xy(float x, float y);
};

