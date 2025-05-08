#pragma once

#include <vector>
#include "Vec2.h"
#include "Enums.h"
#include "SDL.h"

class InputHandler;
class Renderer;
class Point;
class Stick;
class Fan;
class LevelManager;
struct ClothData;
struct FanData;


class Cloth
{
private:
	Vec2 gravity = { 0.0f, 981.0f };
	float drag = 0.01f;
	float elasticity = 10.0f;

	std::vector<Point*> points;
	std::vector<Stick*> sticks;
	std::vector<Fan*> fans;

	int activePoints = 0;

	int closestSelectedPointIndex = -1;
	int leftPointIndex = -1;
	int rightPointIndex = 1;

	Uint32 defaultColor = 0xFF0048E3;
	Uint32 completedColor = 0x8CFC03;
	Uint32 failedColor = 0xFC1C03;

public:
	Cloth() = default;
	Cloth(int numColumns, int numRows, int spacing, int startX, int startY);
	~Cloth();

	void AddWeave(const ClothData& clothData);
	void AddFan(const FanData& fanData);
	void CreateRectangularCloth(float width, float height, int spacing, int startX, int startY);
	void Reset();
	void Update(ApplicationMode applicationMode, InputHandler* inputHandler, Renderer* renderer, float deltaTime);
	void UpdateSelection(InputHandler* inputHandler);
	void UpdateSimulation(Renderer* renderer, InputHandler* inputHandler, float deltaTime);
	void UpdateDesign(InputHandler* inputHandler);
	void Draw(Renderer* renderer, LevelManager* levelManager, const bool drawPoints, const bool drawSticks) const;
	void OnPointRemoved();
	int GetNumActivePoints() const {
		return activePoints;
	};
};
