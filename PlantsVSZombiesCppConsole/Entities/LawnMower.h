#pragma once
#include "../Entities/GameObject.h"

class LawnMower : public GameObject
{
private:
    float speed{ 0.0f };
    int screenWidth{ 80 };
    bool isTriggered{ false };

public:
    LawnMower(Vector2 pos, int widthLimit = 80)
        : GameObject(pos, std::vector<std::wstring>{ L"⣰⣶⡆" }),
        screenWidth(widthLimit)
    {
    }

    void Update(float deltaTime) override
    {
        if (!IsActive()) return;

        if (isTriggered)
        {
            float newX = position.GetX() + speed * deltaTime;
            SetPosition(Vector2(static_cast<int>(newX), position.GetY()));

            if (position.GetX() > screenWidth)
            {
                SetActive(false);
            }
        }
    }

    void Trigger()
    {
        if (!isTriggered)
        {
            isTriggered = true;
            speed = 25.0f;
        }
    }

    bool IsMoving() const { return isTriggered; }
};
