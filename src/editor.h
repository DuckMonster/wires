#pragma once
#include "input.h"
#include "circuit.h"

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

	i32 cursor_x = 0;
	i32 cursor_y = 0;
	void set_cursor(i32 x, i32 y);

	// Connection stuff
	void begin_connection(Node* source) { connection_src = source; }
	bool is_building_connection() { return connection_src != nullptr; }

	Node* connection_src = nullptr;
};
extern Editor editor;