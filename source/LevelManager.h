#pragma once

#include "LevelData.h"
#include "Enums.h"
#include "SDL.h"

#include <vector>

class Renderer;
class Application;
class Cloth;
class ScoreManager;

class LevelManager
{
private:
	std::vector<LevelData> levelData;

	int currentLevelIndex = 0;
	LevelState currentLevelState = LevelState::InProgress;

	float timeSpentWaitingForLevelStart = 0.f;
	float timeSpentWaitingForLevelEnd = 0.f;
	float secondsBeforeLevelStart = 3.f;
	float secondsBeforeLevelEnd = 3.f;

	Uint32 greenColor = 0x8CFC03;
	Uint32 orangeColor = 0xFCE803;
	Uint32 redColor = 0xFC1C03;
	Uint32 blueColor = 0xFF0048E3;

public:
	void InitLevels(const Renderer* renderer);

	void LoadLevel(const int levelIndex, Application* application, Cloth* cloth, ScoreManager* scoreManager);

	void Update(Application* application, Cloth* cloth, ScoreManager* scoreManager, const float deltaTime);

	LevelState GetCurrentLevelState() const { return currentLevelState; };
};