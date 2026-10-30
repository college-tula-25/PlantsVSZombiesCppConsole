#pragma once
#include "../Attackers.h"

class Peashooter : public Attackers
{
public:
    Peashooter(Vector2 pos)
        : Attackers(pos, std::vector<std::wstring>{
        L" ⢴⣻⣿⣛⣶ ",
            L"  ⢹⡏⠉   ",
            L" ⢙⣺⣗⡋  "
    }, 300, 100, 20, 1.5f)
    {
    }
};
