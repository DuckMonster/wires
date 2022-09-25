#include "editor.h"
#include "console.h"
#include "palette.h"
#include <stdio.h>

Editor editor;

static bool handle_key_wrapper(SDL_Scancode key, bool pressed, bool repeat) { return editor.handle_key(key, pressed, repeat); }
bool Editor::handle_key(SDL_Scancode key, bool pressed, bool repeat)
{
	// Cursor navigation
	if (pressed)
	{
		if (key == SDL_SCANCODE_H)
		{
			set_cursor(cursor.left());
			return true;
		}
		if (key == SDL_SCANCODE_J)
		{
			set_cursor(cursor.down());
			return true;
		}
		if (key == SDL_SCANCODE_K)
		{
			set_cursor(cursor.up());
			return true;
		}
		if (key == SDL_SCANCODE_L)
		{
			set_cursor(cursor.right());
			return true;
		}
	}

	// Part creation
	if (mode == MODE_Normal)
	{
		if (pressed)
		{
			if (key == SDL_SCANCODE_W)
			{
				create_node(cursor);
				return true;
			}
			if (key == SDL_SCANCODE_I)
			{
				create_inverter(cursor);
				return true;
			}

			if (key == SDL_SCANCODE_X)
			{
				Part* part = circuit.find_part_at(cursor);
				if (part)
					circuit.delete_part(part);
			}

			if (key == SDL_SCANCODE_V)
			{
				begin_box_select(cursor);
				return true;
			}
		}
	}
	if (mode == MODE_Connection)
	{
		if (pressed)
		{
			if (key == SDL_SCANCODE_ESCAPE)
			{
				end_connection();
				return true;
			}

			if (key == SDL_SCANCODE_W)
			{
				create_node(cursor);
				return true;
			}
		}
	}
	if (mode == MODE_Box)
	{
		if (pressed)
		{
			if (key == SDL_SCANCODE_ESCAPE || key == SDL_SCANCODE_V)
			{
				end_box();
				return true;
			}

			if (key == SDL_SCANCODE_X)
			{
			}
		}
	}

	// Debug
	if (pressed && !repeat)
	{
		if (key == SDL_SCANCODE_N)
		{
			debug_networks = !debug_networks;
			return true;
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
	if (mode == MODE_Connection)
	{
		circuit.draw_wire(connection_src->position, cursor, PAL_PART_INACTIVE);
	}

	// Debug draw networks
	if (debug_networks)
	{
		char buffer[8];
		for(u32 i = 0; i < circuit.networks.count(); ++i)
		{
			sprintf(buffer, "%u", i);
			for(auto* node : circuit.networks[i]->nodes)
			{
				console.write_string(node->position, buffer, 0x00FF00, 0x000000);
			}
		}

		char count_buffer[512];
		sprintf(count_buffer, "Num networks: %u", circuit.networks.count());
		console.write_string(Point(0, 0), count_buffer, 0xFFFFFF, 0x000000);
	}

	// Highlight cursor
	{
		Glyph& glyph = console.get_glyph(cursor);
		glyph.fg = PAL_CURSOR_FG;
		glyph.bg = PAL_CURSOR_BG;
	}

	if (mode == MODE_Box)
	{
		Point min = Point::component_min(cursor, box_origin);
		Point max = Point::component_max(cursor, box_origin);

		for(i32 y = min.y; y <= max.y; ++y)
		{
			for(i32 x = min.x; x <= max.x; ++x)
			{
				Glyph& glyph = console.get_glyph(Point(x, y));
				glyph.fg = PAL_CURSOR_FG;
				glyph.bg = PAL_CURSOR_BG;
			}
		}

		Glyph& glyph = console.get_glyph(cursor);
		glyph.fg = PAL_CURSOR_BG;
		glyph.bg = PAL_CURSOR_FG;
	}
}

void Editor::set_cursor(Point position)
{
	cursor = position;
}

void Editor::create_node(Point at)
{
	Part* part = circuit.find_part_at(at);
	if (part)
	{
		if (part->type == Node::static_type())
		{
			if (mode == MODE_Connection)
			{
				if (part == connection_src)
				{
					connection_src = nullptr;
					return;
				}

				Node* other = (Node*)part;
				connection_src->network->add_connection(connection_src, other);
				end_connection();
			}
			else
			{
				begin_connection((Node*)part);
			}
		}

		return;
	}

	Node* node = circuit.add_part<Node>(at);
	if (mode == MODE_Connection)
	{
		connection_src->network->add_connection(connection_src, node);
		end_connection();
	}
	else
	{
		begin_connection(node);
	}
}

void Editor::create_inverter(Point at)
{
	if (circuit.find_part_at(cursor) != nullptr)
		return;

	circuit.add_part<Inverter>(cursor);
}