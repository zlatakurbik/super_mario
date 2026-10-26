#include "flyable_enemy.hpp"

using biv::FlyableEnemy;

FlyableEnemy::FlyableEnemy(const Coord& top_left, const int width, const int height, float period) 
    : RectMapMovableAdapter(top_left, width, height),
      MovingCollisionable(top_left, width, height) {
    vspeed = 0;
    hspeed = 0.2;
	this->period = period;
}

biv::Rect FlyableEnemy::get_rect() const noexcept {
    return {top_left, width, height};
}

biv::Speed FlyableEnemy::get_speed() const noexcept {
    return {vspeed, hspeed};
}

void FlyableEnemy::process_horizontal_static_collision(Rect* obj) noexcept {
    hspeed = -hspeed;
    move_horizontally();
}

void FlyableEnemy::process_mario_collision(Collisionable* mario) noexcept {
    if (mario->get_speed().v > 0 && mario->get_speed().v != V_ACCELERATION) {
        // Марио прыгнул на врага
    } else {
        mario->kill();
    }
}

void FlyableEnemy::process_vertical_static_collision(Rect* obj) noexcept {}

void FlyableEnemy::move_vertically() noexcept {
    if (this->is_active() == true) {
        vspeed = 0;
    } else {
        vspeed = 1;
        top_left.y += vspeed;
    }
}