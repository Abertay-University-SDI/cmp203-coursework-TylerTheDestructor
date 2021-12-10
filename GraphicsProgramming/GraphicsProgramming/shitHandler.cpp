#include "shitHandler.h"

void Shit::initialise()
{
	grassTex = SOIL_load_OGL_texture(
		"gfx/grass.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	pepsiman.load("models/pepsiman.obj", "models/Pepsiman.png");
	teapot.load("models/teapot.obj", "gfx/crate.png");
	
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Shit::calcFloor(float floorCountX, float floorCountZ,float texSize, float quadX, float quadY, float quadZ, float quadSize)
{
	floorVertCount = 0;
	for (int i = quadX; i < (quadX + floorCountX); i += quadSize)
	{
		for (int j = quadZ; j < (quadZ + floorCountZ); j += quadSize)
		{
			floorNorms.push_back(0.0f);
			floorNorms.push_back(1.0f);
			floorNorms.push_back(0.0f);

			floorTexcoords.push_back(0.0f);
			floorTexcoords.push_back(0.0f);

			floorVerts.push_back((i - quadSize));
			floorVerts.push_back(quadY);
			floorVerts.push_back((j - quadSize));

			floorNorms.push_back(0.0f);
			floorNorms.push_back(1.0f);
			floorNorms.push_back(0.0f);

			floorTexcoords.push_back(0.0f);
			floorTexcoords.push_back(texSize);

			floorVerts.push_back((i - quadSize));
			floorVerts.push_back(quadY);
			floorVerts.push_back(j);

			floorNorms.push_back(0.0f);
			floorNorms.push_back(1.0f);
			floorNorms.push_back(0.0f);

			floorTexcoords.push_back(texSize);
			floorTexcoords.push_back(texSize);

			floorVerts.push_back(i);
			floorVerts.push_back(quadY);
			floorVerts.push_back(j);

			floorNorms.push_back(0.0f);
			floorNorms.push_back(1.0f);
			floorNorms.push_back(0.0f);

			floorTexcoords.push_back(texSize);
			floorTexcoords.push_back(0.0f);

			floorVerts.push_back(i);
			floorVerts.push_back(quadY);
			floorVerts.push_back((j - quadSize));

			floorVertCount += 4;
		}
	}
}

void Shit::renderFloor()
{
	//using the second render function from the lab exercise to draw my disc
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, floorVerts.data());
	glNormalPointer(GL_FLOAT, 0, floorNorms.data());
	glTexCoordPointer(2, GL_FLOAT, 0, floorTexcoords.data());
	glBindTexture(GL_TEXTURE_2D, grassTex);

	glDrawArrays(GL_QUADS, 0, floorVertCount);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();
}

void Shit::renderPepsi()
{
	//PUTTING SHADOW STUFF HERE
	GLfloat shadowLightPos[4] = { 7.5,1,-3.5,1 };
	GLfloat shadowFloorVerts[12] = { -8.0f,-0.3f,-6.0f, //top left
									-8.0f,-0.3f,8.0f, //bottom left
									8.0f,-0.3f,8.0f, // bottom right
									8.0f,-0.3f,-6.0f }; //top right
	shadow.generateShadowMatrix(shadowMatrix, shadowLightPos, shadowFloorVerts);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.1f, 0.1f, 0.1f); // Shadow's colour
	glPushMatrix();
		glMultMatrixf((GLfloat*)shadowMatrix);
		//translate to floor and draw shadow, remember to match any transforms on the object
		glEnable(GL_BLEND);
		glTranslatef(7.0f, 0.31f, -3.0f);
		glScalef(0.005f, 0.005f, 0.005f);
		glRotatef(-45, 0.0f, 1.0f, 0.0f);
		pepsiman.render();
		glDisable(GL_BLEND);
	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f); // S
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(7.0f, 0.31f, -3.0f);
	glScalef(0.005f, 0.005f, 0.005f);
	glRotatef(-45, 0.0f, 1.0f, 0.0f);
	pepsiman.render();
	glPopMatrix();
}

void Shit::renderTeapot()
{
	//PUTTING SHADOW STUFF HERE
	GLfloat shadowLightPos[4] = { -7.5,1,-3.5,1 };
	GLfloat shadowFloorVerts[12] = { -8.0f,-0.3f,-6.0f, //top left
									-8.0f,-0.3f,8.0f, //bottom left
									8.0f,-0.3f,8.0f, // bottom right
									8.0f,-0.3f,-6.0f }; //top right
	shadow.generateShadowMatrix(shadowMatrix, shadowLightPos, shadowFloorVerts);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.1f, 0.1f, 0.1f); // Shadow's colour
	glPushMatrix();
	glMultMatrixf((GLfloat*)shadowMatrix);
	//translate to floor and draw shadow, remember to match any transforms on the object
	glEnable(GL_BLEND);
	glTranslatef(-6.8f, 0.1f, -2.8f);
	glScalef(0.05f, 0.05f, 0.05f);
	glRotatef(45, 0.0f, 1.0f, 0.0f);
	teapot.render();
	glDisable(GL_BLEND);
	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f); // S
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(-7.0f, 0.1f, -3.0f);
	glScalef(0.05f, 0.05f, 0.05f);
	glRotatef(45, 0.0f, 1.0f, 0.0f);
	teapot.render();
	glPopMatrix();
};