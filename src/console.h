#pragma once
#include "input.h"

#define GLYPH_W 6
#define GLYPH_H 9
#define GLYPH_PADDING 1
#define GLYPH_SCALE 2

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

struct Glyph
{
	char chr = 0;
	u32 fg = 0xFFFFFF;
	u32 bg = 0x232323;
};

class Console
{
public:
	void open(u32 cols, u32 rows);
	void close();
	bool is_open() { return !should_close; }
	void update();

	void clear();

	void set_glyph(Point point, char chr, u32 fg, u32 bg) { glyphs[point.x + point.y * cols] = { chr, fg, bg }; }
	Glyph& get_glyph(Point point) { return glyphs[point.x + point.y * cols]; }

	void write_string(Point point, const char* string, u32 fg, u32 bg);

	void push_key_handler(key_handle_func func) { key_handler_stack[num_key_handlers++] = func; }
	void push_char_handler(char_handle_func func) { char_handler_stack[num_char_handlers++] = func; }

	u32 cols;
	u32 rows;

private:
	void set_color(u32 fg, u32 bg);
	void load_glyph_atlas(const char* path);

	key_handle_func key_handler_stack[6];
	u32 num_key_handlers = 0;
	char_handle_func char_handler_stack[6];
	u32 num_char_handlers = 0;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* glyph_atlas;
	u32 atlas_cols;

	bool should_close = false;
	Glyph* glyphs;
};

extern Console console;