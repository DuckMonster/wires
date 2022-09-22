#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include "tga.h"

Console console;

void Console::load_glyph_atlas(const char* path)
{

}

void Console::open(u32 cols, u32 rows)
{
	this->cols = cols;
	this->rows = rows;

	window = SDL_CreateWindow("Console Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, cols * GLYPH_W * GLYPH_SCALE, rows * GLYPH_H * GLYPH_SCALE, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Tga_File glyph_file;
	glyph_file.load("font.tga");

	atlas_cols = glyph_file.width / (GLYPH_W + GLYPH_PADDING);

	glyph_atlas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, glyph_file.width, glyph_file.height);
	SDL_UpdateTexture(glyph_atlas, NULL, glyph_file.data, glyph_file.width * glyph_file.channels);
	SDL_SetTextureBlendMode(glyph_atlas, SDL_BLENDMODE_BLEND);

	glyph_file.free();
	glyphs = new Glyph[cols * rows];
}

void Console::close()
{
	if (should_close)
		return;

	SDL_DestroyWindow(window);
	should_close = true;
}

void Console::update()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				for(i32 i = num_key_handlers - 1; i >= 0; --i)
				{
					bool result = key_handler_stack[i](event.key.keysym.scancode, true, !!event.key.repeat);
					if (result)
						break;
				}

				break;

			case SDL_KEYUP:
				for(i32 i = num_key_handlers - 1; i >= 0; --i)
				{
					bool result = key_handler_stack[i](event.key.keysym.scancode, false, false);
					if (result)
						break;
				}

				break;

			case SDL_QUIT:
				should_close = true;
				break;
		}
	}

	SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 35, 35, 255);
	Glyph* glyph = glyphs;

	for(u32 r = 0; r < rows; ++r)
	{
		for(u32 c = 0; c < cols; ++c)
		{
			u32 atlas_x = glyph->chr % atlas_cols;
			u32 atlas_y = glyph->chr / atlas_cols;

			SDL_Rect dst_rect = {
				(int)c * GLYPH_W * GLYPH_SCALE,
				(int)r * GLYPH_H * GLYPH_SCALE,
				GLYPH_W * GLYPH_SCALE,
				GLYPH_H * GLYPH_SCALE
			};
			SDL_Rect src_rect = {
				(int)atlas_x * (GLYPH_W + GLYPH_PADDING),
				(int)atlas_y * (GLYPH_H + GLYPH_PADDING),
				GLYPH_W,
				GLYPH_H
			};

			// Background
			set_color(glyph->fg, glyph->bg);
			SDL_RenderFillRect(renderer, &dst_rect);

			// Foreground
			SDL_RenderCopy(renderer, glyph_atlas, &src_rect, &dst_rect);

			glyph++;
		}
	}

	SDL_RenderPresent(renderer);
}

void Console::clear()
{
	memset(glyphs, 0, sizeof(Glyph) * cols * rows);
}

void Console::set_color(u32 fg, u32 bg)
{
	SDL_SetRenderDrawColor(renderer, (bg >> 16) & 0xFF, (bg >> 8) & 0xFF, (bg) & 0xFF, 255);
	SDL_SetTextureColorMod(glyph_atlas, (fg >> 16) & 0xFF, (fg >> 8) & 0xFF, (fg) & 0xFF);
}

void Console::write_string(Point point, const char* string, u32 fg, u32 bg)
{
	while(*string)
	{
		set_glyph(point, *string, fg, bg);
		point.x++;
		string++;
	}
}