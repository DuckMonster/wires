#include "circuit.h"
#include "console.h"
#include "palette.h"

void Circuit::step()
{
	Array<Part*> parts_to_evaluate = evaluation_queue;
	evaluation_queue.empty();

	for(Part* part : parts_to_evaluate)
		part->evaluate();
}

void Circuit::draw()
{
	// Draw background blueprint
	for(u32 row = 0; row < console.rows; ++row)
	{
		for(u32 col = 0; col < console.cols; ++col)
		{
			char glyph = ' ';
			if ((col * row) % 5 == 0)
				glyph = '.';

			console.set_glyph(Point(col, row), glyph, PAL_BLUEPRINT_FG, PAL_BLUEPRINT_BG);
		}
	}

	// --- Draw parts ---
	for(Part* part : parts)
	{
		Glyph& glyph = console.get_glyph(part->position);
		glyph.chr = part->chr;
		glyph.fg = part->state ? PAL_PART_ACTIVE : PAL_PART_INACTIVE;
	}

	// Nodes
}

void Circuit::draw_wire(Point from, Point to)
{
	/*
	i32 delta_x = x2 - x1;
	i32 delta_y = y2 - y1;

	if (Math::abs(delta_x) > Math::abs(delta_y))
	{
		i32 min = Math::min(x1, x2);
		i32 max = Math::max(x1, x2);

		for(i32 x = min + 1; x < max; ++x)
		{
			Glyph& glyph = console.get_glyph(x, y1);
			glyph.chr = '-';
			glyph.fg = 0x808080;
		}
	}
	else
	{
		i32 min = Math::min(y1, y2);
		i32 max = Math::max(y1, y2);

		for(i32 y = min + 1; y < max; ++y)
		{
			Glyph& glyph = console.get_glyph(x1, y);
			glyph.chr = '|';
			glyph.fg = 0x808080;
		}
	}
	*/
}