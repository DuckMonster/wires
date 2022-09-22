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

	Point cursor;
	void set_cursor(Point position);

	// Connection stuff
	void begin_connection(Node* source) { connection_src = source; }
	bool is_building_connection() { return connection_src != nullptr; }

	Node* connection_src = nullptr;
};
extern Editor editor;