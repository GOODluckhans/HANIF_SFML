#include "Camera.hpp"
#include <algorithm>
Camera::Camera(sf::RenderWindow& window) : window(window) {
	camera_view = window.getDefaultView();
	camera_view.zoom(0.5f);
}
void Camera::setZoom(float zoom) {
	camera_view.zoom(zoom);
}

void Camera::setCenter(const sf::Vector2f& center) {
	camera_view.setCenter(center);
}

sf::Vector2f Camera::getCenter() const {
	return camera_view.getCenter();
}

sf::View& Camera::getView() {
	return camera_view;
}

void Camera::setView(const sf::View& view) {
	camera_view = view;
}
void Camera::update(float dt, float smoothness,const sf::Vector2f& pos) {
	sf::Vector2f cameraPos = camera_view.getCenter();
	sf::Vector2f targetPos = pos;
	cameraPos += (targetPos - cameraPos) * smoothness * dt;

	sf::Vector2f viewSize = camera_view.getSize();
	float halfWidth = viewSize.x / 2.0f;
	float halfHeight = viewSize.y / 2.0f;

	cameraPos.x = std::clamp(cameraPos.x, halfWidth, mapSize.x - halfWidth);
	cameraPos.y = std::clamp(cameraPos.y, halfHeight, mapSize.y - halfHeight);


	camera_view.setCenter(cameraPos);
	window.setView(camera_view);
}