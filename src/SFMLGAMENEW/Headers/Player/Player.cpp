#include "Player.hpp"
#include "../Collision/Collision.hpp"
Player::Player(sf::RenderWindow& window, sf::Vector2f pos, sf::Texture tex, sf::Vector2f size) : window(window), camera(window) {
	this->pd.pos = pos;
	this->pd.tex = tex;
	this->pd.size = size;
	this->pd.canMove = true;

}
void Player::createPlayerECS() {

    player_entity = sm.CreateSpriteECS(
        pd.tex,
        pd.pos,
        pd.size,
        pd.canMove
    );
	auto& world = ECS::World::get_instance();
	auto transform = world.get_component<ECS::Transform>(player_entity);
    transform->get().bounds = sf::FloatRect(
        pd.pos,
        sf::Vector2f(
            pd.tex.getSize().x * pd.size.x,
            pd.tex.getSize().y * pd.size.y
        )
    );
}

void Player::drawPlayerECS(sf::RenderWindow& window) {
	sm.drawSpriteECS(window);
	
}


//
//void Player::update(float dt, ECS::Entity e) {
//
//    sm.update(player_entity, dt);
//
//    auto& world = ECS::World::get_instance();
//
//	auto transform_comp = world.get_component<ECS::Transform>(player_entity);
//	auto personality_comp = world.get_component<ECS::Personality>(player_entity);
//
//	if (!transform_comp || !personality_comp) {
//		return;
//	}
//
//	transform_comp->get().bounds = sf::FloatRect(
//		transform_comp->get().position,
//        sf::Vector2f(
//            pd.tex.getSize().x * pd.size.x,
//            pd.tex.getSize().y * pd.size.y
//        )
//    );
//
//    sf::Vector2f oldP = transform_comp->get().position;
//
//	sf::Vector2f movement(0.0f,0.0f);
//
//	if (personality_comp->get().canMove) {
//
//		
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
//			movement.x -= 100.f * dt;
//		}
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
//			movement.x += 100.f * dt;
//		}
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
//			movement.y -= 100.f * dt;
//		}
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
//			movement.y += 100.f * dt;
//		}
//
//	}
//	sf::Vector2f colliderSize = sf::Vector2f(pd.tex.getSize().x * pd.size.x, pd.tex.getSize().y * pd.size.y);
//	sf::Vector2f newPos = Collision::ResolveCollision(
//		oldP,
//		movement,
//		colliderSize,
//		e
//	);
//	transform_comp->get().position = newPos;
//	transform_comp->get().bounds = sf::FloatRect(newPos, colliderSize);
//}

void Player::update(float dt,std::vector< sf::FloatRect>& bound) {

	sm.update(player_entity, dt);

	auto& world = ECS::World::get_instance();

	auto transform_comp = world.get_component<ECS::Transform>(player_entity);
	auto personality_comp = world.get_component<ECS::Personality>(player_entity);

	if (!transform_comp || !personality_comp) {
		return;
	}

	transform_comp->get().bounds = sf::FloatRect(
		transform_comp->get().position,
		sf::Vector2f(
			pd.tex.getSize().x * pd.size.x,
			pd.tex.getSize().y * pd.size.y
		)
	);
	camera.update(dt, 5.f, transform_comp->get().position);
	sf::Vector2f oldP = transform_comp->get().position;

	sf::Vector2f movement(0.0f, 0.0f);

	if (personality_comp->get().canMove) {



		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			movement.x -= 100.f * dt;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			movement.x += 100.f * dt;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			movement.y -= 100.f * dt;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			movement.y += 100.f * dt;
		}

	}
	sf::Vector2f colliderSize(
		pd.tex.getSize().x * pd.size.x,
		pd.tex.getSize().y * pd.size.y
	);

	sf::Vector2f newPos = Collision::ResolveCollision(
		oldP,
		movement,
		colliderSize,
		bound
	);

	transform_comp->get().position = newPos;
	transform_comp->get().bounds = sf::FloatRect(newPos, colliderSize);
}