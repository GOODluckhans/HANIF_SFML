#pragma once
#include <SFML/Graphics.hpp>
#include "../ECS/ECS.hpp"
class AnimationClass {
public:

	AnimationClass();

	void update(float dt,char dir,bool isMoving , ECS::Animation& anim);

private:

};
