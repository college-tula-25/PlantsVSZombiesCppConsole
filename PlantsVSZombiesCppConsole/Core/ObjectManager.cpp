#include "ObjectManager.h"
#include <iostream>
#include <algorithm>

ObjectManager::ObjectManager(int width) : screenWidth(width) {}

void ObjectManager::InitMowers(int rows, int startX, const std::vector<int>& rowYPositions)
{
    mowers.clear();
    mowers.resize(rows);
    for (int r = 0; r < rows; ++r)
    {
        mowers[r] = std::make_unique<LawnMower>(Vector2(startX, rowYPositions[r]), screenWidth);
    }
}

int ObjectManager::CollectAllSuns()
{
    int amountCollected = 0;
    for (const auto& sun : droppedSuns) {
        if (sun && sun->IsActive()) {
            amountCollected += 25;
        }
    }
    droppedSuns.clear();
    return amountCollected;
}

void ObjectManager::UpdateEntities(float deltaTime, GridManager& gridManager, bool& isRunning)
{
    // --- 1. Œ¡ÕŒ¬À≈Õ»≈ —Õ¿–ﬂƒŒ¬, —ŒÀÕ÷ » √¿«ŒÕŒ Œ—»ÀŒ  ---
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        (*it)->Update(deltaTime);
        if ((*it)->GetPosition().GetX() >= screenWidth) {
            (*it)->SetActive(false);
        }

        if (!(*it)->IsActive()) {
            it = projectiles.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto it = droppedSuns.begin(); it != droppedSuns.end();) {
        (*it)->Update(deltaTime);
        if (!(*it)->IsActive()) {
            it = droppedSuns.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto& mower : mowers) {
        if (mower && mower->IsActive()) {
            mower->Update(deltaTime);
        }
    }

    // --- 2. ƒ¬»∆≈Õ»≈, ¿“¿ ¿ «ŒÃ¡» » “–»√√≈–€  Œ—»ÀŒ  ---
    for (auto& zombie : zombies) {
        if (!zombie->IsActive()) continue;

        zombie->Update(deltaTime);
        if (!zombie->IsActive()) continue;

        int zX = zombie->GetPosition().GetX();
        int zY = zombie->GetPosition().GetY();
        int logicalRow = (zY - gridManager.GetGridOffsetY()) / gridManager.GetCellHeight();

        if (logicalRow >= 0 && logicalRow < static_cast<int>(mowers.size()))
        {
            auto& mower = mowers[logicalRow];
            if (mower && mower->IsActive())
            {
                int mX = mower->GetPosition().GetX();

                if (mower->IsMoving() && zX <= mX + mower->GetWidth() && zX + zombie->GetWidth() > mX)
                {
                    zombie->TakeDamage(9999);
                    continue;
                }
                else if (!mower->IsMoving() && zX <= mX + 1)
                {
                    mower->Trigger(); // «‡‚Ó‰ËÏ ÏÓÚÓ!
                    zombie->TakeDamage(9999);
                    continue;
                }
            }
            else
            {
                if (zX <= 1)
                {
                    isRunning = false;
                    return;
                }
            }
        }

        bool obstacleFound = false;
        if (logicalRow >= 0 && logicalRow < gridManager.GetRows()) {
            for (int c = 0; c < gridManager.GetCols(); ++c) {
                Plant* plant = gridManager.GetPlantAt(logicalRow, c);
                if (plant && plant->IsActive()) {
                    int pX = plant->GetPosition().GetX();

                    if (zX <= pX + plant->GetWidth() && zX + zombie->GetWidth() > pX) {
                        obstacleFound = true;
                        zombie->SetEating(true);

                        if (zombie->CanAttackNow()) {
                            plant->TakeDamage(zombie->GetDamage());
                            zombie->ResetAttackFlag();
                        }
                        break;
                    }
                }
            }
        }

        if (!obstacleFound) zombie->SetEating(false);
    }

    // --- 3. Œ¡–¿¡Œ“ ¿ œŒœ¿ƒ¿Õ»… —Õ¿–ﬂƒŒ¬ ( ŒÀÀ»«»») ---
    for (auto& proj : projectiles) {
        if (!proj->IsActive()) continue;

        for (auto& zombie : zombies) {
            if (!zombie->IsActive()) continue;

            int pX = proj->GetPosition().GetX();
            int pY = proj->GetPosition().GetY();
            int zX = zombie->GetPosition().GetX();
            int zY = zombie->GetPosition().GetY();

            if (pY >= zY && pY < zY + zombie->GetHeight() && pX >= zX && pX <= zX + zombie->GetWidth()) {
                zombie->TakeDamage(proj->GetDamage());
                proj->SetActive(false);
                break;
            }
        }
    }

    // --- 4. Œ—¬Œ¡Œ∆ƒ≈Õ»≈ œ¿Ãﬂ“» ---
    zombies.remove_if([](const std::unique_ptr<Zombie>& z) { return !z->IsActive(); });
    projectiles.remove_if([](const std::unique_ptr<Projectile>& p) { return !p->IsActive(); });

    for (auto& mower : mowers) {
        if (mower && !mower->IsActive()) {
            mower.reset();
        }
    }

    gridManager.RemoveDeadPlants();
}
