#include "Cloth.h"
#include "Stick.h"
#include "Renderer.h"
#include "Point.h"
#include "InputHandler.h"

Cloth::Cloth(int numColumns, int numRows, int spacing, int startX, int startY)
{
	for (int y = 0; y <= numRows; y++) {
		for (int x = 0; x <= numColumns; x++)
		{
			Point* point = new Point(startX + x * spacing, startY + y * spacing);

			if (x != 0)
			{
				Point* leftPoint = points[this->points.size() - 1];
				Stick* s = new Stick(*point, *leftPoint, spacing);
				leftPoint->AddStick(s, 0);
				point->AddStick(s, 0);
				sticks.push_back(s);
			}

			if (y != 0)
			{
				Point* upPoint = points[x + (y - 1) * (numColumns + 1)];
				Stick* s = new Stick(*point, *upPoint, spacing);
				upPoint->AddStick(s, 1);
				point->AddStick(s, 1);
				sticks.push_back(s);
			}

			if (y == 0 && x % 2 == 0)
			{
				point->Pin();
			}

			points.push_back(point);
		}
	}
}

void Cloth::Update(Renderer* renderer, InputHandler* inputHandler, float deltaTime)
{
	for (int i = 0; i < points.size(); i++)
	{
		Point* point = points[i];
		point->Update(deltaTime, drag, gravity, elasticity, inputHandler, renderer->GetWindowWidth(), renderer->GetWindowHeight());
	}

	for (int i = 0; i < sticks.size(); i++)
	{
		sticks[i]->Update();
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
}

Cloth::~Cloth()
{
	for (auto point : points)
	{
		delete point;
	}

	for (auto stick : sticks)
	{
		delete stick;
	}
}
