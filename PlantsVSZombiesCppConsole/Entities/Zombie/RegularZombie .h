#pragma once
#include "Zombie.h" 

class RegularZombie : public Zombie
{
public:
    RegularZombie(Vector2 pos)
        : Zombie(pos, std::vector<std::wstring>{
        L" ⢰⣟⣿⣷  ",
            L"⠠⠤⣵⣷⡁  ",
            L" ⣠⠟⠙⣦  "
    }, 200, 15, 0.8f)
    {
    }
};
