#include "level_type_block_grid.h"


Level_type_block_grid::Level_type_block_grid(block_grid_config config)
{
	
	this->level_status.lives = config.lives,
	this->level_status.score = config.score,
	this->level_status.result = OUT_OF_LIVES; // Just so its not uninitialized. It should be set before its read
	

	this->name = config.name;
	this->points_per_block = config.points_per_block;
	ball_in_play = false;
	
	
	
	for(int i = 0; i < config.number_of_rows; i++) // < 13
	{
		for(int j = 0; j < config.number_of_columns; j++) // < 4
		{
			int x = (j * (config.block_width + config.left_padding)) + config.starting_x;
			int y = (i * (config.block_height + config.top_padding)) + config.starting_y;
	
			Box* block = new Box(x, y, config.block_width, config.block_height, config.block_default_char, TB_DEFAULT, TB_DEFAULT);
	
			block->replace_string(0, 0, config.block_width, config.block_string, TB_DEFAULT, TB_DEFAULT);

			this->blocks.push_back(std::shared_ptr<Box>(block));
			
		}
	}
	


	border.reset(new Box(0, 0, full_width, full_height, 1, '#', TB_DEFAULT, TB_DEFAULT));
	paddle.reset(new Paddle(32, 20, 13, 1, '=', TB_DEFAULT, TB_DEFAULT));
	ball.reset(new Ball(3, 22, 1, -1, 'o', TB_DEFAULT, TB_DEFAULT));
	
}





Level_status Level_type_block_grid::run()
{

	// Game event loop	
	while(true)
	{	

		// Draw everthing
		tb_clear();
		
		draw_data();
		
		border->draw();
		paddle->draw();
		border->draw();

		for(blocks_it = blocks.begin(); blocks_it != blocks.end(); blocks_it++)
			(*blocks_it)->draw();
	

		if(ball_in_play)
			ball->draw();

		tb_present();




		// Handle input

		// Launch ball
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if(!ball_in_play)
			{
				srand (time(NULL));
				int ran = rand() % 100;
		
				int dx;
				if(ran > 50)
					dx = 1;
				if(ran <= 50)
					dx = -1;
							
				ball->dx = dx;
				ball->dy = -1;
			
				paddle->ball = false;
				ball_in_play = true;
				ball->move_to(paddle->ball_x(), paddle->get_y());
			}
	
		}
		// Move paddle left
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			// Check if we can move two (prefered), one or zero spaces
			bool left_collide_1 = will_collide(paddle, paddle->get_x() - 1, paddle->get_y());
			bool left_collide_2 = will_collide(paddle, paddle->get_x() - 2, paddle->get_y());

	
			if(!(left_collide_1 || left_collide_2))
				paddle->move_left(2);
			else if(!left_collide_1)
				paddle->move_left(1);
		}
		// Move paddle right
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			// Check if we can move two (prefered), one or zero spaces
			bool right_collide_1 = will_collide(paddle, paddle->get_x() + paddle->get_width(), paddle->get_y());
			bool right_collide_2 = will_collide(paddle, paddle->get_x() + paddle->get_width() + 1, paddle->get_y());
	
			if(!(right_collide_1 || right_collide_2))
				paddle->move_right(2);
			else if(!right_collide_1)
				paddle->move_right(1);
		}
		// Pause
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			if(pause_window())
			{
				level_status.result = QUIT;
			 	return level_status;
			}
	
		}
		// Quit
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if(quit_window())
			{
				level_status.result = QUIT;
			 	return level_status;
			}
		}
	







		// Ball collsion and movement
		if(ball_in_play)
		{
			bool hor_wall = false;
			bool ver_wall = false;

			// Check if its a horizontal wall, above or below
			if(will_collide(ball, ball->get_x(), ball->get_y() + ball->dy))
				hor_wall = true;
	
			// next, a ver wall, left or right
			if(will_collide(ball, ball->get_x() + ball->dx, ball->get_y()))
				ver_wall = true;
	
			
			if(will_collide(ball, ball->get_x() + ball->dx, ball->get_y() + ball->dy) || ver_wall || hor_wall)
			{
				// Delete all hit blocks from blocks, clear hit_blocks, incriments score
				delete_hit();
		
				// if its a corner (inside or outside), reverse both
				if((hor_wall && ver_wall) || (!hor_wall && !ver_wall))
				{
					ball->dx *= -1;
					ball->dy *= -1;
				}
				else if(hor_wall) // A horizontal wall (ball moving up or down to), reverse dy
				{
					ball->dy *= -1;
				}
				else if(ver_wall) // A ver wall (ball moving left or right to), reverse dy
				{
					ball->dx *= -1;
				}
			}


			// Check if it will hit the bottom
			if(ball_in_play && ball->get_y() == 22)
			{
				paddle->ball = true;
				ball_in_play = false;
				level_status.lives--;
				if(level_status.lives == 0)	// Check if game was lost
				{
					level_status.result = OUT_OF_LIVES;
					return level_status;
				}
			}

			ball->move(); // Does not re-draw untill top of next loop

		}





		// Check if the game was won
		if(blocks.size() == 0)
		{
			level_status.result = WON;
			return level_status;
		}





		sleep(tick);
	}


};


bool Level_type_block_grid::will_collide(std::shared_ptr<Drawable> object, int x, int y)
{
	if(object != paddle && paddle->contains_point(x, y))
		return true;
	
	if(object != ball && ball->contains_point(x, y))
		return true;
	
	
	if(border->contains_point(x, y))
		return true;
	
	
	for(blocks_it = blocks.begin(); blocks_it != blocks.end(); blocks_it++)
	{
		if((*blocks_it)->contains_point(x,y))
		{
			hit_blocks.insert(*blocks_it);
			
			return true;
		}
	}
	
	return false;
};




void Level_type_block_grid::delete_hit()
{
	for(hit_it = hit_blocks.begin(); hit_it != hit_blocks.end(); hit_it++)
	{
		level_status.score += points_per_block;	
		blocks.remove(*hit_it);		
	}
	hit_blocks.clear();
};

void Level_type_block_grid::draw_data()
{
	draw_string(5, 1, 45, name, TB_DEFAULT, TB_DEFAULT);
	draw_string(50, 1, 10, "Lives: " + std::to_string(level_status.lives), TB_DEFAULT, TB_DEFAULT);
	draw_string(65, 1, 15, "Score: " + std::to_string(level_status.score), TB_DEFAULT, TB_DEFAULT);
}