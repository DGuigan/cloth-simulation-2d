#pragma once

#include <vector>
#include <SDL.h>

#include "Enums.h"
#include "LevelData.h"

class InputHandler;
class Renderer;
class Cloth;
class ScoreManager;
class LevelManager;
class TimeManager;

struct Application
{
private:
	Renderer* renderer = nullptr;
	InputHandler* inputHandler = nullptr;
	Cloth* cloth = nullptr;
	ScoreManager* scoreManager = nullptr;
	LevelManager* levelManager = nullptr;
	TimeManager* timeManager = nullptr;

	bool isRunning = false;
	bool drawPoints = true;
	bool drawSticks = true;

	Uint32 lastUpdateTime = 0;

	ApplicationMode applicationMode = ApplicationMode::Simulate;

public:
	Application() = default;
	~Application() = default;

	bool IsRunning() const;

	void Setup(int clothWidth, int clothHeight, int clothSpacing);
	void Reset();
	void Input();
	void Update();
	void Render() const;
	void Destroy();
};