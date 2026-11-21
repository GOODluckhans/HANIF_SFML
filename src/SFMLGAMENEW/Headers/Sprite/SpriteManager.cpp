#include "SpriteManager.hpp"

ECS::Entity SpriteManager::CreateSpriteECS(sf::Texture& tex, sf::Vector2f& pos,sf::Vector2f scale, bool canMove) {
	// Implementation for creating sprite
	auto& world = ECS::World::get_instance();
	auto entity = world.createEntity();
	world.add_component<ECS::Transform>(entity, pos, scale);
	world.add_component<ECS::SpriteData>(entity, tex );
	world.add_component<ECS::Personality>(entity, canMove);
	world.add_component<ECS::Depth>(entity, 0.0f);

	return entity;	
}
ECS::Entity SpriteManager::CreateSpritePartECS(
	sf::Texture& tex,
	sf::Vector2f& pos,
	sf::Vector2f scale,
	sf::IntRect rect,
	float depthOffset,
	bool canMove
) {

	auto& world = ECS::World::get_instance();

	auto entity = world.createEntity();

	world.add_component<ECS::Transform>(entity, pos, scale);
	world.add_component<ECS::Depth>(entity, depthOffset);
	world.add_component<ECS::SpriteData>(
		entity,
		tex,
		rect,
		true
	);

	world.add_component<ECS::Personality>(entity, canMove);

	return entity;
}




void SpriteManager::drawSpriteECS(sf::RenderWindow& window) {

    auto& world = ECS::World::get_instance();

	struct RenderData
	{
		ECS::Entity e;
		float depthY;
	};

	std::vector<RenderData> renderQueue;

	for(const auto& [e, sprite] : world.get_component_map<ECS::SpriteData>()) {
		auto transform = world.get_component<ECS::Transform>(e);
		auto depth = world.get_component<ECS::Depth>(e);
		if (!transform) {
			continue;
		}
		float offsetY = 0.f;
		if(depth) {
			 offsetY = depth->get().offsetY;
		}
		float spriteHeight = sprite.texture.getSize().y;

		if (sprite.useTextureRect) {
			spriteHeight = sprite.textureRect.size.y;
		}

		float depthY = transform->get().position.y
			+ (spriteHeight * transform->get().scale.y)
			+ offsetY;
		renderQueue.push_back({ e, depthY });

	}

	std::stable_sort(
		renderQueue.begin(),
		renderQueue.end(),
		[](const RenderData& a, const RenderData& b) {
			return a.depthY < b.depthY;
		}
	);


	for (const auto& data : renderQueue) {
		auto spriteData = world.get_component<ECS::SpriteData>(data.e);
		auto transform = world.get_component<ECS::Transform>(data.e);

		if (!spriteData || !transform) {
			continue;
		}

		sf::Sprite drawnSprite(spriteData->get().texture);

		if (spriteData->get().useTextureRect) {
			drawnSprite.setTextureRect(spriteData->get().textureRect);
		}

		drawnSprite.setPosition(transform->get().position);
		drawnSprite.setScale(transform->get().scale);
		//spriteData->get().sprite = &drawnSprite;
		window.draw(drawnSprite);
	}
}


void SpriteManager::update(ECS::Entity entity, float dt) {

	auto& world = ECS::World::get_instance();
	//std::cout << world.get_component_map<ECS::Transform>().size() << std::endl;

}