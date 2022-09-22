#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "editor.h"

bool key_event(SDL_Scancode key, bool pressed, bool repeat)
{
	if (!pressed)
		return false;

	if (key == SDL_SCANCODE_ESCAPE)
	{
		console.close();
		return true;
	}

	return false;
}

void main()
{
	console.open(75, 40);
	console.push_key_handler(key_event);

	editor.enter();

	while(console.is_open())
	{
		console.clear();

		editor.step();
		editor.draw();

		console.update();
	}
}