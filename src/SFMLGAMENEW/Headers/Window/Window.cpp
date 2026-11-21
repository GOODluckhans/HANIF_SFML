#include "Window.hpp"

Window::Window() {
}

void Window::CreateWindow(sf::String title, sf::Vector2u size) {
	window.create(sf::VideoMode({ size.x, size.y }), title);
}
void Window::CreateWindow(sf::String title, sf::State s ) {
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	window.create(desktop, title , s);
}

sf::RenderWindow& Window::GetWindow() {
	return window;
}
