#pragma once
#include <glm/glm.hpp>

class particle {
	bool has_tex;

	glm::vec3 place; // position
	glm::vec3 size;
	glm::vec3 speed;
	glm::vec3 acc;
	glm::vec3 angle;
	glm::vec3 color;

	unsigned int texture;
	float life;
	friend class emitter;

	void draw(unsigned int texture);
	void draw();
	void show();
	void update();

public:
	particle();
	const glm::vec3& getPlace();
	const glm::vec3& getSize();
	const glm::vec3& getAngle();

	particle(glm::vec3 size, glm::vec3 speed, glm::vec3 acc, glm::vec3 angle,
		glm::vec3 _life, unsigned int _texture);

	particle(glm::vec3 size, glm::vec3 speed, glm::vec3 acc, glm::vec3 angle,
		glm::vec3 _life, glm::vec3 _color);
};

class emitter {
	float x1, y1, x2, y2, z1, z2;
	int speed;
	bool isforever;
	int num;
	particle **p;
	bool dead;
	particle* (*f)();
	bool(*isDead)(particle*);

public:
	void emit(particle* (init)(), bool(*judge)(particle*));
	void emit(particle* (init)());
	void update();
	void reset();
	void fade();
	void show();
	emitter(int _speed, float _x1,
		float _x2, float _y1, float _y2, float _z1, float _z2);
};