#include "moving_collisionable.hpp"

#include "map_movable.hpp"

using biv::MovingCollisionable;

MovingCollisionable::MovingCollisionable(const Coord& top_left, const int width, const int height) 
	: RectMapMovableAdapter(top_left, width, height) {
	vspeed = 0;
	hspeed = 0.2;
	period = 100.0f;
	counter = 0;
}

biv::Rect MovingCollisionable::get_rect() const noexcept {
	return {top_left, width, height};
}

biv::Speed MovingCollisionable::get_speed() const noexcept {
	return {vspeed, hspeed};
}

void MovingCollisionable::process_horizontal_static_collision(Rect* obj) noexcept {
	hspeed = -hspeed;
	move_horizontally();
}

void MovingCollisionable::process_vertical_object_collision(Collisionable* obj) noexcept {};

void MovingCollisionable::move_vertically() noexcept {}

void MovingCollisionable::move_horizontally() noexcept {
	if (counter >= period) {
		counter = 0;
		hspeed = -hspeed;
	}
	top_left.x += hspeed;
	counter++;
}

