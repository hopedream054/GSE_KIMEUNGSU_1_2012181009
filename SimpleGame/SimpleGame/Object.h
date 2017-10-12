#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include<math.h>

#include "Dependencies\glew.h"


class ObjectCC
{
	ObjectCC();
	~ObjectCC();
public:
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void Update();
	
};

