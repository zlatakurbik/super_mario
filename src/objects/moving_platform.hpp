#pragma once

#include "moving_collisionable.hpp"
#include "rect.hpp"
#include "rect_map_movable_adapter.hpp"
#include "speed.hpp"

namespace biv {
	class MovingPlatform : virtual public RectMapMovableAdapter, virtual public MovingCollisionable{
		private:
			float period;
			int counter;
		public:
			MovingPlatform(const Coord& top_left, const int width, const int height);

			Rect get_rect() const noexcept override;
			Speed get_speed() const noexcept override;

			void process_horizontal_static_collision(Rect*) noexcept override;
			void process_vertical_object_collision(Collisionable*) noexcept;
			void move_vertically() noexcept override;
			void move_horizontally() noexcept override;
	};
}