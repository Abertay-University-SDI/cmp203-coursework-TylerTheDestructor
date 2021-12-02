#include "Floor.h"


void Floor::calcFloor(float floorCountX, float floorCountZ,float texSize, float quadX, float quadY, float quadZ, float quadSize)
{
	floorVertCount = 0;
	for (int i = quadX; i < (quadX + floorCountX); i += quadSize)
	{
		for (int j = quadZ; j < (quadZ + floorCountZ); j += quadSize)
		{
			floorNorms.push_back(0.0f);
			floorNorms.push_back(-1.0f);
			floorNorms.push_back(0.0f);

			floorTexcoords.push_back(texSize);
			floorTexcoords.push_back(0.0f);
			
			floorVerts.push_back(i);
			floorVerts.push_back(quadY);
			floorVerts.push_back(j);

			floorNorms.push_back(0.0f);
			floorNorms.push_back(-1.0f);
			floorNorms.push_back(0.0f);

			floorTexcoords.push_back(texSize);
			floorTexcoords.push_back(texSize);

			floorVerts.push_back(i);
			floorVerts.push_back(quadY);
			floorVerts.push_back((j - quadSize));

			floorNorms.push_back(0.0f);
			floorNorms.push_back(-1.0f);
			floorNorms.push_back(0.0f);

			floorTexcoords.push_back(0.0f);
			floorTexcoords.push_back(texSize);

			floorVerts.push_back((i - quadSize));
			floorVerts.push_back(quadY);
			floorVerts.push_back((j - quadSize));

			floorNorms.push_back(0.0f);
			floorNorms.push_back(-1.0f);
			floorNorms.push_back(0.0f);

			floorTexcoords.push_back(0.0f);
			floorTexcoords.push_back(0.0f);

			floorVerts.push_back((i - quadSize));
			floorVerts.push_back(quadY);
			floorVerts.push_back(j);

			floorVertCount += 4;
		}
	}
}

void Floor::renderFloor(GLuint item)
{
	//using the second render function from the lab exercise to draw my disc
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, floorVerts.data());
	glNormalPointer(GL_FLOAT, 0, floorNorms.data());
	glTexCoordPointer(2, GL_FLOAT, 0, floorTexcoords.data());
	glBindTexture(GL_TEXTURE_2D, item);

	glDrawArrays(GL_QUADS, 0, floorVertCount);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();
}