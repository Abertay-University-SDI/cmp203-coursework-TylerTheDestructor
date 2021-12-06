#pragma once
#include <vector>
#include "Vector3.h"
#include "glew.h"
#include "glut.h"
#include "Input.h"
#include <stdio.h>

class Camera
{
public:
	Camera();
	void update();

	Vector3 getPosition();
	Vector3 getLook();
	Vector3 getUp();

	void rotateX(float rot, float dt);
	void rotateY(float rot, float dt);
	void moveForward(float rot, float dt);
	void moveSide(float rot, float dt);
	void moveUp(float rot, float dt);

	float getYaw();
	float getPitch();
	float getRoll();

	void setYaw(float newYaw);
	void setPitch(float newPitch);
	void setRoll(float newRoll);

protected:
	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	Vector3 position = Vector3(0.0f, 0.3f, 0.0f);
	Vector3 lookAt = Vector3(0.0f, 0.0f, -8.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 forward = Vector3(0.0f, 0.0f, -1.0f);
	Vector3 right;
};

