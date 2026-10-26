#include "console_flyable_enemy.hpp"

using biv::ConsoleFlyableEnemy;

ConsoleFlyableEnemy::ConsoleFlyableEnemy(const Coord& top_left, const int width, const int height) 
    : Rect(top_left, width, height),
      RectMapMovableAdapter(top_left, width, height),
      FlyableEnemy(top_left, width, height) {}

char ConsoleFlyableEnemy::get_brush() const noexcept {
    return 'F';
}