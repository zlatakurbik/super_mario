#include "console_moving_platform.hpp"

using biv::ConsoleMovingPlatform;

ConsoleMovingPlatform::ConsoleMovingPlatform(const Coord& top_left, const int width, const int height)
    : RectMapMovableAdapter(top_left, width, height),
      MovingCollisionable(top_left, width, height),     
      MovingPlatform(top_left, width, height) { 
}

char ConsoleMovingPlatform::get_brush() const noexcept {
	return 'M';
}

void ConsoleMovingPlatform::process_mario_collision(Collisionable* mario) noexcept {}
void ConsoleMovingPlatform::process_vertical_static_collision(Rect* obj) noexcept {}