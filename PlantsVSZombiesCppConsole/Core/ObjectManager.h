#pragma once
#include <list>
#include <vector>
#include <memory>
#include "../Entities/Zombie/Zombie.h"
#include "../Entities/LawnMower.h"
#include "../Entities/Sun.h"
#include "../Entities/Projectile.h"
#include "GridManager.h"

class ObjectManager
{
private:
    std::list<std::unique_ptr<Zombie>> zombies;
    std::list<std::unique_ptr<Projectile>> projectiles;
    std::list<std::unique_ptr<Sun>> droppedSuns;
    std::vector<std::unique_ptr<LawnMower>> mowers;
    int screenWidth;

public:
    explicit ObjectManager(int width);
    ~ObjectManager() = default;

    void AddZombie(std::unique_ptr<Zombie> zombie) { zombies.push_back(std::move(zombie)); }
    void AddSun(std::unique_ptr<Sun> sun) { droppedSuns.push_back(std::move(sun)); }

    void InitMowers(int rows, int startX, const std::vector<int>& rowYPositions);

    void UpdateEntities(float deltaTime, GridManager& gridManager, bool& isRunning);

    int CollectAllSuns();

    const std::list<std::unique_ptr<Zombie>>& GetZombies() const { return zombies; }
    const std::list<std::unique_ptr<Projectile>>& GetProjectiles() const { return projectiles; }
    const std::list<std::unique_ptr<Sun>>& GetSuns() const { return droppedSuns; }
    const std::vector<std::unique_ptr<LawnMower>>& GetMowers() const { return mowers; }
    û
    std::list<std::unique_ptr<Sun>>& GetSunsRef() { return droppedSuns; }
    std::list<std::unique_ptr<Projectile>>& GetProjectilesRef() { return projectiles; }
};
