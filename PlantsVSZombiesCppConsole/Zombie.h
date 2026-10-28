#pragma once
#include "GameObject.h"
#include <vector>
#include <string>

class BaseZombie : public GameObject
{
protected:
    int hp;
    int damagePerTick;
    bool isEating = false;

public:
    // ЧИСТЫЙ КОНСТРУКТОР: Убраны значения "= 200" и "= 20", чтобы компилятор не путался
    BaseZombie(Vector2 pos, const std::vector<std::string>& spr, int health, int dmg)
        : GameObject(pos, spr), hp(health), damagePerTick(dmg)
    {
    }

    virtual ~BaseZombie() = default;

    int GetHp() const { return hp; }
    void TakeDamage(int amount) { hp -= amount; }

    void SetEating(bool state) { isEating = state; }
    bool IsEating() const { return isEating; }
    int Attack() const { return damagePerTick; }

    void MoveLeft()
    {
        if (!isEating)
        {
            position = Vector2(position.GetX() - 1, position.GetY());
        }
    }
};

class RegularZombie : public BaseZombie
{
public:
    // Теперь передача аргументов кристально чистая для компилятора
    RegularZombie(Vector2 pos)
        : BaseZombie(pos, std::vector<std::string>{
        " [Z] ",
            " /|\\ ",
            " / \\ "
    }, 200, 15)
    {
    }
};
