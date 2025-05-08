#include "LevelManager.h"

#include "Renderer.h"
#include "Application.h"
#include "Cloth.h"
#include "ScoreManager.h"

void LevelManager::InitLevels(const Renderer* renderer)
{
	{ // LEVEL 1
		LevelData& level1 = levelData.emplace_back();

		float clothWidth = 1200.f;
		float clothHeight = 320.f;
		float clothX = (renderer->GetWindowWidth() - clothWidth) * 0.5f;
		float clothY = renderer->GetWindowHeight() * 0.1f;

		level1.targetScore = 3000000;
		level1.clothData.push_back({ ClothType::Rectangular, 10.f, {clothWidth, clothHeight}, {clothX, clothY} });

		level1.fanData.push_back({ { 350, 400 }, { 1, 0 }, 100, 30 });
		level1.fanData.push_back({ { 1000, 500 }, { 0, -1 }, 100, 30 });
	}

	{ // LEVEL 2 
		LevelData& level2 = levelData.emplace_back();

		float clothWidth = 750.f;
		float clothHeight = 250.f;
		float clothX = (renderer->GetWindowWidth() - clothWidth) * 0.5f;
		float clothY = renderer->GetWindowHeight() * 0.1f;

		level2.targetScore = 3000000;
		level2.clothData.push_back({ ClothType::Rectangular, 10.f, {clothWidth, clothHeight}, {clothX, clothY} });

		level2.fanData.push_back({ { 350, 600 }, { 1, 1 }, 100, 30 });
		level2.fanData.push_back({ { 1000, 500 }, { 0, -1 }, 100, 30 });
	}
}

void LevelManager::LoadLevel(const int levelIndex, Application* application, Cloth* cloth, ScoreManager* scoreManager)
{
	application->Reset();

	currentLevelState = LevelState::Waiting;
	timeSpentWaitingForLevelStart = 0.f;
	timeSpentWaitingForLevelEnd = 0.f;

	if (levelIndex != -1)
	{
		currentLevelIndex = levelIndex;
	}

	const LevelData& level = levelData[currentLevelIndex];

	scoreManager->SetTargetScore(level.targetScore);

	for (const ClothData& clothData : level.clothData)
	{
		cloth->AddWeave(clothData);
	}

	for (const FanData& fanData : level.fanData)
	{
		cloth->AddFan(fanData);
	}
}

void LevelManager::Update(Application* application, Cloth* cloth, ScoreManager* scoreManager, const float deltaTime)
{
	if (currentLevelState == LevelState::InProgress)
	{
		if (scoreManager->GetTargetScoreReached())
		{
			currentLevelState = LevelState::Completed;
		}
		else if (cloth->GetNumActivePoints() == 0)
		{
			currentLevelState = LevelState::Failed;
		}
	}

	if (currentLevelState == LevelState::Waiting)
	{
		timeSpentWaitingForLevelStart += deltaTime;

		if (secondsBeforeLevelStart <= timeSpentWaitingForLevelStart)
		{
			currentLevelState = LevelState::InProgress;
		}
	}
	else if (currentLevelState == LevelState::Completed || currentLevelState == LevelState::Failed)
	{
		timeSpentWaitingForLevelEnd += deltaTime;

		if (secondsBeforeLevelEnd <= timeSpentWaitingForLevelEnd)
		{
			if (currentLevelState == LevelState::Completed)
			{
				currentLevelIndex = (currentLevelIndex + 1) % levelData.size();
			}

			LoadLevel(currentLevelIndex, application, cloth, scoreManager);
		}
	}
}
