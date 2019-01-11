#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


#define MAX_ACCE 0.005f
#define MIN_ACCE -0.005f

#define DISTANCE_SLICE 1.0f
#define TIME_SLICE 0.3f

#define ACCE 0.001f

#define MAX_SPEED 0.05f
#define MIN_SPEED -0.05f

//  for calculate rotate
#define SPEED_SLICE 0.1f

#define MIN_X 0.0f
#define MAX_X 15.f
#define MIN_Z 0.0f
#define MAX_Z 15.f



// contains information for model's movements
class Move {
private:
	// speed
	float xv, yv, zv;
	// position
	float curx, dstx;
	float cury, dsty;
	float curz, dstz;
public:
	Move() :
		xv(0.1f), yv(0.1f), zv(0.1f),
		curx(-6.0f), dstx(-6.0f),
		cury(0.0f), dsty(0.0f),
		curz(-6.0f), dstz(-6.0f)
	{}

	glm::vec3 getV() {
		return glm::vec3(xv, yv, zv);
	}

	// set speed
	void setV(glm::vec3 v) {
		xv = v[0];
		yv = v[1];
		zv = v[2];
	}

	// set current position
	void setCurPosition(float x, float y, float z) {
		curx = x;
		cury = y;
		curz = z;
	}

	// set destination
	void setDstPosition(float x, float y, float z) {
		dstx = x;
		dsty = y;
		dstz = z;
	}

	// set destination
	void setDstPosition(glm::vec3 v) {
		dstx = v[0];
		dsty = v[1];
		dstz = v[2];
	}

	void printPosition() {
		std::cout << "dstx: " << dstx << ", dsty: " << dsty << ", dstz: " << dstz << std::endl;
		std::cout << "curx: " << curx << ", cury: " << cury << ", curz: " << curz << std::endl;
		std::cout << "xv: " << xv << ", yv: " << yv << ", zv: " << zv << std::endl;
	}

	glm::vec3 getCurPosition() {
		return glm::vec3(curx, cury, curz);
	}

	glm::vec3 getDstPosition() {
		return glm::vec3(dstx, dsty, dstz);
	}
	// hide from position (pos)
	void setHideDst(glm::vec3 pos) {
		dstx = curx - pos[0];
		dsty = cury - pos[1];
		dstz = curz - pos[2];
	}

	// set a random destination
	void setRandomDst() {
		float dtx = ((rand() % 100 / (double)101) * (MAX_X - MIN_X)) + MIN_X;
		float dty = ((rand() % 100 / (double)101) * 2) - 1;
		float dtz = ((rand() % 100 / (double)101) * (MAX_Z - MIN_Z)) + MIN_Z;
		// printPosition();
		setDstPosition(glm::vec3(dtx, dty, dtz));
	}

	// get next position
	glm::vec3 getNextPosition() {
		float dx = xv * TIME_SLICE;
		float dy = yv * TIME_SLICE;
		float dz = zv * TIME_SLICE;
		curx += dx;
		cury += dy;
		curz += dz;
		// suit the scene model
		if (curx < MIN_X)
			curx = MIN_X;
		else if (curx > MAX_X)
			curx = MAX_X;
		if (curz < MIN_Z)
			curz = MIN_Z;
		else if (curz > MAX_Z)
			curz = MAX_Z;
		if (cury < 1.f)
			cury = 1.f;
		else if (cury > 5.0f)
			cury = 5.0f;
		updateSpeed();
		return glm::vec3(curx, cury, curz);
	}

	glm::vec3 getRotate() {
		// return rotation
		return glm::vec3(xv / SPEED_SLICE, yv / SPEED_SLICE, zv / SPEED_SLICE);
	}

	// fall to ground
	glm::vec3 fallToGround() {
		if (cury < -0.6f) {
			return glm::vec3(curx, cury, curz);
		}
		yv += TIME_SLICE * 0.001f;
		cury -= TIME_SLICE * yv;
		return glm::vec3(curx, cury, curz);
	}

	void updateSpeed() {
		xv += getSpeedSlice(xv, curx, dstx);
		yv += getSpeedSlice(yv, cury, dsty);
		zv += getSpeedSlice(zv, cury, dstz);
		limitSpeed(xv);
		limitSpeed(yv);
		limitSpeed(zv);
	}

	void limitSpeed(float &v) {
		if (v > MAX_SPEED)
			v = MAX_SPEED;
		else if (v < MIN_SPEED)
			v = MIN_SPEED;
	}

	float getSpeedSlice(float &v, float cur, float dst) {
		float distance = getDistance(cur, dst);
		// accelarate according to distance
		// the further, the faster
		float dv = (distance / DISTANCE_SLICE) * ACCE;
		// setup dv limit
		if (dv > MAX_ACCE)
			dv = MAX_ACCE;
		else if (dv < MIN_ACCE)
			dv = MIN_ACCE;
		return dv;
	}

	double getDistance(float cur, float dst) {
		float minus = dst - cur;
		float dis = sqrt(minus * minus);
		if (minus > 0)
			return dis;
		else
			return -dis;
	}
};
