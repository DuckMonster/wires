#include "editor.h"
#include "console.h"
#include "palette.h"

Editor editor;

static bool handle_key_wrapper(SDL_Scancode key, bool pressed, bool repeat) { return editor.handle_key(key, pressed, repeat); }
bool Editor::handle_key(SDL_Scancode key, bool pressed, bool repeat)
{
	// Cursor navigation
	if (pressed)
	{
		if (key == SDL_SCANCODE_H)
		{
			set_cursor(cursor + Point(-1, 0));
			return true;
		}
		if (key == SDL_SCANCODE_J)
		{
			set_cursor(cursor + Point(0, 1));
			return true;
		}
		if (key == SDL_SCANCODE_K)
		{
			set_cursor(cursor + Point(0, -1));
			return true;
		}
		if (key == SDL_SCANCODE_L)
		{
			set_cursor(cursor + Point(1, 0));
			return true;
		}
	}

	// Part creation
	if (pressed)
	{
		if (key == SDL_SCANCODE_W)
		{
			if (circuit.find_part_at(cursor) == nullptr)
				circuit.add_part<Node>(cursor);

			return true;
		}
		if (key == SDL_SCANCODE_I)
		{
			if (circuit.find_part_at(cursor) == nullptr)
				circuit.add_part<Inverter>(cursor);

			return true;
		}

		if (key == SDL_SCANCODE_X)
		{
			Part* part = circuit.find_part_at(cursor);
			if (part)
				circuit.delete_part(part);
		}
	}

	return false;
}

void Editor::enter()
{
	console.push_key_handler(handle_key_wrapper);
}

void Editor::step()
{
	circuit.step();
}

void Editor::draw()
{
	circuit.draw();

	// Draw connection building
	if (is_building_connection())
	{
		circuit.draw_wire(connection_src->position, cursor);
	}

	// Highlight cursor
	Glyph& glyph = console.get_glyph(cursor);
	glyph.fg = PAL_CURSOR_FG;
	glyph.bg = PAL_CURSOR_BG;
}

void Editor::set_cursor(Point position)
{
	cursor = position;
}