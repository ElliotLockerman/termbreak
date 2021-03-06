#include <cassert>

#include "termbox.h"

#include "drawable.h"
#include "constants.h"

#pragma once


/*
 * The Ball
 * One cell drawable with velocity
 */


class Ball : public Drawable
{
private:
    
    tb_cell cell;
    
public:
    // Distance moved in each direction each time move() is called. 
    int dy, dx;
    
    Ball(int x, int y, int dx, int dy, uint32_t ch, uint16_t fg, uint16_t bg);
    
    // Moves the ball by one dx and one dy
    void move();
    
    
    // Drawable implementations. See drawable.h
    using Drawable::draw;
    virtual void draw();
    
    // X and Y are absolute
    using Drawable::contains_point;
    virtual bool contains_point(int x, int y);
    
    // X and Y are absolute
    using Drawable::move_to;
    virtual void move_to(int x, int y);
    
    
    using Drawable::get_x;
    virtual int get_x();
    
    using Drawable::get_y;
    virtual int get_y();
};