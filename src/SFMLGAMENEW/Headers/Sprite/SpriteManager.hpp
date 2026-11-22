#pragma once
#include <SFML/Graphics.hpp>
#include "../ECS/ECS.hpp"
#include <iostream>	
class SpriteManager {
	
public:

	static void update(ECS::Entity entity, float dt);
	static ECS::Entity CreateSpriteECS(sf::Texture& tex, sf::Vector2f& pos,sf::Vector2f scale, bool canMove);
	static void drawSpriteECS(sf::RenderWindow& window);
	static ECS::Entity CreateSpritePartECS(sf::Texture& tex, sf::Vector2f& pos, sf::Vector2f scale, sf::IntRect rect, float depthY, bool canMove, std::vector<std::pair<int, sf::IntRect>> animationPacks = {});
};