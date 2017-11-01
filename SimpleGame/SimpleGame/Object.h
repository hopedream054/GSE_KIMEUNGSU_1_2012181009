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
	float object_x, object_y;
	float object_speed;
	float object_size;
	float direction_x, direction_y;
	float object_R, object_G, object_B;
	float lifeTime,life;
public:
	ObjectCC();
	ObjectCC(float x, float y, float size, float speed);
	~ObjectCC();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void Update(float timeget);
	void DrawSence();
	void Set_xy(float x, float y);
	void Set_RGB(float R, float G, float B);
	float GetX();
	float GetY();
	float GetSize();
	float GetLifeTime();
	float GetLife();
};

