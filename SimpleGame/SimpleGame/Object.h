#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include<math.h>
#include <time.h>     //time

#include "Dependencies\glew.h"


class ObjectCC
{
private:
	int t_x, t_y;
	int t_speed;
	int t_size;
	int direction_x, direction_y;
public:
	ObjectCC();
	ObjectCC(float x, float y, float size, float speed);
	~ObjectCC();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void Update();
	void DrawSence();
	void ObjectCC::Set_xy(float x, float y);
};

