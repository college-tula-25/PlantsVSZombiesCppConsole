#pragma once
#include "../SunGives.h"

class Sunflower : public SunGives
{
public:
    Sunflower(Vector2 pos)
        : SunGives(pos, std::vector<std::wstring>{
        L" ⢰⣷⣻⣟⣾⡆ ",
            L" ⠈⠃⢻⡟⠘⠁ ",
            L" ⠈⢛⣺⣗⡛⠁ "
    }, 300, 50, 25, 10.0f)
    {
    }
};
