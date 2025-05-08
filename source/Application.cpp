#include <iostream>
#include "Application.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "Cloth.h"
#include "ScoreManager.h"
#include "LevelData.h"
#include "LevelManager.h"
#include "TimeManager.h"

void Application::Setup(int clothWidth, int clothHeight, int clothSpacing)
{
	renderer = new Renderer();
	isRunning = renderer->Setup();

	inputHandler = new InputHandler();
	levelManager = new LevelManager();
	scoreManager = new ScoreManager({ 100, 500 }, { 50, 300 });
	timeManager = new TimeManager({ static_cast<float>(renderer->GetWindowWidth() - 150), 500 }, { 50, 300 });
	cloth = new Cloth();

	levelManager->InitLevels(renderer);

	levelManager->LoadLevel(0, this, cloth, scoreManager, timeManager);

	lastUpdateTime = SDL_GetTicks();
}

void Application::Reset()
{
	cloth->Reset();
	scoreManager->Reset();
	timeManager->Reset();
}

void Application::Input()
{
	SDL_Event event;

	inputHandler->OnFrameStart();

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
		{
			isRunning = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case (SDLK_ESCAPE):
			{
				isRunning = false;
				break;
			}
			case (SDLK_DELETE):
			{
				Reset();
				break;
			}
			case (SDLK_1):
			{
				drawPoints = !drawPoints;
				break;
			}
			case (SDLK_2):
			{
				drawSticks = !drawSticks;
				break;
			}
			case (SDLK_RETURN):
			{
				if (applicationMode != ApplicationMode::Simulate)
				{
					applicationMode = ApplicationMode::Simulate;
				}
				else
				{
					applicationMode = ApplicationMode::Design;
				}
				break;
			}
			case (SDLK_LCTRL):
			{
				inputHandler->SetLeftCtrlDown(true);
				break;
			}
			case (SDLK_LSHIFT):
			{
				inputHandler->SetLeftShiftDown(true);
				break;
			}
			case (SDLK_RIGHT):
			{
				inputHandler->SetArrowKeyState(0, 1.f);
				break;
			}
			case (SDLK_d):
			{
				inputHandler->SetArrowKeyState(0, 1.f);
				break;
			}
			case (SDLK_LEFT):
			{
				inputHandler->SetArrowKeyState(0, -1.f);
				break;
			}
			case (SDLK_a):
			{
				inputHandler->SetArrowKeyState(0, -1.f);
				break;
			}
			case(SDLK_UP):
			{
				inputHandler->SetArrowKeyState(1, 1.f);
				break;
			}
			case(SDLK_w):
			{
				inputHandler->SetArrowKeyState(1, 1.f);
				break;
			}
			case(SDLK_DOWN):
			{
				inputHandler->SetArrowKeyState(1, -1.f);
				break;
			}
			case(SDLK_s):
			{
				inputHandler->SetArrowKeyState(1, -1.f);
				break;
			}
			case(SDLK_f):
			{
				inputHandler->SetFDown(true);
				break;
			}
			case(SDLK_EQUALS):
			{
				inputHandler->SetPlusDown(true);
				break;
			}
			case(SDLK_MINUS):
			{
				inputHandler->SetMinusDown(true);
				break;
			}
			case(SDLK_r):
			{
				levelManager->LoadLevel(-1, this, cloth, scoreManager, timeManager);
				break;
			}
			}
			break;
		}
		case SDL_KEYUP:
		{
			switch (event.key.keysym.sym)
			{
			case (SDLK_LCTRL):
			{
				inputHandler->SetLeftCtrlDown(false);
				break;
			}
			case (SDLK_LSHIFT):
			{
				inputHandler->SetLeftShiftDown(false);
				break;
			}
			case (SDLK_RIGHT):
			{
				inputHandler->SetArrowKeyState(0, 0.f);
				break;
			}
			case (SDLK_d):
			{
				inputHandler->SetArrowKeyState(0, 0.f);
				break;
			}
			case (SDLK_LEFT):
			{
				inputHandler->SetArrowKeyState(0, 0.f);
				break;
			}
			case (SDLK_a):
			{
				inputHandler->SetArrowKeyState(0, 0.f);
				break;
			}
			case(SDLK_UP):
			{
				inputHandler->SetArrowKeyState(1, 0.f);
				break;
			}
			case(SDLK_w):
			{
				inputHandler->SetArrowKeyState(1, 0.f);
				break;
			}
			case(SDLK_DOWN):
			{
				inputHandler->SetArrowKeyState(1, 0.f);
				break;
			}
			case(SDLK_s):
			{
				inputHandler->SetArrowKeyState(1, 0.f);
				break;
			}
			case(SDLK_f):
			{
				inputHandler->SetFDown(false);
				break;
			}
			case(SDLK_EQUALS):
			{
				inputHandler->SetPlusDown(false);
				break;
			}
			case(SDLK_MINUS):
			{
				inputHandler->SetMinusDown(false);
				break;
			}
			}
			break;
		}
		case SDL_MOUSEMOTION:
		{
			int x = event.motion.x;
			int y = event.motion.y;
			inputHandler->UpdateMousePosition(x, y);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			inputHandler->UpdateMousePosition(x, y);

			if (!inputHandler->GetLeftMouseButtonDown() && event.button.button == SDL_BUTTON_LEFT)
			{
				inputHandler->SetLeftMouseButtonDown(true);
			}
			if (!inputHandler->GetRightMouseButtonDown() && event.button.button == SDL_BUTTON_RIGHT)
			{
				inputHandler->SetRightMouseButtonDown(true);
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if (inputHandler->GetLeftMouseButtonDown() && event.button.button == SDL_BUTTON_LEFT)
			{
				inputHandler->SetLeftMouseButtonDown(false);
			}
			if (inputHandler->GetRightMouseButtonDown() && event.button.button == SDL_BUTTON_RIGHT)
			{
				inputHandler->SetRightMouseButtonDown(false);
			}
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			if (event.wheel.y > 0)
			{
				inputHandler->IncreaseCursorSize(10);
			}
			else if (event.wheel.y < 0)
			{
				inputHandler->IncreaseCursorSize(-10);
			}
			break;
		}
		}
	}
}

void Application::Update()
{
	Uint32 currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;
	lastUpdateTime = currentTime;

	if (levelManager->GetCurrentLevelState() == LevelState::InProgress)
	{
		cloth->Update(applicationMode, inputHandler, renderer, deltaTime);

		scoreManager->IncrementCurrentScore(cloth->GetNumActivePoints());

		timeManager->Update(deltaTime);
	}

	levelManager->Update(this, cloth, scoreManager, timeManager, deltaTime);

}

void Application::Render() const
{
	renderer->ClearScreen(0xFF000816);

	cloth->Draw(renderer, levelManager, drawPoints, drawSticks);

	scoreManager->Draw(renderer, levelManager);

	timeManager->Draw(renderer, levelManager);

	renderer->Render();
}


bool Application::IsRunning() const
{
	return isRunning;
}

void Application::Destroy()
{
	delete inputHandler;
	delete renderer;
	delete cloth;
	delete scoreManager;
	delete levelManager;
	delete timeManager;
}
