#pragma once

enum class GameCommand
{
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    SelectPeashooter,
    SelectSunflower,
    SelectWallNut,
    CollectSuns,
    CheatSpawnZombie,
    Quit
};

class InputManager
{
public:
    InputManager() = default;
    ~InputManager() = default;

    GameCommand HandleInput();
};
