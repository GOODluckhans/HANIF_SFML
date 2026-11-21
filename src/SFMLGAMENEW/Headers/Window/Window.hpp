#pragma once
#include <SFML/Graphics.hpp>

class Window {
	sf::RenderWindow window;
public:
	Window();
	
	void CreateWindow(sf::String title , sf::Vector2u size);
	void CreateWindow(sf::String title, sf::State s);

	sf::RenderWindow& GetWindow();

};