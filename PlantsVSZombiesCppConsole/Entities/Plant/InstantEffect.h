#pragma once
#include "Plant.h"

class InstantEffect : public Plant
{
protected:
    int explosionRadius{ 1 };
    int explosionDamage{ 1800 };
    float timeToExplode{ 1.0f };
    bool hasExploded{ false };

public:
    InstantEffect(Vector2 pos, const std::vector<std::string>& spr, int health = 300, int cost = 150, int radius = 1, int dmg = 1800)
        : Plant(pos, spr, health, cost, PlantActivity::Always),
        explosionRadius(radius), explosionDamage(dmg)
    {
    }

    void Update(float deltaTime) override
    {
        Plant::Update(deltaTime);

        if (!IsActive() || hasExploded) return;

        timeToExplode -= deltaTime;
        if (timeToExplode <= 0.0f)
        {
            hasExploded = true;
            SetActive(false);
        }
    }

    bool IsExplodingNow() const { return hasExploded; }

    int GetExplosionDamage() const { return explosionDamage; }
    int GetExplosionRadius() const { return explosionRadius; }
};
