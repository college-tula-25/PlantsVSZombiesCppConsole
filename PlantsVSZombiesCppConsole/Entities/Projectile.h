#pragma once
#include "../Entities/GameObject.h"

class Projectile : public GameObject
{
private:
    int damage{ 20 };
    float speed{ 15.0f };
    int maxScreenWidth{ 80 };

public:
    Projectile(Vector2 pos, int projDamage = 20, float projSpeed = 15.0f)
        : GameObject(pos, std::vector<std::wstring>{ L"&&" }),
        damage(projDamage),
        speed(projSpeed)
    {
    }

    void Update(float deltaTime) override
    {
        if (!IsActive()) return;

        float newX = position.GetX() + speed * deltaTime;
        SetPosition(Vector2(static_cast<int>(newX), position.GetY()));

        if (position.GetX() > maxScreenWidth)
        {
            SetActive(false);
        }
    }

    int GetDamage() const { return damage; }

    void OnHit()
    {
        SetActive(false);
    }
};
