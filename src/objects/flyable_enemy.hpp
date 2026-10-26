#pragma once

#include "moving_collisionable.hpp"

namespace biv {
    class FlyableEnemy : public MovingCollisionable {
        public:
            FlyableEnemy(const Coord& top_left, const int width, const int height, float period = 1000.0f);

            Rect get_rect() const noexcept override;
            Speed get_speed() const noexcept override;

            void process_horizontal_static_collision(Rect*) noexcept override;
            void process_mario_collision(Collisionable*) noexcept override;
            void process_vertical_static_collision(Rect*) noexcept override;
            
            void move_vertically() noexcept override;
    };
}