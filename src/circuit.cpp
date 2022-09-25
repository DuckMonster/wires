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

	// --- Draw networks ---
	for(Node_Network* network : networks)
	{
		for(auto& connection : network->connections)
		{
			draw_wire(connection.a->position, connection.b->position, network->state ? PAL_PART_ACTIVE : PAL_PART_INACTIVE);
		}
	}
}

void Circuit::draw_wire(Point from, Point to, u32 color)
{
	Point delta = to - from;

	if (Math::abs(delta.x) > Math::abs(delta.y))
	{
		i32 min = Math::min(from.x, to.x);
		i32 max = Math::max(from.x, to.x);

		for(i32 x = min + 1; x < max; ++x)
		{
			Glyph& glyph = console.get_glyph(Point(x, from.y));
			glyph.chr = '-';
			glyph.fg = color;
		}
	}
	else
	{
		i32 min = Math::min(from.y, to.y);
		i32 max = Math::max(from.y, to.y);

		for(i32 y = min + 1; y < max; ++y)
		{
			Glyph& glyph = console.get_glyph(Point(from.x, y));
			glyph.chr = '|';
			glyph.fg = color;
		}
	}
}