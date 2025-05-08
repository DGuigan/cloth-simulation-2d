#pragma once

#include <vector>

#include "Vec2.h"
#include "Enums.h"

struct ClothData
{
	ClothType clothType = ClothType::Rectangular;
	float spacing = 0.f;
	Vec2 dimensions = { 0.f, 0.f };
	Vec2 position = { 0.f, 0.f };
};

struct FanData
{
	Vec2 position = { 0.f, 0.f };
	Vec2 direction = { 0.f, 0.f };
	float magnitude = 0.f;
	float angle = 0.f;
};

struct LevelData
{
public:
	LevelData() = default;

	int targetScore = 0;
	float timeLimit = 0.f;

	std::vector<ClothData> clothData;
	std::vector<FanData> fanData;
};