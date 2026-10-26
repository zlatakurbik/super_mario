#include "moving_platform.hpp"

#include "map_movable.hpp"

using biv::MovingPlatform;

MovingPlatform::MovingPlatform(const Coord& top_left, const int width, const int height) 
	: RectMapMovableAdapter(top_left, width, height), MovingCollisionable(top_left, width, height) {
	vspeed = 0;
	hspeed = 0.2;
	period = 100.0f;
	counter = 0;
}

biv::Rect MovingPlatform::get_rect() const noexcept {
	return {top_left, width, height};
}

biv::Speed MovingPlatform::get_speed() const noexcept {
	return {vspeed, hspeed};
}

void MovingPlatform::process_horizontal_static_collision(Rect* obj) noexcept {
	hspeed = -hspeed;
	move_horizontally();
}

void MovingPlatform::process_vertical_object_collision(Collisionable*) noexcept {}

void MovingPlatform::move_vertically() noexcept {
	if (this->is_active() == true) {
		vspeed = 0;
	}
	else {
		vspeed = 1;
		top_left.y += vspeed;
	}
}

void MovingPlatform::move_horizontally() noexcept {
	if (counter >= period) {
		counter = 0;
		hspeed = -hspeed;
	}
	top_left.x += hspeed;
	counter++;
}
