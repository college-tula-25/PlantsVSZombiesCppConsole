#pragma once
#include <chrono>
#include <vector>
#include "../Render/ConsoleRender.h"
#include "GridManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "GameState.h"

class GameEngine
{
private:
    const int screenWidth = 80;
    const int screenHeight = 25;

    ConsoleRender renderer;
    GridManager grid;
    ObjectManager objects;
    InputManager inputManager;

    bool isRunning{ true };
    int playerSun{ 150 };
    float skySunTimer{ 0.0f };

    GameState state{ GameState::MainMenu };
    int mainMenuCursor{ 0 };
    int levelSelectCursor{ 0 };

    std::vector<LevelData> levels;
    int currentLevelIndex{ 0 };

    std::chrono::steady_clock::time_point lastTime;

    void ProcessInput();
    void UpdatePhysics(float deltaTime);
    void RenderAll();

    void RenderMainMenu();
    void RenderLevelSelection();
    void RenderHUD();

public:
    GameEngine();
    void Run();
};
