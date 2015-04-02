#include <iostream>

#include "../external/jsoncpp/json/json-forwards.h"
#include "../external/jsoncpp/json/json.h"

#include "game_main.h"

/*
 * Handles comand line arguments, initializes termbox and starts the game
 *
 */

int main()
{
	// Initialize
	int init_status = tb_init();
	if(init_status != 0)
	{ 
		std::cerr << "tb_init() failed with error code " << init_status << std::endl; 
		return 1;
	}
	
	Game_main game;
	game.run();
}