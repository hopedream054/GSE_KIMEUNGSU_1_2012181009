#pragma once

#include"Object.h"
#include"Renderer.h"
#include"simplemacro.h"
#include "Sound.h"//srand
#include <windows.h>
#include <stdlib.h>       //srand

#include <time.h>     //time

#define MaxObject 50
#define MaxBullet 100
#define MaxArrow 100
#define MaxBuilding 6

#define OBJECT_BUILDING 1
#define OBJECT_CHARACTER 2
#define OBJECT_BULLET 3
#define OBJECT_ARROW 4


class SceneMgr
{
private:
	ObjectCC *object[MaxObject];
	ObjectCC *building[MaxBuilding];
	ObjectCC *buildingBullet[MaxBullet];
	ObjectCC *arrowObject[MaxArrow];
	int arrowType[MaxArrow];
	float characterTime[MaxObject];
	float createRedTime;
	float bulletTime[MaxBuilding];
	int n_building,n_bullet,n_arrow;
	int now;
	float prevTime;
	float mouseTime;
	float saveTime;
	float flowTime;//현재 지나간시간
	
	Renderer *m_renderer;
	Sound *	m_sound;
	int soundBG;

	GLuint m_texBlueBuilding;
	GLuint m_texBackground;
	GLuint m_texSprite;
	GLuint m_texSpriteReverse;
	GLuint m_texParticleRed;
	GLuint m_texParticleBlue;
	GLuint m_texRainParticle;
	GLuint m_texBoss;
	GLuint m_texBulletRed;
	GLuint m_texBulletBlue;

public:
	SceneMgr(int width, int height);
	~SceneMgr();
	void Update(float elapsedTime);
	void DrawObject();
	void MouseSet(int x, int y);
	void CollisionTest();
	bool Collision(float ix, float iy, float isize, float jx, float jy, float jsize);
	void ObjectTimeover();
	void ObjectLifeover();
};

