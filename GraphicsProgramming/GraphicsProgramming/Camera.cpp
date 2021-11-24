#include "Camera.h"

#include <iostream>
using namespace::std;
Camera::Camera()
{

}

void Camera::update()
{
	float cosR, cosP, cosY;
	float sinR, sinP, sinY; //temp values for cos/sin
	//Roll, Pitch and Yall are variables stored by camera
	//handle rotation
	//Only want to calculate these once, when rotation changes, not every frame
	cosY = cosf(yaw * 3.1415 / 180);
	cosP = cosf(pitch * 3.1415 / 180);
	cosR = cosf(roll * 3.1415 / 180);
	sinY = sinf(yaw * 3.1415 / 180);
	sinP = sinf(pitch * 3.1415 / 180);
	sinR = sinf(roll * 3.1415 / 180);
	//This uses the equation of a sphere

	//Calculate the three vectors into glu Lookat
	//Look direction, position and the up vector
	//This function could also calculate the right vector
	forward.x = sinY * cosP;
	forward.y = sinP;
	forward.z = cosP * -cosY;

	//Look At Point
	//To calculate, add forward vector to camera position
	lookAt.x = forward.x + position.x;
	lookAt.y = forward.y + position.y;
	lookAt.z = forward.z + position.z;
	std::cout << lookAt.x << ", " << lookAt.y << ", " << lookAt.z << endl;
	//Up vector
	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY * sinR - sinP * cosR * -cosY;

	//Side vector (right)
	//this is a cross product between forwards and up vector
	// if you don't need to calculate this, dont
	right = forward.cross(up);
};

Vector3 Camera::getPosition()
{
	//return x,y,z coordinates of camera
	return Vector3(position.x, position.y, position.z);
}

Vector3 Camera::getLook()
{
	//return x,y,z coordinates of point camera is looking at
	return Vector3(lookAt.x, lookAt.y, lookAt.z);
}

Vector3 Camera::getUp()
{
	//return x,y,z coordinates of up vector
	return Vector3(up.x, up.y, up.z);
}

void Camera::rotateX(float rot, float dt)
{
	//changes Yaw value by amount input by user
	float x = getYaw();
	x += rot * dt;
	setYaw(x);
}

void Camera::rotateY(float rot, float dt)
{
	//changes Pitch by user value
	float y = getPitch();
	y += rot * dt;
	setPitch(y);
}

void Camera::moveForward(float rot, float dt)
{
	//gets x, y and z coordinate of camera, and increases/decreases them by multiplying the forwards vector by user value
	float x = position.getX();
	float y = position.getY();
	float z = position.getZ();
	x -= (forward.getX() * (rot * dt));
	y -= (forward.getY() * (rot * dt));
	z -= (forward.getZ() * (rot * dt));
	position.setX(x);
	position.setY(y);
	position.setZ(z);
}

void Camera::moveSide(float rot, float dt)
{
	//gets x and z coordinate of camera, and increases/decreases it by multiplying the sideways vector by user value
	float x = position.getX();
	float z = position.getZ();
	x -= (right.getX() * (rot * dt));
	z -= (right.getZ() * (rot * dt));
	position.setX(x);
	position.setZ(z);
}

void Camera::moveUp(float rot, float dt)
{
	//gets y coordinate of camera, and increases/decreases it by multiplying the up vector by user value
	float y = position.getY();
	y -= (up.getY() * (rot * dt));
	position.setY(y);
}

//These are pretty self explanatory
float Camera::getYaw()
{
	return yaw;
}

float Camera::getPitch()
{
	return pitch;
}

float Camera::getRoll()
{
	return roll;
}

void Camera::setYaw(float newYaw)
{
	yaw = newYaw;
}

void Camera::setPitch(float newPitch)
{
	pitch = newPitch;
}

void Camera::setRoll(float newRoll)
{
	roll = newRoll;
}
