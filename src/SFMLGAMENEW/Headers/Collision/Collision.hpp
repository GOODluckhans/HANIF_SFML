#pragma once
#include <SFML/Graphics.hpp>
#include "../ECS/ECS.hpp"
#include <vector>
class Collision {
public:

    static bool CheckCollision(
        const sf::FloatRect& a,
        const sf::FloatRect& b
    ) {
        return a.findIntersection(b).has_value();
    }

    static sf::Vector2f ResolveCollision(sf::Vector2f oldpos, sf::Vector2f& movement, sf::Vector2f& colliderSize, ECS::Entity e) {
        sf::Vector2f newPos = oldpos;
        auto& world = ECS::World::get_instance();
        auto obs = world.get_component<ECS::Transform>(e);

        //check x
        newPos.x += movement.x;

        sf::Vector2f colliderOffset(
            colliderSize.x * 0.25f,
            colliderSize.y * 0.25f
        );

        sf::Vector2f smallerSize(
            colliderSize.x * 0.5f,
            colliderSize.y * 0.5f
        );

        sf::FloatRect playerBounds(
            newPos + colliderOffset,
            smallerSize
        );

        if (CheckCollision(playerBounds, obs->get().bounds)) {
            newPos.x = oldpos.x;
        }

        //check y
        newPos.y += movement.y;

        playerBounds.position = newPos + colliderOffset;

        if (CheckCollision(playerBounds, obs->get().bounds)) {
            newPos.y = oldpos.y;
        }
        return newPos;
    }

    static sf::Vector2f ResolveCollision(
        sf::Vector2f oldpos,
        sf::Vector2f& movement,
        sf::Vector2f& colliderSize,
        const std::vector<sf::FloatRect>& bounds
    ) {
        sf::Vector2f newPos = oldpos;

        sf::Vector2f colliderOffset(
            colliderSize.x * 0.25f,
            colliderSize.y * 0.25f
        );

        sf::Vector2f smallerSize(
            colliderSize.x * 0.5f,
            colliderSize.y * 0.5f
        );

        // Check X
        sf::Vector2f candidate = newPos;
        candidate.x += movement.x;

        sf::FloatRect playerBounds(
            candidate + colliderOffset,
            smallerSize
        );

        bool collidedX = false;

        for (const auto& bound : bounds) {
            if (CheckCollision(playerBounds, bound)) {
                collidedX = true;
                break;
            }
        }

        if (!collidedX) {
            newPos.x = candidate.x;
        }

        // Check Y
        candidate = newPos;
        candidate.y += movement.y;

        playerBounds.position = candidate + colliderOffset;

        bool collidedY = false;

        for (const auto& bound : bounds) {
            if (CheckCollision(playerBounds, bound)) {
                collidedY = true;
                break;
            }
        }

        if (!collidedY) {
            newPos.y = candidate.y;
        }

        return newPos;
    }
};