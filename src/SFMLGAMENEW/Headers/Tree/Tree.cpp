#include "Tree.hpp"

Tree::Tree(sf::Vector2f pos, sf::Texture tex, sf::Vector2f size) {
    this->td.pos = pos;
    this->td.tex = tex;
    this->td.size = size;
}

void Tree::createTreeECS(sf::IntRect trunkRect, sf::IntRect leavesRect) {

    sf::Vector2f trunkPos = td.pos + sf::Vector2f(
        trunkRect.position.x * td.size.x,
        trunkRect.position.y * td.size.y - 5.f
    );

    trunkEntity = sm.CreateSpritePartECS(
        td.tex,
        trunkPos,
        td.size,
        trunkRect,
        0.f,
        false
    );

    sf::Vector2f leavesPos = td.pos + sf::Vector2f(
        leavesRect.position.x * td.size.x,
        leavesRect.position.y * td.size.y
    );

    leavesEntity = sm.CreateSpritePartECS(
        td.tex,
        leavesPos,
        td.size,
        leavesRect,
        200.f,
        false
    );
    auto& world = ECS::World::get_instance();
    auto transform = world.get_component<ECS::Transform>(trunkEntity);
    float width = trunkRect.size.x * td.size.x * 0.5f;
    float height = trunkRect.size.y * td.size.y * 0.5f;

    transform->get().bounds = sf::FloatRect(
        sf::Vector2f(
            trunkPos.x + width * 0.5f,
            trunkPos.y + height * 0.5f
        ),
        sf::Vector2f(width, height)
    );
}

void Tree::drawTreeECS(sf::RenderWindow& window) {
    sm.drawSpriteECS(window);
}

void Tree::update(float dt) {
    sm.update(trunkEntity, dt);
    sm.update(leavesEntity, dt);
}