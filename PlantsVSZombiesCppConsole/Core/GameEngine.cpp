#include "GameEngine.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>
#include <thread>

#include "../Entities/Plant/SunGives/Sunflower.h"
#include "../Entities/Plant/Attackers/Peashooter.h"
#include "../Entities/Plant/Defensive/WallNut.h"
#include "../Entities/Zombie/RegularZombie .h"

GameEngine::GameEngine()
    : renderer(screenWidth, screenHeight),
    objects(screenWidth)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    lastTime = std::chrono::steady_clock::now();

    levels = {
        {1, true,  2},
        {2, false, 3},
        {3, false, 5}
    };
}

void GameEngine::ProcessInput()
{
    GameCommand cmd = inputManager.HandleInput();

    if (cmd == GameCommand::None)
        return;

    if (state == GameState::MainMenu)
    {
        if (cmd == GameCommand::MoveUp)
            mainMenuCursor = (mainMenuCursor - 1 + 6) % 6;

        if (cmd == GameCommand::MoveDown)
            mainMenuCursor = (mainMenuCursor + 1) % 6;

        if (cmd == GameCommand::CollectSuns)
        {
            if (mainMenuCursor == 0)
                state = GameState::LevelSelection;
            else if (mainMenuCursor == 5)
                isRunning = false;
        }

        if (cmd == GameCommand::Quit)
            isRunning = false;

        return;
    }

    if (state == GameState::LevelSelection)
    {
        const int levelCount = static_cast<int>(levels.size());

        if (cmd == GameCommand::MoveUp)
            levelSelectCursor = (levelSelectCursor - 1 + levelCount) % levelCount;

        if (cmd == GameCommand::MoveDown)
            levelSelectCursor = (levelSelectCursor + 1) % levelCount;

        if (cmd == GameCommand::CollectSuns)
        {
            if (levels[levelSelectCursor].isUnlocked)
            {
                currentLevelIndex = levelSelectCursor;
                state = GameState::Playing;
                playerSun = 150;
                skySunTimer = 0.0f;

                std::vector<int> rowYPositions;
                rowYPositions.reserve(static_cast<std::size_t>(grid.GetRows()));

                for (int r = 0; r < grid.GetRows(); ++r)
                {
                    rowYPositions.push_back(grid.GridToScreen(r, 0).GetY());
                }

                objects.InitMowers(grid.GetRows(), 0, rowYPositions);
            }
        }

        if (cmd == GameCommand::Quit)
            state = GameState::MainMenu;

        return;
    }

    if (state == GameState::GameOver)
    {
        if (cmd == GameCommand::CollectSuns || cmd == GameCommand::Quit)
            state = GameState::MainMenu;

        return;
    }

    const int r = grid.GetCursorRow();
    const int c = grid.GetCursorCol();
    const Vector2 pos = grid.GridToScreen(r, c);

    switch (cmd)
    {
    case GameCommand::MoveUp:
        grid.MoveCursorUp();
        break;

    case GameCommand::MoveDown:
        grid.MoveCursorDown();
        break;

    case GameCommand::MoveLeft:
        grid.MoveCursorLeft();
        break;

    case GameCommand::MoveRight:
        grid.MoveCursorRight();
        break;

    case GameCommand::SelectPeashooter:
        if (playerSun >= 100)
        {
            if (grid.TrySpawnPlant(r, c, std::make_unique<Peashooter>(pos)))
                playerSun -= 100;
        }
        break;

    case GameCommand::SelectSunflower:
        if (playerSun >= 50)
        {
            if (grid.TrySpawnPlant(r, c, std::make_unique<Sunflower>(pos)))
                playerSun -= 50;
        }
        break;

    case GameCommand::SelectWallNut:
        if (playerSun >= 50)
        {
            if (grid.TrySpawnPlant(r, c, std::make_unique<WallNut>(pos)))
                playerSun -= 50;
        }
        break;

    case GameCommand::CollectSuns:
        playerSun += objects.CollectAllSuns();
        break;

    case GameCommand::CheatSpawnZombie:
    {
        const int randRow = std::rand() % grid.GetRows();
        const Vector2 spawnPos(
            screenWidth - 7,
            grid.GridToScreen(randRow, grid.GetCols() - 1).GetY()
        );

        objects.AddZombie(std::make_unique<RegularZombie>(spawnPos));
        break;
    }

    case GameCommand::Quit:
        state = GameState::MainMenu;
        break;

    default:
        break;
    }
}

void GameEngine::UpdatePhysics(float deltaTime)
{
    if (state != GameState::Playing)
        return;

    skySunTimer += deltaTime;

    if (skySunTimer >= 8.0f)
    {
        skySunTimer = 0.0f;

        const int sunRow = std::rand() % grid.GetRows();
        const int sunCol = std::rand() % grid.GetCols();
        const Vector2 sunPos = grid.GridToScreen(sunRow, sunCol);

        objects.AddSun(std::make_unique<Sun>(sunPos));
    }

    grid.UpdatePlants(
        deltaTime,
        objects.GetSunsRef(),
        objects.GetProjectilesRef()
    );

    bool isGameStillRunning = true;
    objects.UpdateEntities(deltaTime, grid, isGameStillRunning);

    if (!isGameStillRunning)
        state = GameState::GameOver;
}

void GameEngine::RenderAll()
{
    renderer.ClearBuffer();

    if (state == GameState::MainMenu)
    {
        RenderMainMenu();
    }
    else if (state == GameState::LevelSelection)
    {
        RenderLevelSelection();
    }
    else if (state == GameState::Playing)
    {
        RenderHUD();

        for (int r = 0; r < grid.GetRows(); ++r)
        {
            for (int c = 0; c < grid.GetCols(); ++c)
            {
                Plant* plant = grid.GetPlantAt(r, c);

                if (plant != nullptr)
                {
                    if (plant->GetMaxHp() > 1000)
                        renderer.DrawObject(*plant, ConsoleColor::Brown);
                    else
                        renderer.DrawObject(*plant, ConsoleColor::Green);
                }
            }
        }

        for (const auto& projectile : objects.GetProjectiles())
            renderer.DrawObject(*projectile, ConsoleColor::Blue);

        for (const auto& zombie : objects.GetZombies())
            renderer.DrawObject(*zombie, ConsoleColor::White);

        for (const auto& sun : objects.GetSuns())
            renderer.DrawObject(*sun, ConsoleColor::Yellow);

        for (const auto& mower : objects.GetMowers())
        {
            if (mower)
                renderer.DrawObject(*mower, ConsoleColor::Red);
        }

        GameObject cursorObj(
            grid.GetCursorScreenPos(),
            { L"+----+", L"|    |", L"+----+" }
        );
        renderer.DrawObject(cursorObj, ConsoleColor::Red);
    }
    else if (state == GameState::GameOver)
    {
        renderer.DrawTextStr(30, 10, L"!!! ИГРА ОКОНЧЕНА !!!", ConsoleColor::Red);
        renderer.DrawTextStr(26, 12, L"Зомби съели ваши мозги...", ConsoleColor::White);
        renderer.DrawTextStr(24, 15, L"Нажмите [E] или [Q] для меню", ConsoleColor::Yellow);
    }

    renderer.Display();
}

void GameEngine::RenderMainMenu()
{
    renderer.DrawTextStr(
        0, 0,
        L"⢰⣶⠶⣦⡄⢰⣶⠀⠀⠀⢠⣶⣆⠀⠀⣶⣦⠀⢰⡆⠶⢶⣶⠶⠆⣴⡶⠶⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠶⠶⣶⡶⠀⣠⡶⠶⣦⡄⠀⣶⣶⠀⢠⣶⡆⢰⣶⠶⣶⡄⢰⣶⠀⣶⡶⠶⠆⠀⣴⡶⠶⡆",
        ConsoleColor::Green
    );
    renderer.DrawTextStr(
        0, 1,
        L"⢸⣿⣤⣾⠇⢸⣿⠀⠀⠀⣾⠋⣿⡄⠀⣿⡟⣧⣸⡇⠀⢸⣿⠀⠀⠻⢶⣦⣄⠀⠀⠀⠹⣇⢠⡿⠀⣿⣉⡛⠀⠀⠀⠀⣴⡟⠁⠀⣿⠁⠀⢸⣿⠀⣿⡟⣧⡾⢹⡇⢸⣿⠶⣿⡁⢸⣿⠀⣿⡷⠶⠆⠀⠻⢶⣦⣄",
        ConsoleColor::Green
    );
    renderer.DrawTextStr(
        0, 2,
        L"⢸⣿⠀⠀⠀⢸⣿⣤⣤⣼⡟⠛⠻⣷⠀⣿⡇⠸⣿⡇⠀⢸⣿⠀⠀⣦⣤⣼⠟⠀⠀⠀⠀⢻⣾⠃⠀⣬⣭⡿⠀⠀⠀⣾⣯⣤⣤⠀⠻⣧⣤⡾⠏⠀⣿⡇⠙⠃⢸⡇⢸⣿⣤⣾⠇⢸⣿⠀⣿⣧⣤⡄⠀⣦⣤⣼⠟",
        ConsoleColor::Green
    );

    renderer.DrawTextStr(0, 4, L"⠀⠀⠀⠀⠀⣀⣤⣶⣶⣶⣶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣀⡀⠀⣰⣷⣄⣀⣠⣴⣇⠀⠀⢀⠀⠀", ConsoleColor::White);
    renderer.DrawTextStr(0, 5, L"⠀⠀⠀⠀⣠⣾⣿⣿⣿⡟⢡⣶⡌⢻⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣿⠏⠀", ConsoleColor::White);
    renderer.DrawTextStr(0, 6, L"⠀⠀⠀⣰⣿⣿⣿⣿⣿⣧⡈⠛⢁⣼⣿⣿⣶⣶⣶⡶⠦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⡿⠿⣿⣿⡿⠛⠻⣿⣯⠀⠀", ConsoleColor::White);
    renderer.DrawTextStr(0, 7, L"⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⢀⣴⣶⣄⠀⢀⣴⣾⣿⡏⠠⡦⠈⣿⡀⠺⠂⣸⣿⡇⠀", ConsoleColor::White);
    renderer.DrawTextStr(0, 8, L"⠀⠀⠀⢿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⡀⠀⡀⠀⠘⢿⣿⠟⠀⠸⣿⣿⣿⣿⣦⣤⠎⠘⣿⣶⣾⣿⣿⡇⠀⠀", ConsoleColor::White);
    renderer.DrawTextStr(0, 9, L"⠀⠀⠀⠈⢿⣶⣭⣛⣿⣿⣿⣿⣿⣿⣿⣿⣿⠛⠛⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⢻⣿⣿⠯⢤⠤⠼⡿⠿⣿⡟⠀⠀", ConsoleColor::White);
    renderer.DrawTextStr(0, 10, L"⠀⠀⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⣿⠿⠋⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣀⣸⣀⣀⣇⣀⣿⠀⠀⠀", ConsoleColor::White);
    renderer.DrawTextStr(0, 11, L"⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⢩⣭⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⠿⠛⠿⣿⣿⣿⠀⠀⠀", ConsoleColor::White);
    renderer.DrawTextStr(0, 12, L"⠀⠀⠀⠀⢀⣠⣀⠀⠀⠀⢸⣿⡇⠀⠀⢀⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⠀⠀⢠⣿⣿⣿⣷⣄⠀", ConsoleColor::White);
    renderer.DrawTextStr(0, 13, L"⠀⠀⠒⢿⣿⣿⣿⣿⣶⣦⣼⣿⡇⣠⣾⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠠⣾⡿⣿⣿⣿⣿⣆⠀⣾⣿⣿⣿⣿⢿⠗", ConsoleColor::White);
    renderer.DrawTextStr(0, 14, L"⠀⡠⡀⠀⠈⡻⣿⠿⠿⡿⣿⣿⣿⣿⣿⣿⡿⡫⡀⠀⠀⡠⡀⠀⠀⡠⡀⠀⠀⡨⣀⣿⣿⣿⣿⣿⠀⣿⣿⣿⣿⣿⡀⠀", ConsoleColor::White);

    renderer.DrawTextStr(46, 4, L"╔══════════════════════════════╗", ConsoleColor::Yellow);
    renderer.DrawTextStr(46, 5, L"║ ГЛАВНОЕ МЕНЮ                 ║", ConsoleColor::Yellow);
    renderer.DrawTextStr(46, 6, L"╠══════════════════════════════╣", ConsoleColor::Yellow);
    renderer.DrawTextStr(46, 7, L"║                              ║", ConsoleColor::Yellow);
    renderer.DrawTextStr(46, 8, L"║ Приключение                  ║", ConsoleColor::Yellow);
    renderer.DrawTextStr(46, 9, L"║ Мини-игры                    ║", ConsoleColor::Yellow);
    renderer.DrawTextStr(46, 10, L"║ Выживание                    ║", ConsoleColor::Yellow);
    renderer.DrawTextStr(46, 11, L"║ Альманах                     ║", ConsoleColor::Yellow);
    renderer.DrawTextStr(46, 12, L"║ Настройки                    ║", ConsoleColor::Yellow);
    renderer.DrawTextStr(46, 13, L"║ Выход                        ║", ConsoleColor::Yellow);
    renderer.DrawTextStr(46, 14, L"╚══════════════════════════════╝", ConsoleColor::Yellow);

    const int cursorLineY = 8 + mainMenuCursor;
    renderer.DrawTextStr(49, cursorLineY, L"►", ConsoleColor::Red);

    renderer.DrawTextStr(
        0, 15,
        L"⣰⣁⣱⣀⣰⣁⣱⣀⣰⣁⣹⣛⣻⣉⣹⣉⣰⣁⣱⣀⣰⣁⣱⣀⣰⣁⣱⣀⣰⣁⣹⣛⣻⣛⣻⣛⣲⣛⣻⣛⣻⣛⣳⣀⣰⣁⣱⣀⣰⣁⣱⣀⣰⣁⣱⣀⣰⣁⣱⣀⣰⣁⣱⣀⣰⣁⣱⣀⣰⣁⣱⣀⣰⣁⣱⣀⣰⣁",
        ConsoleColor::Green
    );
    renderer.DrawTextStr(
        0, 16,
        L"⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉",
        ConsoleColor::Green
    );

    renderer.DrawTextStr(
        13, 19,
        L"↑/↓ — выбор Enter / E — подтвердить Q — выход",
        ConsoleColor::White
    );
}

void GameEngine::RenderLevelSelection()
{
    renderer.DrawTextStr(22, 4, L"╔══════════════════════════════════╗", ConsoleColor::Yellow);
    renderer.DrawTextStr(22, 5, L"║           ВЫБОР УРОВНЯ          ║", ConsoleColor::Yellow);
    renderer.DrawTextStr(22, 6, L"╠══════════════════════════════════╣", ConsoleColor::Yellow);

    for (std::size_t i = 0; i < levels.size(); ++i)
    {
        std::wstring levelLine = L"║ ";

        if (static_cast<int>(i) == levelSelectCursor)
            levelLine += L"► ";
        else
            levelLine += L"  ";

        levelLine += L"Этап " + std::to_wstring(levels[i].number);

        if (!levels[i].isUnlocked)
            levelLine += L" [ ЗАБЛОКИРОВАН ]";
        else
            levelLine += L" (Сложность: Волн " + std::to_wstring(levels[i].totalWaves) + L")";

        while (levelLine.size() < 35)
            levelLine += L' ';

        levelLine += L"║";

        renderer.DrawTextStr(
            22,
            8 + static_cast<int>(i),
            levelLine,
            levels[i].isUnlocked ? ConsoleColor::Yellow : ConsoleColor::White
        );
    }

    renderer.DrawTextStr(
        22,
        8 + static_cast<int>(levels.size()),
        L"╚══════════════════════════════════╝",
        ConsoleColor::Yellow
    );
    renderer.DrawTextStr(
        20, 15,
        L"[E] - Начать уровень | [Q] - Назад в меню",
        ConsoleColor::White
    );
}

void GameEngine::RenderHUD()
{
    renderer.DrawTextStr(0, 0, L"================================================================================", ConsoleColor::White);
    renderer.DrawTextStr(2, 1, L"РЕСУРСЫ: ☼ " + std::to_wstring(playerSun), ConsoleColor::Yellow);
    renderer.DrawTextStr(24, 1, L" Горохострел: 100 ☼", ConsoleColor::Blue);
    renderer.DrawTextStr(47, 1, L" Подсолнух: 50 ☼", ConsoleColor::Green);
    renderer.DrawTextStr(67, 1, L" Орех: 50 ☼", ConsoleColor::Brown);
    renderer.DrawTextStr(2, 2, L"Сбор солнц: [E] | Чит-спавн Зомби: [Z] | Вернуться в меню: [Q]", ConsoleColor::White);
    renderer.DrawTextStr(0, 3, L"--------------------------------------------------------------------------------", ConsoleColor::White);
}

void GameEngine::Run()
{
    const float targetFrameTime = 1.0f / 30.0f;

    while (isRunning)
    {
        const auto now = std::chrono::steady_clock::now();

        float deltaTime =
            std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;

        if (deltaTime > 0.1f)
            deltaTime = 0.1f;

        ProcessInput();
        UpdatePhysics(deltaTime);
        RenderAll();

        const auto frameEndTime = std::chrono::steady_clock::now();

        const float actualFrameTime =
            std::chrono::duration<float>(frameEndTime - now).count();
        const float sleepTime = targetFrameTime - actualFrameTime;

        if (sleepTime > 0.0f)
        {
            std::this_thread::sleep_for(
                std::chrono::duration<float>(sleepTime)
            );
        }
    }
}
