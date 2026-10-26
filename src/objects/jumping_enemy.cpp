#include "jumping_enemy.hpp"

#include "map_movable.hpp"

using biv::JumpingEnemy;

JumpingEnemy::JumpingEnemy(const Coord& top_left, const int width, const int height) 
	: RectMapMovableAdapter(top_left, width, height) {
	vspeed = 0;
	hspeed = 0;
}

biv::Rect JumpingEnemy::get_rect() const noexcept {
	return {top_left, width, height};
}

biv::Speed JumpingEnemy::get_speed() const noexcept {
	return {vspeed, hspeed};
}

void JumpingEnemy::process_horizontal_static_collision(Rect* obj) noexcept {
	hspeed = -hspeed;
	move_horizontally();
}

void JumpingEnemy::process_mario_collision(Collisionable* mario) noexcept {
	if (mario->get_speed().v > 0 && mario->get_speed().v != V_ACCELERATION) {
		kill();
	} else {
		mario->kill();
	}
}

void JumpingEnemy::process_vertical_static_collision(Rect* obj) noexcept {
	// Проверка: не свалился ли враг с корабля. 
	// Т.е., если он на краю, то он должен разверуться 
	// и побежать в обратную сторону.
	top_left.x += hspeed;
	if (!has_collision(obj)) {
		process_horizontal_static_collision(obj);
	} else {
		top_left.x -= hspeed;
	}
	
	// Особенность модели вертикального передвижения в игры
	if (vspeed > 0) {
		top_left.y -= vspeed;
		vspeed = 0;
	}
}

void JumpingEnemy::move_horizontally() noexcept {}

void JumpingEnemy::move_vertically() noexcept {
    if (vspeed == 0) {
        jump_timer += 1;
    }
    if (jump_timer >= JUMP_INTERVAL) {
        jump();
        jump_timer = 0;
    }
    if (vspeed < MAX_V_SPEED) {
        vspeed += V_ACCELERATION;

    }
    top_left.y += vspeed;
}