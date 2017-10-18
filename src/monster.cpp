#include "monster.h"
#include <iostream>

Monster::Monster(double health, double attackPower, double defensivePower, double speed, int race, sf::Vector2f position)
	: health(health), attackPower(attackPower), defensivePower(defensivePower), speed(speed), position(position)
{
	this->race = static_cast<Race>(race);
	std::string imagePath = "";
	switch(this->race)
	{
	case ORC:
		imagePath = "data/img/orc.png";
		break;
	case TROLL:
		imagePath = "data/img/troll.png";
		break;
	case GOBLIN:
		imagePath = "data/img/goblin.png";
		break;
	}
	if (!texture.loadFromFile(imagePath))
	{
		std::cerr << "Unable to load image (" << imagePath << ")";
	}
	sprite.setTexture(texture);
	sprite.setPosition(position);
}


Monster::~Monster()
{
}

void Monster::attack(Monster& enemy)
{
	double damage = this->attackPower - enemy.defensivePower;
	if (damage < 1)
		damage = 1;

	enemy.health = enemy.health - damage;
}

void Monster::draw(sf::RenderWindow& window)
{
	window.draw(this->sprite);
}