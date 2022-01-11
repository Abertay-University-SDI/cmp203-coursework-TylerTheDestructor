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
	
	camera1.init(0.0f, 0.3f, 0.0f, 0.0f, 0.0f, -8.0f, 0.0f, 1.0f, 0.0f);
	camera2.init(-5.0f, 2.0f, 0.0f, 0.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f);
	camera3.init(0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -8.0f, 0.0f, 1.0f, 0.0f);
	cameraNumber = 0;
	object.initialise();

	// Initialise scene variables
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluQuadricTexture(theOrb, GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Scene::handleInput(float dt)
{
	// Handle user input
	//makes variables for mouse coordinates
	//adjusts camera according to mouse position (rotates camera left if mouse is left of the center of the window, etc) then resets mouse back to center of window
	int mouseX = input->getMouseX(), mouseY = input->getMouseY();
	if (input->isKeyDown('1'))
	{
		cameraNumber = 0;
		input->setKeyUp('1');
	}
	if (input->isKeyDown('2'))
	{
		cameraNumber = 1;
		input->setKeyUp('2');
	}
	if (input->isKeyDown('3'))
	{
		cameraNumber = 2;
		input->setKeyUp('3');
	}
	// Handle user input


	switch (cameraNumber)
	{
	case 0:
		if (mouseX > (width / 2))
		{
			camera1.rotateX(80.0f, dt);
			camera1.update();
			glutWarpPointer((width / 2), (height / 2));
		}
		if (mouseX < (width / 2))
		{
			camera1.rotateX(-80.0f, dt);
			camera1.update();
			glutWarpPointer((width / 2), (height / 2));
		}
		if (mouseY > (height / 2))
		{
			camera1.rotateY(-80.0f, dt);
			camera1.update();
			glutWarpPointer((width / 2), (height / 2));
		}
		if (mouseY < (height / 2))
		{
			camera1.rotateY(80.0f, dt);
			camera1.update();
			glutWarpPointer((width / 2), (height / 2));
		}
		if (input->isKeyDown('a'))
		{
			camera1.moveSide(1.0f, dt);
			camera1.update();
		};
		if (input->isKeyDown('d'))
		{
			camera1.moveSide(-1.0f, dt);
			camera1.update();
		};

		if (input->isKeyDown('w'))
		{
			camera1.moveForward(-1.0f, dt);
			camera1.update();
		};

		if (input->isKeyDown('s'))
		{
			camera1.moveForward(1.0f, dt);
			camera1.update();
		};

		if (input->isKeyDown('q'))
		{
			camera1.moveUp(1.0f, dt);
			camera1.update();
		};

		if (input->isKeyDown('e'))
		{
			camera1.moveUp(-1.0f, dt);
			camera1.update();
		}
		break;
	case 1:
		//stationary camera so doesnt need anything
		break;
	case 2:
		//camera3.update();
		break;
	}
	//wireframe mode toggle
	if (input->isKeyDown('p'))
	{
		if (wfMode == false)
		{
			glPolygonMode(GL_FRONT, GL_LINE);
				glPolygonMode(GL_BACK, GL_LINE);
				input->setKeyUp('p');
				wfMode = true;
		}
		else if (wfMode == true)
		{
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
			input->setKeyUp('p');
			wfMode = false;
		}
	};
};

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
	switch (cameraNumber) {
	case 0:
		gluLookAt(camera1.getPosition().x, camera1.getPosition().y, camera1.getPosition().z, 
			camera1.getLook().x, camera1.getLook().y, camera1.getLook().z,
			camera1.getUp().x, camera1.getUp().y, camera1.getUp().z);
		break;
	case 1:
		gluLookAt(camera2.getPosition().x, camera2.getPosition().y, camera2.getPosition().z, 
			camera2.getLook().x, camera2.getLook().y, camera2.getLook().z,
			0.0f, 1.0f, 0.0f); //using up vector without mouse controls will make the camera rotate oddly, we don't want this
		break;
	case 2:
		glRotatef(rotation, 0.0f, 1.0f, 0.0f);
		gluLookAt(camera3.getPosition().x, camera3.getPosition().y, camera3.getPosition().z,
			camera3.getLook().x, camera3.getLook().y, camera3.getLook().z,
			0.0f, 1.0f, 0.0f);
		break;
	}
	

	//set up light

	GLfloat Light1_Diffuse[] = { 0.0f,1.0f,0.0f,1.0f };
	GLfloat Light2_Diffuse[] = { 1.0f,0.0f,0.0f,1.0f };
	GLfloat Light3_Diffuse[] = { 0.0f,0.0f,1.0f,1.0f };

	GLfloat Light1_Position[] = { 0.0f,1.0f,1.0f,1.0f };
	GLfloat Light2_Position[] = { 7.5f,1.0f,-3.5f,1.0f };
	GLfloat Light3_Position[] = { -7.5f,1.0f,-3.5f,1.0f };

	GLfloat Light1_Direction[] = { 0.0f, -1.0f,0.0f };

	glPushMatrix();
	//glRotatef(rotation, 0.0f, 1.0f, 0.0f);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light1_Diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, Light1_Position); 
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Light1_Direction); 
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45); 
	glEnable(GL_LIGHT0);
	glPopMatrix();

	glLightfv(GL_LIGHT1, GL_DIFFUSE, Light2_Diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, Light2_Position);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_DIFFUSE, Light3_Diffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, Light3_Position);
	glEnable(GL_LIGHT2);
	// Render geometry/scene here -------------------------------------
	switch (cameraNumber) {
	case 0:
		skyBox.skyboxRender(camera1.getPosition());
		break;
	case 1:
		skyBox.skyboxRender(camera2.getPosition());
		break;
	case 2:
		skyBox.skyboxRender(camera3.getPosition());
		break;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	//turn off writing to frame buffer and set up stencil
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//disable depth test since we dont want depth values when writing to stencil buffer
	glDisable(GL_DEPTH_TEST); 
	for (int i = -8; i < 9; i++)
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
		glTranslatef(0.0f, 0.5f, -8.0f);
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
	for (int i = -8; i < 9; i++)
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
		glTranslatef(0.0f, -0.1f, -8.0f);
		glRotatef(90, -1.0f, 0.0f, 0.0f);
		glRotatef(180, 0.0f, 2.0f, 0.0f);
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);
		gluSphere(theOrb, 0.2, 10, 10);
	glPopMatrix();

	glPushMatrix();
	//draw shadow impostor because im lazy
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
	glBindTexture(GL_TEXTURE_2D, shadowTexture);
	glBegin(GL_QUADS);
	drawFloor(0.25f, -0.29f, -7.75f, 0.5f, 1);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();

	//draw the rest of the floor (non-transparent no reflection crap woohoo normal floor)
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	object.calcFloor(17, 14, 2.0f, -8.0f, -0.3f, -6.0f, 1.0f); //do NOT set quadSize lower than 1
	object.renderFloor();
	//render objects
	object.renderPepsi();
	object.renderTeapot();

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
} //this is here exclusively for referencing the code

void Scene::drawFloor(float x, float y, float z, float size, float texSize)
{
	glPushMatrix();
	glNormal3f(0.0f, 1.0f, 0.0f);
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