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

enum battleState
{
	IDLE,
	ANIMATE
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
		
	Monster m1 = Monster(data["monsters"][0]["health"], data["monsters"][0]["attack"], data["monsters"][0]["defense"], data["monsters"][0]["speed"], data["monsters"][0]["race"], sf::Vector2f(data["monsters"][0]["position"]["x"], data["monsters"][0]["position"]["y"]));
	Monster m2 = Monster(data["monsters"][1]["health"], data["monsters"][1]["attack"], data["monsters"][1]["defense"], data["monsters"][1]["speed"], data["monsters"][1]["race"], sf::Vector2f(data["monsters"][1]["position"]["x"], data["monsters"][1]["position"]["y"]));
	
	int round = 0;
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

		do
		{
			ism1CurrentMonster ? m1.attack(m2) : m2.attack(m1);
			ism1CurrentMonster = !ism1CurrentMonster;
			round++;
		} while (m1.health > 0 && m2.health > 0);

		std::cout << "Monster ";
		m1.health == 0 ? std::cout << "1" : std::cout << "2";
		std::cout << " has won in " << round << "rounds !\n";

		window.clear();
		
		m1.draw(window);
		m2.draw(window);
		
		window.display();
	}

	return 0;
}