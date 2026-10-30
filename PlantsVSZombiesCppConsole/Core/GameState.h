#pragma once

enum class GameState
{
    MainMenu,
    LevelSelection,
    Playing,
    GameOver
};

struct LevelData {
    int number;
    bool isUnlocked;
    int totalWaves;
};
