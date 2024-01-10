#pragma once

struct Nyan
{
	double posX, posY, velY, velX, radius;
	int health;
};

struct Enemy
{
	double posX, posY, radius;
	int health=3;
};

struct Boost
{
	double posX, posY, radius;
};
