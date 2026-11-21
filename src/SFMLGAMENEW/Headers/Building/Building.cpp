#include "Building.hpp"

Building::Building(sf::Vector2f pos, sf::Texture tex, sf::Vector2f size) {
	this->bd.pos = pos;
	this->bd.tex = tex;
	this->bd.size = size;

}

void Building::createBuildingECS(sf::IntRect bodyRect, sf::IntRect roofRect) {

    sf::Vector2f bodyPos = bd.pos + sf::Vector2f(
        bodyRect.position.x * bd.size.x,
        bodyRect.position.y * bd.size.y - 5.f
    );

    bodyEntity = sm.CreateSpritePartECS(
        bd.tex,
        bodyPos,
        bd.size,
        bodyRect,
        0.f,
        false
    );

    sf::Vector2f roofPos = bd.pos + sf::Vector2f(
        roofRect.position.x * bd.size.x,
        roofRect.position.y * bd.size.y
    );

    roofEntity = sm.CreateSpritePartECS(
        bd.tex,
        roofPos,
        bd.size,
        roofRect,
        200.f,
        false
    );

	auto& world = ECS::World::get_instance();
	auto transform = world.get_component<ECS::Transform>(bodyEntity);
    float width = bodyRect.size.x * bd.size.x;
    float height = bodyRect.size.y * bd.size.y;

    transform->get().bounds = sf::FloatRect(
        sf::Vector2f(
            bodyPos.x  ,
            bodyPos.y 
        ),
        sf::Vector2f(width, height)
    );
}

void Building::drawBuildingECS(sf::RenderWindow& window) {
	sm.drawSpriteECS(window);
}

void Building::update(float dt) {
	sm.update(bodyEntity, dt);
    sm.update(roofEntity, dt);
}