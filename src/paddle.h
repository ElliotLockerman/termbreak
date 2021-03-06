#include "termbox.h"

#include "box.h"

#pragma once



class Paddle : public Box
{
private:
    tb_cell ball_cell;

public:
    bool ball;
    
    
    Paddle(int x, int y, int width, int height, 
        uint32_t cch, uint16_t cfg, uint16_t cbg) : 
        Box(x, y, width, height, cch, cfg, cbg)
        {
            assert(x > 0);
            assert(y > 0);
            assert(x + width <= tb_width());
            assert(y + height <= tb_height());
                
            ball = true;
            
            ball_cell.ch = 'o';
            ball_cell.fg = cfg;
            ball_cell.bg = cbg;
            
        }
    
    // Drawable implementations. See drawable.h
    using Drawable::draw;
    virtual void draw();
    
    // Get the ball's x position
    int ball_x();

    
    // Moving paddle
    void move_right(int dx);
    void move_left(int dx);
    
    // For launching a new ball
    void show_ball();
    void hide_ball();
};
