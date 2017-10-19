// Algo ex 2.2 - Monsters
// Description : Battling program
// Author : Duncan Bourquard
// Date : 18.10.2017

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "json.hpp"

#include "monster.h"

#define UNSEEN_POSITION -500

#define GAME_STATE_POS_X 30 
#define GAME_STATE_POS_Y 200
#define TEXT_SIZE_GAMESTATE 24

#define TEXT_SIZE_DAMAGE 30
#define TIME_DISPLAY_DAMAGE 0.5f
#define OFFSET_DAMAGE_TEXT_Y 50
#define OFFSET_DAMAGE_TEXT_X 10

#define MONSTER_MOVE_SPEED 5

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

	json dataM1 = data["monsters"][0];
	json dataM2 = data["monsters"][1];
	Monster m1 = Monster(dataM1["health"], dataM1["attack"], dataM1["defense"], dataM1["speed"], dataM1["race"], sf::Vector2f(dataM1["position"]["x"], dataM1["position"]["y"]));
	Monster m2 = Monster(dataM2["health"], dataM2["attack"], dataM2["defense"], dataM2["speed"], dataM2["race"], sf::Vector2f(dataM2["position"]["x"], dataM2["position"]["y"]));

	sf::RenderWindow window(sf::VideoMode(data["windows"]["width"], data["windows"]["height"]), "SFML works!");
	window.setFramerateLimit(data["windows"]["framerateLimit"]);

	// Image Sprites
	sf::Texture textureCross;
	if (!textureCross.loadFromFile("data/img/ded.png"))
	{
		std::cerr << "Unable to load image (ded.png)";
	}
	sf::Sprite dedSprite;
	dedSprite.setTexture(textureCross);
	dedSprite.setPosition(UNSEEN_POSITION, UNSEEN_POSITION); // Positionnement en dehors de l'écran
	
	sf::Texture textureHeart;
	if (!textureHeart.loadFromFile("data/img/heart.png"))
	{
		std::cerr << "Unable to load image (heart.png)";
	}
	sf::Sprite heartSprite;
	heartSprite.setTexture(textureHeart);
	heartSprite.setPosition((m1.position.x + m2.position.x) / 2, (m1.position.y + m2.position.y) / 2.);

	// Text Sprites
	sf::Font font;
	if (!font.loadFromFile("data/Pixel.ttf"))
	{
		std::cerr << "Unable to load file (Pixeled.ttf)\n";
		system("pause");
		return EXIT_FAILURE;
	}

	sf::Text damageText;
	damageText.setFont(font);
	damageText.setCharacterSize(TEXT_SIZE_DAMAGE);
	damageText.setColor(sf::Color::Red);

	sf::Text gameStateText;
	gameStateText.setFont(font);
	gameStateText.setCharacterSize(TEXT_SIZE_GAMESTATE);
	gameStateText.setPosition(sf::Vector2f(GAME_STATE_POS_X, GAME_STATE_POS_Y));

	int round = 0, damage;
	double middleBattleground = (m1.position.x + m2.position.x)/2;
	bool ism1CurrentMonster;
	gameState currentGameState = IDLE;
	sf::Clock clock;
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
			case IDLE: // When no monsters are battling
				if (m1.race == m2.race)
				{
					gameStateText.setString("Monsters with the same race can't battle !\n Press 'Enter' button to close the window\n");
					window.draw(heartSprite);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
						window.close();
				}
				else if (m1.health > 0 && m2.health > 0) {
					round++;
					gameStateText.setString("Round " + std::to_string(round));
					currentGameState = MONSTER_ATTACKING;
				}
				else
				{
					std::string wonMonster = "";
					m1.health == 0 ? wonMonster = " Right " : wonMonster = " Left ";
					m1.health == 0 ? dedSprite.setPosition(m1.position) : dedSprite.setPosition(m2.position);
					gameStateText.setString(wonMonster + " monster has won in " + std::to_string(round) + " rounds !\n" +
						"Press 'Enter' button to close the window\n");
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) 
						window.close();
				}

				break;
			case MONSTER_ATTACKING: // When the monster moves toward to attack

				ism1CurrentMonster ? m1.move(MONSTER_MOVE_SPEED, 0) : m2.move(-MONSTER_MOVE_SPEED,0);

				if (m1.position.x >= middleBattleground || m2.position.x <= middleBattleground)
				{
					ism1CurrentMonster ? damage = m1.attack(m2) : damage = m2.attack(m1);
					damageText.setString("-" + std::to_string(damage));
					
					clock.restart();
					currentGameState = DEALING_DAMAGE;
					if (ism1CurrentMonster)
						damageText.setPosition(m2.position - sf::Vector2f(OFFSET_DAMAGE_TEXT_X, OFFSET_DAMAGE_TEXT_Y));
					else
						damageText.setPosition(m1.position - sf::Vector2f(OFFSET_DAMAGE_TEXT_X, OFFSET_DAMAGE_TEXT_Y));
				}
				break;
			case MONSTER_ENDING_ATTACK: // When the monster returns to its original position

				ism1CurrentMonster ? m1.move(-MONSTER_MOVE_SPEED, 0) : m2.move(MONSTER_MOVE_SPEED, 0);

				if (m1.position.x <= m1.origin.x && m2.position.x >= m2.origin.x)
				{
					currentGameState = IDLE;
					ism1CurrentMonster = !ism1CurrentMonster;
				}

				break;
			case DEALING_DAMAGE: // When the damage is applied
				window.draw(damageText);

				if (clock.getElapsedTime().asSeconds() >= TIME_DISPLAY_DAMAGE)
					currentGameState = MONSTER_ENDING_ATTACK;
				
				break;
			}

		// Displaying stuff
		m1.draw(window);
		m2.draw(window);
		window.draw(dedSprite);
		window.draw(gameStateText);
		
		window.display();
	}

	return 0;
}