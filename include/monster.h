#ifndef MONSTER_H
#define MONSTER_H

#include <SFML/Graphics.hpp>

enum Race
{
	ORC = 1,
	TROLL = 2,
	GOBLIN = 3
};

class Monster
{
public:
	Monster(double health, double attackPower, double defensivePower, double speed, int race, sf::Vector2f position);
	~Monster();
	void attack(Monster& enemy);
	void draw(sf::RenderWindow&);

	double health;
	double attackPower;
	double defensivePower;
	double speed;
	Race race;

	
	sf::Vector2f position;
	sf::Texture texture;
	sf::Sprite sprite;
};

#endif

