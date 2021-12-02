#pragma once

#include "glew.h"
#include "glut.h"
#include <vector>

class Floor
{
public:

	void calcFloor(float floorCountX, float floorCountZ, float texSize, float quadX, float quadY, float quadZ, float quadSize);
	void renderFloor(GLuint item);

	int floorVertCount;

	std::vector<float> floorVerts, floorNorms, floorTexcoords;
};