#pragma once

#include <SDL.h>
#include "Vec2.h"

class Renderer {
private:
	int windowWidth, windowHeight;
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	Renderer() = default;
	~Renderer();

	int GetWindowWidth() const { return windowWidth; }
	int GetWindowHeight() const { return windowHeight; }

	bool Setup();

	void ClearScreen(Uint32 color) const;
	void Render() const;

	void DrawLine(int x0, int y0, int x1, int y1, Uint32 color) const;
	void DrawLine(Vec2 start, Vec2 end, Uint32 color) const;
	void DrawPoint(int x, int y, Uint32 color) const;
	void DrawPoint(Vec2 position, Uint32 color) const;
	void DrawRect(Vec2 position, Vec2 dimensions, Uint32 color, const bool fill) const;
};