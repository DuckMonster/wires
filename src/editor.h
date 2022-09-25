#pragma once
#include "input.h"
#include "circuit.h"

enum Editor_Mode
{
	MODE_Normal,
	MODE_Connection,
	MODE_Box,
};

class Editor
{
public:
	void enter();

	void step();
	void draw();

private:
	friend bool handle_key_wrapper(SDL_Scancode key, bool pressed, bool repeat);
	bool handle_key(SDL_Scancode key, bool pressed, bool repeat);

	Circuit circuit;

	Editor_Mode mode = MODE_Normal;
	Point cursor;
	void set_cursor(Point position);

	// Spawning
	void create_node(Point at);
	void create_inverter(Point at);

	// Connection stuff
	void begin_connection(Node* source)
	{
		connection_src = source;
		mode = MODE_Connection;
	}
	void end_connection()
	{
		connection_src = nullptr;
		mode = MODE_Normal;
	}

	Node* connection_src = nullptr;

	// Box select
	void begin_box_select(Point origin)
	{
		box_origin = origin;
		mode = MODE_Box;
	}
	void end_box()
	{
		mode = MODE_Normal;
	}

	Point box_origin;

	// Debug
	bool debug_networks = false;
};
extern Editor editor;