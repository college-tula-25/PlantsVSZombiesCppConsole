#pragma once
#include "Plant.h"

class Defensive : public Plant
{
public:
    Defensive(Vector2 pos, const std::vector<std::wstring>& spr, int health = 4000, int cost = 50)
        : Plant(pos, spr, health, cost, PlantActivity::Always)
    {
    }

    float GetHealthPercentage() const
    {
        return maxHp > 0 ? (static_cast<float>(hp) / maxHp) * 100.0f : 0.0f;
    }
};
