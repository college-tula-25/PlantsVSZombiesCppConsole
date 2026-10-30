#pragma once
#include "Plant.h"
#include "../Entities/Projectile.h" 

class Attackers : public Plant
{
protected:
    int damage{ 20 };
    float shootCooldown{ 1.5f };
    float shootTimer{ 0.0f };
    bool isReloaded{ true };

public:
    Attackers(Vector2 pos, const std::vector<std::wstring>& spr, int health = 300, int cost = 100, int dmg = 20, float cooldown = 1.5f)
        : Plant(pos, spr, health, cost, PlantActivity::Always),
        damage(dmg), shootCooldown(cooldown)
    {
    }

    void Update(float deltaTime) override
    {
        Plant::Update(deltaTime);
        if (!IsAwake()) return;

        if (!isReloaded)
        {
            shootTimer += deltaTime;
            if (shootTimer >= shootCooldown)
            {
                isReloaded = true;
                shootTimer = 0.0f;
            }
        }
    }

    Projectile Shoot()
    {
        isReloaded = false;
        Vector2 spawnPos(position.GetX() + width, position.GetY() + (height / 2));
        return Projectile(spawnPos, damage);
    }

    bool CanShoot() const { return isReloaded && IsAwake(); }
    void SetReloaded(bool state) { isReloaded = state; }
};
