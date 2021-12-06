#pragma once

#include "glew.h"
#include "glut.h"
#include <vector>
#include "SOIL.h"
#include "Shadow.h"
#include "Model.h"

class Shit
{
public:
	void initialise();
	void calcFloor(float floorCountX, float floorCountZ, float texSize, float quadX, float quadY, float quadZ, float quadSize);
	void renderFloor();

	void renderPepsi();

	int floorVertCount;
	float shadowMatrix[16];

	std::vector<float> floorVerts, floorNorms, floorTexcoords;
	Shadow shadow;
	GLuint grassTex;
	Model pepsiman;
};