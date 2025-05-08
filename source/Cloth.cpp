#include "Cloth.h"
#include "Stick.h"
#include "Renderer.h"
#include "Point.h"
#include "Fan.h"
#include "InputHandler.h"
#include <iostream>

Cloth::Cloth(int numColumns, int numRows, int spacing, int startX, int startY)
{
	CreateRectangularCloth(numColumns, numRows, spacing, startX, startY);

	fans.push_back(new Fan({ 350, 400 }, { 1, 0 }, 100, 30));
	fans.push_back(new Fan({ 1000, 500 }, { 0, -1 }, 100, 30));

}

void Cloth::CreateRectangularCloth(int numColumns, int numRows, int spacing, int startX, int startY)
{
	for (int y = 0; y <= numRows; y++) {
		for (int x = 0; x <= numColumns; x++)
		{
			Point* point = new Point(startX + x * spacing, startY + y * spacing);
			activePoints++;

			if (x != 0)
			{
				Point* leftPoint = points[this->points.size() - 1];
				Stick* s = new Stick(*point, *leftPoint, spacing);
				leftPoint->AddStick(s);
				point->AddStick(s);
				sticks.push_back(s);
			}

			if (y != 0)
			{
				Point* upPoint = points[x + (y - 1) * (numColumns + 1)];
				Stick* s = new Stick(*point, *upPoint, spacing);
				upPoint->AddStick(s);
				point->AddStick(s);
				sticks.push_back(s);
			}

			if (y == 0 && x % 2 == 0 && false)
			{
				point->SetPinned(true);
			}

			points.push_back(point);
		}
	}
}


void Cloth::Reset()
{
	for (Point* point : points)
	{
		delete point;
	}

	for (Stick* stick : sticks)
	{
		delete stick;
	}

	for (Fan* fan : fans)
	{
		delete fan;
	}

	points.clear();
	sticks.clear();
	fans.clear();

	closestSelectedPointIndex = -1;
	rightPointIndex = -1;
	leftPointIndex = -1;

	activePoints = 0;
}


void Cloth::Update(ApplicationMode applicationMode, InputHandler* inputHandler, Renderer* renderer, float deltaTime)
{
	//UpdateSelection(inputHandler);

	if (applicationMode == ApplicationMode::Simulate)
	{
		UpdateSimulation(renderer, inputHandler, deltaTime);
	}
	else if (applicationMode == ApplicationMode::Design)
	{
		UpdateDesign(inputHandler);
	}
}

void Cloth::UpdateSelection(InputHandler* inputHandler)
{
	closestSelectedPointIndex = -1;
	float closestSelectedPointDistance = 0.f;

	if (!inputHandler->GetLeftMouseButtonDown())
	{
		leftPointIndex = -1;
		rightPointIndex = -1;
	}

	for (int i = 0; i < points.size(); ++i)
	{
		const float distance = points[i]->UpdateSelection(inputHandler);

		if (distance > -1.f && (closestSelectedPointIndex == -1 || distance < closestSelectedPointDistance))
		{
			if (inputHandler->GetLeftMouseButtonDown())
			{
				leftPointIndex = rightPointIndex;
				rightPointIndex = i;
			}

			closestSelectedPointIndex = i;
			closestSelectedPointDistance = distance;

		}
	}
}

void Cloth::UpdateSimulation(Renderer* renderer, InputHandler* inputHandler, float deltaTime)
{
	for (int i = 0; i < points.size(); i++)
	{
		Point* point = points[i];
		point->Update(deltaTime, drag, gravity, elasticity, &fans, inputHandler, this, renderer->GetWindowWidth(), renderer->GetWindowHeight());
	}

	for (int i = 0; i < sticks.size(); i++)
	{
		sticks[i]->Update();
	}

	for (int i = 0; i < fans.size(); i++)
	{
		fans[i]->Update(inputHandler);
	}
}

void Cloth::UpdateDesign(InputHandler* inputHandler)
{
	if (inputHandler->GetLeftMouseButtonJustClicked())
	{
		const Vec2 position = inputHandler->GetMousePosition();

		Point* point = nullptr;

		if (closestSelectedPointIndex == -1)
		{
			point = new Point(position.x, position.y);
			points.push_back(point);
			activePoints++;
		}
		else
		{
			point = points[closestSelectedPointIndex];
		}

		if (inputHandler->GetLeftShiftDown())
		{
			point->SetPinned(!point->GetPinned());
		}
	}

	const bool validLeftPoint = leftPointIndex != -1 && leftPointIndex < points.size();
	const bool validRightPoint = rightPointIndex != -1 && rightPointIndex < points.size();
	const bool uniquePoints = leftPointIndex != rightPointIndex;
	const bool shouldConnectPoints = validLeftPoint && validRightPoint && uniquePoints;

	if (inputHandler->GetLeftMouseButtonDown() && shouldConnectPoints)
	{
		Point* leftPoint = points[leftPointIndex];
		Point* rightPoint = points[rightPointIndex];

		if (leftPoint && rightPoint)
		{
			float distance = Vec2::Distance(leftPoint->GetPosition(), rightPoint->GetPosition());
			Stick* stick = new Stick(*leftPoint, *rightPoint, distance);
			leftPoint->AddStick(stick);
			rightPoint->AddStick(stick);
			sticks.push_back(stick);
		}
	}

	if (inputHandler->GetFDown())
	{
		fans.push_back(new Fan(inputHandler->GetMousePosition()));
		inputHandler->SetFDown(false);
	}
}

void Cloth::Draw(Renderer* renderer, const bool drawPoints, const bool drawSticks) const
{
	if (drawPoints)
	{
		for (const Point* point : points)
		{
			point->Draw(renderer);
		}
	}

	if (drawSticks)
	{
		for (const Stick* stick : sticks)
		{
			stick->Draw(renderer);
		}
	}

	for (const Fan* fan : fans)
	{
		fan->Draw(renderer);
	}
}

void Cloth::OnPointRemoved()
{
	if (0 < activePoints)
	{
		activePoints--;
	}
	else
	{
		SDL_assert(false);
	}
}


Cloth::~Cloth()
{
	Reset();
}
