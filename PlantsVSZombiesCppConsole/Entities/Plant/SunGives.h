#pragma once
#include "Plant.h"

class SunGives : public Plant
{
protected:
    int sunsCount{ 25 };
    float productionCooldown{ 10.0f };
    float productionTimer{ 0.0f };
    bool readyToProduce{ false };

public:
    SunGives(Vector2 pos, const std::vector<std::wstring>& spr, int health = 300, int cost = 50, int suns = 25, float cooldown = 10.0f)
        : Plant(pos, spr, health, cost, PlantActivity::DayOnly),
        sunsCount(suns), productionCooldown(cooldown)
    {
    }

    void Update(float deltaTime) override
    {
        Plant::Update(deltaTime);
        if (!IsAwake()) return;

        if (!readyToProduce)
        {
            productionTimer += deltaTime;
            if (productionTimer >= productionCooldown)
            {
                readyToProduce = true;
            }
        }
    }

    bool CanProduceSun() const { return readyToProduce; }
    int GetSunsCount() const { return sunsCount; }
    void ResetProduction()
    {
        productionTimer = 0.0f;
        readyToProduce = false;
    }
};
