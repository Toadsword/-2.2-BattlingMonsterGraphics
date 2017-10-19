// Algo ex 2.2 - Monsters
// Description : Battling program
// Author : Duncan Bourquard
// Date : 18.10.2017

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "json.hpp"

#include "monster.h"

using json = nlohmann::json;

enum gameState
{
	IDLE,
	MONSTER_ATTACKING,
	MONSTER_ENDING_ATTACK,
	DEALING_DAMAGE
};

int main()
{
	// Reading the json file
	std::ifstream ifs("data/elements.json");
	json data;
	if (ifs.is_open())
		data = json::parse(ifs);
	else
	{
		std::cerr << "Unable to read the json file.\n";
		system("pause");
		return EXIT_FAILURE;
	}

	sf::RenderWindow window(sf::VideoMode(data["windows"]["width"], data["windows"]["height"]), "SFML works!");
	window.setFramerateLimit(data["windows"]["framerateLimit"]);

	float speed = data["windows"]["speed"];
	
	gameState currentGameState = IDLE;
	sf::Clock clock; // démarre le chrono
	sf::Font font;
	if (!font.loadFromFile("data/Pixeled.ttf"))
	{
		std::cerr << "Unable to load file (Pixeled.ttf)\n";
		system("pause");
		return EXIT_FAILURE;
	}
	sf::Text damageText;
	damageText.setFont(font);
	damageText.setCharacterSize(24);
	damageText.setColor(sf::Color::Red);

	Monster m1 = Monster(data["monsters"][0]["health"], data["monsters"][0]["attack"], data["monsters"][0]["defense"], data["monsters"][0]["speed"], data["monsters"][0]["race"], sf::Vector2f(data["monsters"][0]["position"]["x"], data["monsters"][0]["position"]["y"]));
	Monster m2 = Monster(data["monsters"][1]["health"], data["monsters"][1]["attack"], data["monsters"][1]["defense"], data["monsters"][1]["speed"], data["monsters"][1]["race"], sf::Vector2f(data["monsters"][1]["position"]["x"], data["monsters"][1]["position"]["y"]));
	
	int round = 0;
	int damage;
	bool ism1CurrentMonster;
	m1.speed > m2.speed ? ism1CurrentMonster = true : ism1CurrentMonster = false;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
			switch (currentGameState)
			{
			case IDLE:
				if (m1.health > 0 && m2.health > 0) {
					round++;
					currentGameState = MONSTER_ATTACKING;
				}
				else
				{
					std::cout << "Monster ";
					m1.health == 0 ? std::cout << "1" : std::cout << "2";
					std::cout << " has won in " << round << "rounds !\n";
				}

				break;
			case MONSTER_ATTACKING:

				ism1CurrentMonster ? m1.move(5, 0) : m2.move(-5,0);

				if (m1.position.x >= 300 || m2.position.x <= 300)
				{
					currentGameState = DEALING_DAMAGE;
					clock.restart();
					ism1CurrentMonster ? damage = m1.attack(m2) : damage = m2.attack(m1);
					damageText.setString("-" + std::to_string(damage));
					if (ism1CurrentMonster)
						damageText.setPosition(m2.position - sf::Vector2f(10, 50));
					else
						damageText.setPosition(m1.position - sf::Vector2f(10, 50));
				}
				break;
			case MONSTER_ENDING_ATTACK:

				ism1CurrentMonster ? m1.move(-5, 0) : m2.move(5, 0);

				if (m1.position.x <= 100 && m2.position.x >= 500)
				{
					currentGameState = IDLE;
					ism1CurrentMonster = !ism1CurrentMonster;
				}

				break;
			case DEALING_DAMAGE:
				window.draw(damageText);
				if (clock.getElapsedTime().asSeconds() >= 0.5f)
				{
					currentGameState = MONSTER_ENDING_ATTACK;
				}
				break;
			}

		m1.draw(window);
		m2.draw(window);
		
		window.display();
	}

	return 0;
}