#include "Animation.hpp"

AnimationClass::AnimationClass(){}
void AnimationClass::update(float dt, char dir, bool isMoving,ECS::Animation& anim) {

    int idx = 0;

    if (dir == 'L') idx = 2;
    else if (dir == 'R') idx = 3;
    else if (dir == 'U') idx = 1;
    else if (dir == 'D') idx = 0;

    anim.lastDir = dir;
    if (!isMoving) {
        anim.currentFrame = 0;
        anim.animationTimer = 0.f;

        anim.lastRect = anim.animationPacks[idx].second;
        return;
    }
    anim.animationTimer += dt;

    if (anim.animationTimer >= anim.frameDuration) {
        anim.animationTimer = 0.f;
        anim.currentFrame = (anim.currentFrame + 1) % 4;
    }

    sf::IntRect rect = anim.animationPacks[idx].second;

    rect.position.x = anim.currentFrame * rect.size.x;

    anim.lastRect = rect;
}