#pragma once

#include <vector>
#include <SDL.h>

class InputHandler;
class Renderer;
class Cloth;

struct Application
{
private:
	Renderer* renderer = nullptr;
	InputHandler* inputHandler = nullptr;
	Cloth* cloth = nullptr;

	bool isRunning = false;
	bool drawPoints = false;
	bool drawSticks = true;

	Uint32 lastUpdateTime;
public:
	Application() = default;
	~Application() = default;

	bool IsRunning() const;

	void Setup(int clothWidth, int clothHeight, int clothSpacing);
	void Input();
	void Update();
	void Render() const;
	void Destroy();
};