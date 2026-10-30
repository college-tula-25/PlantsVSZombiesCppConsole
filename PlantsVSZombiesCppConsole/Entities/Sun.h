#pragma once
#include "../Entities/GameObject.h"

class Sun : public GameObject
{
private:
    int value{ 25 };
    float lifeTime{ 10.0f };
    float targetY{ 0.0f };
    float speed{ 2.0f };

public:
    Sun(Vector2 pos, int sunValue = 25, int stopAtY = 15)
        : GameObject(pos, std::vector<std::wstring>{
        L"  ⢀⣴⣶⣦⡀  ",
            L"  ⠻⢿⣿⡿⠟  "
    }),
        value(sunValue),
        targetY(static_cast<float>(stopAtY))
    {
    }

    void Update(float deltaTime) override
    {
        if (!IsActive()) return;

        if (position.GetY() < targetY)
        {
            float newY = position.GetY() + speed * deltaTime;
            if (newY > targetY) newY = targetY;

            SetPosition(Vector2(position.GetX(), static_cast<int>(newY)));
        }

        lifeTime -= deltaTime;
        if (lifeTime <= 0.0f)
        {
            SetActive(false);
        }
    }

    int Collect()
    {
        SetActive(false);
        return value;
    }

    int GetValue() const { return value; }
};
