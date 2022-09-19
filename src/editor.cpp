#include "editor.h"
#include "console.h"

Editor editor;

static bool handle_key_wrapper(SDL_Scancode key, bool pressed, bool repeat) { return editor.handle_key(key, pressed, repeat); }
bool Editor::handle_key(SDL_Scancode key, bool pressed, bool repeat)
{
	// Cursor navigation
	if (pressed)
	{
		if (key == SDL_SCANCODE_H)
		{
			set_cursor(cursor_x - 1, cursor_y);
			return true;
		}
		if (key == SDL_SCANCODE_J)
		{
			set_cursor(cursor_x, cursor_y + 1);
			return true;
		}
		if (key == SDL_SCANCODE_K)
		{
			set_cursor(cursor_x, cursor_y - 1);
			return true;
		}
		if (key == SDL_SCANCODE_L)
		{
			set_cursor(cursor_x + 1, cursor_y);
			return true;
		}
	}

	// Part creation
	if (pressed && !repeat)
	{
		if (!is_building_connection())
		{
			bool space_empty = (circuit.find_part_at(cursor_x, cursor_y) == nullptr);
			if (space_empty)
			{
				if (key == SDL_SCANCODE_W)
				{
					Node* node = circuit.add_node(cursor_x, cursor_y);
					begin_connection(node);
					return true;
				}

				if (key == SDL_SCANCODE_I)
				{
					circuit.add_inverter(cursor_x, cursor_y);
					return true;
				}
			}
		}
		else
		{
			if (key == SDL_SCANCODE_W)
			{
				Node* target = circuit.add_node(cursor_x, cursor_y);
				circuit.connect_nodes(connection_src, target);

				connection_src = nullptr;
			}
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
		circuit.draw_wire(connection_src->x, connection_src->y, cursor_x, cursor_y);
	}

	// Highlight cursor
	Glyph& glyph = console.get_glyph(cursor_x, cursor_y);
	glyph.fg = 0x252525;
	glyph.bg = 0xEEEEEE;
}

void Editor::set_cursor(i32 x, i32 y)
{
	cursor_x = x;
	cursor_y = y;
}