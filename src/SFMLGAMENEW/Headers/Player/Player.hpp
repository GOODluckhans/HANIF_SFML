#pragma once
#include <SFML/Graphics.hpp>
#include "../Sprite/SpriteManager.hpp"
#include "../Camera/Camera.hpp"
#include <vector>
#include "../Animation/Animation.hpp"

struct playerData {
	sf::Vector2f pos;
	sf::Texture tex;
	sf::Vector2f size;
	bool canMove = true;
	std::vector<std::pair<int, sf::IntRect>> animationFrames;
};
class Player {
	playerData pd;
	SpriteManager sm;
	Camera camera;
	ECS::Entity player_entity;
	sf::RenderWindow& window;
	AnimationClass animationClass;
public:


	Player(sf::RenderWindow& window,sf::Vector2f pos, sf::Texture tex, sf::Vector2f size);
	playerData& getPlayerData() {
		return pd;
	};
	ECS::Entity getPlayerEntity() {
		return player_entity;
	};
	void createPlayerECS(sf::IntRect rect);
	void drawPlayerECS(sf::RenderWindow& window);
	void update(float dt, ECS::Entity e);
	void update(float dt, std::vector<sf::FloatRect>& bound);
};