#include "third_level.hpp"

using biv::ThirdLevel;

ThirdLevel::ThirdLevel(UIFactory* ui_factory) : GameLevel(ui_factory) {
	init_data();
}

bool ThirdLevel::is_final() const noexcept {
	return true;
}

biv::GameLevel* ThirdLevel::get_next() {
	return next;
}

void ThirdLevel::init_data() {
	ui_factory->create_mario({20, 10}, 3, 3);
	
    ui_factory->create_ship({0, 10}, 3, 2);
    ui_factory->create_ship({5, 15}, 3, 2);
    ui_factory->create_ship({10, 20}, 3, 2);

	ui_factory->create_ship({20, 25}, 6, 4);
    ui_factory->create_ship({30, 20}, 6, 4);
    ui_factory->create_ship({50, 20}, 6, 4);
    ui_factory->create_ship({60, 25}, 5, 4);
    ui_factory->create_ship({70, 20}, 5, 4);
    ui_factory->create_ship({80, 25}, 5, 4);
    ui_factory->create_ship({100, 25}, 6, 3);
    ui_factory->create_ship({120, 25}, 7, 5);


    ui_factory->create_moving_platform({130, 20}, 12, 2);
    
    ui_factory->create_jumping_enemy({20, 24}, 3, 2);
    ui_factory->create_jumping_enemy({30, 19}, 3, 2);
    ui_factory->create_jumping_enemy({40, 24}, 3, 2);
    ui_factory->create_jumping_enemy({80, 24}, 3, 2);
    ui_factory->create_jumping_enemy({90, 19}, 3, 2);
    ui_factory->create_jumping_enemy({100, 24}, 3, 2);
    ui_factory->create_jumping_enemy({110, 19}, 3, 2);

    ui_factory->create_flyable_enemy({185, 15}, 3, 2);
    ui_factory->create_flyable_enemy({207, 18}, 3, 2);

    ui_factory->create_ship({175, 15}, 10, 7);

    ui_factory->create_enemy({175, 12}, 3, 2);

    ui_factory->create_ship({185, 24}, 25, 3);

    ui_factory->create_ship({210, 15}, 15, 7);
}