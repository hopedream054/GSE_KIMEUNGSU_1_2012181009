#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include<math.h>
#include <time.h>     //time
#include"Renderer.h"
#include"simplemacro.h"

#include "Dependencies\glew.h"

#define OBJECT_BUILDING 1
#define OBJECT_CHARACTER 2
#define OBJECT_BULLET 3
#define OBJECT_ARROW 4


class ObjectCC
{
private:
	float object_x, object_y;
	float object_speed;
	float object_speedX, object_speedY;
	float object_size;
	float direction_x, direction_y;
	float object_R, object_G, object_B;
	float lifeTime,life;
	int object_type;
	int objectTeam_type;
	float object_level;
	int aniTime;
	int objectAngle;
	int unitType;
public:
	ObjectCC();
	ObjectCC(float x, float y, int type, int teamtype);
	~ObjectCC();
	int Update(float timeget);
	void DrawSolidRect(Renderer *m_renderer);
	void DrawTexturedRect(Renderer *m_renderer, GLuint Image);
	void DrawSolidBullet(Renderer *m_renderer, GLuint Image, float saveTime);
	void Set_xy(float x, float y);
	void Set_RGB(float R, float G, float B);
	float GetX();
	float GetY();
	float RealGetX();
	float RealGetY();
	float GetSize();
	float GetLifeTime();
	float GetLife();
	float SetLife(float damage);
	int GetTeamType();
};

