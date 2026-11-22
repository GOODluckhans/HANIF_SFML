#include "Player.hpp"
#include "../Collision/Collision.hpp"
#include <cmath>
Player::Player(sf::RenderWindow& window, sf::Vector2f pos, sf::Texture tex, sf::Vector2f size) : window(window), camera(window) {
	this->pd.pos = pos;
	this->pd.tex = tex;
	this->pd.size = size;
	this->pd.canMove = true;
	this->pd.animationFrames = {
		{102 , sf::IntRect({ 0, 0 },{ 102, 160 }) }, //up
		{ 102 , sf::IntRect({ 0,160 } ,{102 , 160})}, // down
		{ 102 , sf::IntRect({ 0 , 320 } ,{102 , 160}) }, // left
		{ 102 , sf::IntRect({ 0 , 480 } ,{102 , 160}) }, // right
	};

}
void Player::createPlayerECS(sf::IntRect rect) {



	player_entity = sm.CreateSpritePartECS(
		pd.tex,
		pd.pos,
		pd.size,
		rect,
		0.f,
		pd.canMove,
		pd.animationFrames
		
	);

	auto& world = ECS::World::get_instance();
	auto transform = world.get_component<ECS::Transform>(player_entity);
	auto animation = world.get_component<ECS::Animation>(player_entity);

	sf::Vector2f colliderSize(
		animation->get().lastRect.size.x * pd.size.x,
		animation->get().lastRect.size.y * pd.size.y
	);

	transform->get().bounds = sf::FloatRect(
		pd.pos,
		colliderSize
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
	auto sprite = world.get_component<ECS::SpriteData>(player_entity);
	auto animation = world.get_component<ECS::Animation>(player_entity);
	if (!transform_comp || !personality_comp) {
		return;
	}

	sf::Vector2f colliderSize(
		animation->get().lastRect.size.x * pd.size.x,
		animation->get().lastRect.size.y * pd.size.y
	);

	transform_comp->get().bounds = sf::FloatRect(
		transform_comp->get().position,
		colliderSize
	);
	camera.update(dt, 5.f, transform_comp->get().position);
	sf::Vector2f oldP = transform_comp->get().position;

	sf::Vector2f movement(0.0f, 0.0f);

	if (personality_comp->get().canMove) {

		auto& anim = animation->get();

		bool isMoving = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			movement.x -= 100.f * dt;
			anim.lastDir = 'L';
			isMoving = true;
		}
		 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			movement.x += 100.f * dt;
			anim.lastDir = 'R';
			isMoving = true;
		}
		 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			movement.y -= 100.f * dt;
			anim.lastDir = 'U';
			isMoving = true;
		}
		 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			movement.y += 100.f * dt;
			anim.lastDir = 'D';
			isMoving = true;
		}

		animationClass.update(dt, anim.lastDir, isMoving, anim);
	}

	if(movement.x != 0.f || movement.y != 0.f) {
		movement /= std::sqrt(2.f);
	}

	sf::Vector2f newPos = Collision::ResolveCollision(
		oldP,
		movement,
		colliderSize,
		bound
	);

	transform_comp->get().position = newPos;
	transform_comp->get().bounds = sf::FloatRect(newPos, colliderSize);
}