#include "Scene.h"

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// Other OpenGL / render setting should be applied here.
	shadowTexture = SOIL_load_OGL_texture(
		"gfx/imposter.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	orbTexture = SOIL_load_OGL_texture(
		"gfx/damnDaniel.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	myTexture = SOIL_load_OGL_texture(
		"gfx/grass.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	
	
	// Initialise scene variables
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluQuadricTexture(theOrb, GL_TRUE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Scene::handleInput(float dt)
{
	// Handle user input
	//makes variables for mouse coordinates
	int mouseX = input->getMouseX(), mouseY = input->getMouseY();
	int width = glutGet(GLUT_WINDOW_WIDTH), height = glutGet(GLUT_WINDOW_HEIGHT);
	//adjusts camera according to mouse position (rotates camera left if mouse is left of the center of the window, etc) then resets mouse back to center of window
	if (mouseX > (width / 2))
	{
		camera.rotateX(80.0f, dt);
		camera.update();
		glutWarpPointer((width / 2), (height / 2));
	}
	if (mouseX < (width / 2))
	{
		camera.rotateX(-80.0f, dt);
		camera.update();
		glutWarpPointer((width / 2), (height / 2));
	}
	if (mouseY > (height / 2))
	{
		camera.rotateY(-80.0f, dt);
		camera.update();
		glutWarpPointer((width / 2), (height / 2));
	}
	if (mouseY < (height / 2))
	{
		camera.rotateY(80.0f, dt);
		camera.update();
		glutWarpPointer((width / 2), (height / 2));
	}
	// Handle user input
	if (input->isKeyDown('a'))
	{
		camera.moveSide(1.0f, dt);
		camera.update();
	};

	if (input->isKeyDown('d'))
	{
		camera.moveSide(-1.0f, dt);
		camera.update();
	};

	if (input->isKeyDown('w'))
	{
		camera.moveForward(-1.0f, dt);
		camera.update();
	};

	if (input->isKeyDown('s'))
	{
		camera.moveForward(1.0f, dt);
		camera.update();
	};

	if (input->isKeyDown('q'))
	{
		camera.moveUp(1.0f, dt);
		camera.update();
	};

	if (input->isKeyDown('e'))
	{
		camera.moveUp(-1.0f, dt);
		camera.update();
	};
}

void Scene::update(float dt)
{
	// update scene related variables.
	rotation += speed * dt;
	// Calculate FPS for output
	calculateFPS();
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	Vector3 camPos = Vector3(camera.getPosition());
	Vector3 camLook = Vector3(camera.getLook());
	Vector3 camUp = Vector3(camera.getUp());
	gluLookAt(camPos.x, camPos.y, camPos.z, camLook.x, camLook.y, camLook.z, camUp.x, camUp.y, camUp.z);

	//set up light
	GLfloat Light_Ambient[] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat Light_Diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat Light_Position[] = { 0.0f,-1.0f,0.0f,1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);
	glEnable(GL_LIGHT0);
	// Render geometry/scene here -------------------------------------
	skyBox.skyboxRender(camPos);

	glBindTexture(GL_TEXTURE_2D, 0);
	//turn off writing to frame buffer and set up stencil
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//disable depth test since we dont want depth values when writing to stencil buffer
	glDisable(GL_DEPTH_TEST); 
	for (int i = -8; i < 8; i++)
	{
		for (int j = -8; j < -6; j++)
		{
			glBegin(GL_QUADS);
			drawFloor(i, -0.3f, j, 1, 2);
			glEnd();
		}
	}
	//can reenable depth test now
	glEnable(GL_DEPTH_TEST);
	//turn writing to frame buffer back on, modify stencil 
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//draw orbs reflection (TODO: replace with a 3D model later?)
	glBindTexture(GL_TEXTURE_2D, orbTexture);
	glPushMatrix();
		glScalef(1.0f, -1.0f, 1.0f);
		glTranslatef(0.0f, 0.6f, -8.0f);
		glRotatef(90, -1.0f, 0.0f, 0.0f);
		glRotatef(180, 0.0f, 2.0f, 0.0f);
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);
		gluSphere(theOrb, 0.2, 10, 10);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	//stencil test can now be disabled
	glDisable(GL_STENCIL_TEST);

	glColor4f(0.5f, 0.5f, 1.0f, 0.8f);
	//enable blending and disable lighting (setting up transparent tiles)
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	for (int i = -8; i < 8; i++)
	{
		for (int j = -8; j < -6; j++)
		{
			glBegin(GL_QUADS);
			drawFloor(i, -0.3f, j, 1, 2);
			glEnd();
		}
	}
	//reenable lighting and disable blend after floor is drawn
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
	//draw orb (same as above orb in that i should use a 3d model)
	glBindTexture(GL_TEXTURE_2D, orbTexture);
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -8.0f);
		glRotatef(90, -1.0f, 0.0f, 0.0f);
		glRotatef(180, 0.0f, 2.0f, 0.0f);
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);
		gluSphere(theOrb, 0.2, 10, 10);
	glPopMatrix();
	glPushMatrix();
	//FUCK SHADOWS!!!
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
	glBindTexture(GL_TEXTURE_2D, shadowTexture);
	glBegin(GL_QUADS);
	drawFloor(0.25f, -0.29f, -7.75f, 0.5f, 1);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
	//glBindTexture(GL_TEXTURE_2D, 0);
	//draw the rest of the floor (non-transparent no reflection crap woohoo normal floor)
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		floor.calcFloor(16, 14, 2.0f, -8.0f, -0.3f, -6.0f, 1.0f);
		floor.renderFloor(myTexture);
	glPopMatrix();
	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	renderTextOutput();

	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

void Scene::initialiseOpenGL()
{
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// Calculates FPS
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}

void Scene::drawCube()
{
	//front face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	//bottom face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	//left face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	//back face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	//top face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	//right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
}

void Scene::drawFloor(float x, float y, float z, float size, float texSize)
{
	glPushMatrix();
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(texSize, 0.0f);
	glVertex3f(x, y, z);
	glTexCoord2f(texSize, texSize);
	glVertex3f(x, y, (z-size));
	glTexCoord2f(0.0f, texSize);
	glVertex3f((x - size), y, (z - size));
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f((x - size), y, z);
	glPopMatrix();
}