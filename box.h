#include <vector>

#include "drawable.h"

/*
A box drawn to the terminal, consisting of a single character ch.
x, y are the upper left corner
Extra lines for thicknesses greater than 1 are placed on the *inside* of the width and height, replacing any center
For information on ch, fg and bg, see termbox.h
*/

class Box: public Drawable
{
private:
	int width, height, border_thickness;
	bool has_border, has_center;
	
	struct char_wrap
	{
		tb_cell cell;	
		bool empty; 
	} empty_char, border_cell, center_cell;
	
	 // Outer is column/width (x), inner is row/height (y)
	std::vector<std::vector<char_wrap*> > matrix; 
	std::vector<char_wrap> specials;

	void initialize_matrix();


public:
	// Border only
	Box(int x, int y, int width, int height, int border_thickness, uint32_t bch, uint16_t bfg, uint16_t bbg);

	
	// Center only
	Box(int x, int y, int width, int height, uint32_t cch, uint16_t cfg, uint16_t cbg);
	
	// Both border and center
	Box(int x, int y, int width, int height, int border_thickness, uint32_t bch, uint16_t bfg, uint16_t bbg, uint32_t cch, uint16_t cfg, uint16_t cbg);


	
	
	// Replaces the char at the given location with the arguments.
	// Make removed true to make the char no longer collidable
	void replace_char(int x, int y, uint32_t ch, uint16_t fg, uint16_t bg, bool removed);
	
	
	// Drawable implementations. See drawable.h
	using Drawable::draw;
	virtual void draw ();
	
	using Drawable::contains_point;
	virtual bool contains_point(int x, int y);
	
	using Drawable::move_to;
	virtual void move_to(int x, int y);
	
	
};
