#pragma once
#include "GameObject.h"

class Sun : public GameObject
{
private:
    int countSun{ 25 };
public:
    Sun(Vector2 pos) : GameObject(pos, {
        " **** ",
        "* * * ",
        "  *   "
        }) {
    }
};

class Projectile : public GameObject
{
private:
    int damage{ 20 };
public:
    Projectile(Vector2 pos) : GameObject(pos, { "&&" }) {}

    int GetDamage() const { return damage; }
};
