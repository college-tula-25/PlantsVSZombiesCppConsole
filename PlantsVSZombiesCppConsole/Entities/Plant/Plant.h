#pragma once
#include "../GameObject.h"

enum class PlantActivity
{
    Always,
    DayOnly,
    NightOnly
};

class Plant : public GameObject
{
protected:
    int hp;
    int maxHp;
    int cost{ 0 };
    PlantActivity activity{ PlantActivity::Always };
    bool isSleeping{ false };

public:
    Plant(Vector2 pos, const std::vector<std::wstring>& spr, int health, int sunCost, PlantActivity actMode = PlantActivity::Always)
        : GameObject(pos, spr), hp(health), maxHp(health), cost(sunCost), activity(actMode)
    {
    }

    virtual ~Plant() = default;

    void UpdateTimeOfDay(bool isNight)
    {
        if (activity == PlantActivity::DayOnly && isNight)
        {
            isSleeping = true;
        }
        else if (activity == PlantActivity::NightOnly && !isNight)
        {
            isSleeping = true;
        }
        else
        {
            isSleeping = false;
        }
    }

    void TakeDamage(int amount)
    {
        if (!IsActive()) return;

        hp -= amount;
        if (hp <= 0)
        {
            hp = 0;
            SetActive(false);
        }
    }

    void Update(float deltaTime) override
    {
        if (hp <= 0)
        {
            SetActive(false);
        }
    }

    int GetHp() const { return hp; }
    int GetMaxHp() const { return maxHp; }
    int GetCost() const { return cost; }

    bool IsAlive() const { return hp > 0 && IsActive(); }
    bool IsAwake() const { return IsAlive() && !isSleeping; }
    bool GetIsSleeping() const { return isSleeping; }
};
