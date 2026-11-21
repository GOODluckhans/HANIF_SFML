#pragma once
#include <SFML/Graphics.hpp>
#include "../ECS/ECS.hpp"
class Camera {
private:
	sf::RenderWindow& window;
	sf::View camera_view;

	sf::Vector2u mapSize = {3200, 3200};
public:
	Camera(sf::RenderWindow& window);
	void setZoom(float zoom);
	void  setCenter(const sf::Vector2f& center);
	sf::Vector2f getCenter() const;
	sf::View& getView();
	void setView(const sf::View& view);
	void update(float dt , float smoothness , const sf::Vector2f& pos);	
};
