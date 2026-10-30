#pragma once
#include "../../Entities/GameObject.h"
#include <vector>
#include <string>

class Zombie : public GameObject
{
protected:
    int hp;
    int maxHp;
    int damagePerTick;
    float movementSpeed{ 1.0f };
    float accumulatedX{ 0.0f };

    bool isEating{ false };
    float attackCooldown{ 0.5f };
    float attackTimer{ 0.0f };
    bool readyToAttack{ false };

public:
    Zombie(Vector2 pos, const std::vector<std::wstring>& spr, int health, int dmg, float speed = 1.0f)
        : GameObject(pos, spr), hp(health), maxHp(health), damagePerTick(dmg),
        movementSpeed(speed), accumulatedX(static_cast<float>(pos.GetX()))
    {
    }

    virtual ~Zombie() = default;

    void Update(float deltaTime) override
    {
        GameObject::Update(deltaTime);

        if (!IsActive()) return;

        if (hp <= 0)
        {
            SetActive(false);
            return;
        }

        if (isEating)
        {
            attackTimer += deltaTime;
            if (attackTimer >= attackCooldown)
            {
                readyToAttack = true;
                attackTimer = 0.0f;
            }
        }
        else
        {
            accumulatedX -= movementSpeed * deltaTime;
            if (position.GetX() != static_cast<int>(accumulatedX))
            {
                SetPosition(Vector2(static_cast<int>(accumulatedX), position.GetY()));
            }
            readyToAttack = false;
            attackTimer = 0.0f;
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

    bool CanAttackNow() const { return isEating && readyToAttack; }
    void ResetAttackFlag() { readyToAttack = false; }

    int GetHp() const { return hp; }
    int GetDamage() const { return damagePerTick; }
    void SetEating(bool state) { isEating = state; }
    bool IsEating() const { return isEating; }
};
