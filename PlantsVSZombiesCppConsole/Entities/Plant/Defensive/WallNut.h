#pragma once
#include "../Defensive.h"

class WallNut : public Defensive
{
public:
    WallNut(Vector2 pos)
        : Defensive(pos, std::vector<std::wstring>{
        L" ⢠⡾⣿⣿⢷⡄ ",
            L" ⢸⣿⣛⣛⣿⡇ ",
            L" ⠘⢿⣿⣿⡿⠃ "
    }, 4000, 50)
    {
    }

    void Update(float deltaTime) override
    {
        Defensive::Update(deltaTime);

        if (!IsActive()) return;

        float hpPercent = GetHealthPercentage();

        if (hpPercent <= 30.0f)
        {
            sprite = {
                L" /---\\ ",
                L"| x x |",
                L" \\___/ "
            };
        }
        else if (hpPercent <= 60.0f)
        {
            sprite = {
                L" /---\\ ",
                L"| o o |",
                L" \\___/ "
            };
        }
    }
};
