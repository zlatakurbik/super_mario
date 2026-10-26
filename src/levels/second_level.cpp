#include "second_level.hpp"
#include "third_level.hpp"

using biv::SecondLevel;

SecondLevel::SecondLevel(UIFactory* ui_factory) : GameLevel(ui_factory) {
	init_data();
}

biv::GameLevel* SecondLevel::get_next() {
	if (!next) {
		clear_data();
		next = new biv::ThirdLevel(ui_factory);
	}
	return next;
}

void SecondLevel::init_data() {
	ui_factory->create_mario({39, 10}, 3, 3);
	
	ui_factory->create_ship({20, 25}, 40, 2);
	ui_factory->create_ship({60, 20}, 10, 7);
	ui_factory->create_ship({80, 25}, 20, 2);
	ui_factory->create_ship({120, 20}, 10, 7);
	ui_factory->create_ship({150, 25}, 40, 2);
	ui_factory->create_ship({210, 20}, 10, 7);

	ui_factory->create_flyable_enemy({200, 20}, 3, 2);
	ui_factory->create_jumping_enemy({20, 10}, 3, 2);
	ui_factory->create_moving_platform({30, 20}, 7, 2);
}