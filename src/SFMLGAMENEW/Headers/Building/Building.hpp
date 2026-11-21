#pragma once
#include <SFML/Graphics.hpp>
#include "../Sprite/SpriteManager.hpp"
struct buildingData {
	sf::Vector2f pos;
	sf::Texture tex;
	sf::Vector2f size;

};
class Building {
	buildingData bd;
	SpriteManager sm;

	ECS::Entity bodyEntity;
	ECS::Entity roofEntity;
public:


	Building(sf::Vector2f pos, sf::Texture tex, sf::Vector2f size);
	buildingData& getBuildingData() { return bd; }
	ECS::Entity getBodyEntity() { return bodyEntity; }	
	void createBuildingECS(sf::IntRect bodyRect, sf::IntRect roofRect);
	void drawBuildingECS(sf::RenderWindow& window);
	void update(float dt);
};