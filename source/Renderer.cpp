#include <iostream>
#include "Renderer.h"


bool Renderer::Setup()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Error initializing SDL" << std::endl;
		return false;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = displayMode.w;
	windowHeight = displayMode.h;

	window = SDL_CreateWindow(NULL, 0, 0, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	if (!window)
	{
		std::cerr << "Error initializing SDL window" << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		std::cerr << "Error initializing SDL renderer" << std::endl;
		return false;
	}

	return true;
}

void Renderer::ClearScreen(Uint32 color) const
{
	SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);
	SDL_RenderClear(renderer);
}

void Renderer::Render() const
{
	SDL_RenderPresent(renderer);
}

void Renderer::DrawLine(int x0, int y0, int x1, int y1, Uint32 color) const
{
	SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);
	SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}

void Renderer::DrawLine(Vec2 start, Vec2 end, Uint32 color) const
{
	DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), color);
}

void Renderer::DrawPoint(int x, int y, Uint32 color) const
{
	SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);
	SDL_RenderDrawPoint(renderer, x, y);

	SDL_FRect rect;
	static const float halfLength = 2.5f;
	static const float length = halfLength * 2.f;
	rect.x = x - halfLength;
	rect.y = y - halfLength;
	rect.w = length;
	rect.h = length;
	SDL_RenderDrawRectF(renderer, &rect);
}

void Renderer::DrawPoint(Vec2 position, Uint32 color) const
{
	DrawPoint(static_cast<int>(position.x), static_cast<int>(position.y), color);
}

void Renderer::DrawRect(Vec2 position, Vec2 dimensions, Uint32 color, const bool fill) const
{
	SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);

	SDL_FRect rect;
	rect.x = position.x;
	rect.y = position.y;
	rect.w = dimensions.x;
	rect.h = dimensions.y;
	if (fill)
	{
		SDL_RenderFillRectF(renderer, &rect);
	}
	else
	{
		SDL_RenderDrawRectF(renderer, &rect);
	}
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}