#pragma once
#include <SFML/Graphics.hpp>
#include "../Sprite/SpriteManager.hpp"
struct treeData {
	sf::Vector2f pos;
	sf::Texture tex;
	sf::Vector2f size;
	


};
class Tree {
	treeData td;
	SpriteManager sm;

	ECS::Entity trunkEntity;
	ECS::Entity leavesEntity;
public:


	Tree(sf::Vector2f pos, sf::Texture tex, sf::Vector2f size);
	treeData& getTreeData() { return td; };
	ECS::Entity getTrunkEntity() { return trunkEntity; };
	void createTreeECS(sf::IntRect trunkRect, sf::IntRect leavesRect);
	void drawTreeECS(sf::RenderWindow& window);
	void update(float dt);
};