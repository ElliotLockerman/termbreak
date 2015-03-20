#include <iostream>

#import "game_main.h"


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
	int status = game.run();

	tb_shutdown();
	return status;
 	
}