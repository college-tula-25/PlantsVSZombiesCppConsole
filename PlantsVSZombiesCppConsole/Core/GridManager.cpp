#include "GridManager.h"
#include "../Entities/Plant/SunGives.h"
#include "../Entities/Plant/Attackers.h"
#include "../Entities/Sun.h"
#include "../Entities/Projectile.h"

GridManager::GridManager()
{
    plantGrid.resize(gridRows);
    for (int i = 0; i < gridRows; ++i) {
        plantGrid[i].resize(gridCols);
    }
}

Vector2 GridManager::GridToScreen(int row, int col) const
{
    return Vector2(gridOffsetX + (col * cellWidth), gridOffsetY + (row * cellHeight));
}

Plant* GridManager::GetPlantAt(int row, int col) const
{
    if (row < 0 || row >= gridRows || col < 0 || col >= gridCols) return nullptr;
    return plantGrid[row][col].get();
}

bool GridManager::TrySpawnPlant(int row, int col, std::unique_ptr<Plant> newPlant)
{
    if (row < 0 || row >= gridRows || col < 0 || col >= gridCols) return false;
    if (plantGrid[row][col] != nullptr) return false;

    plantGrid[row][col] = std::move(newPlant);
    return true;
}

void GridManager::UpdatePlants(float deltaTime, std::list<std::unique_ptr<Sun>>& sunsList, std::list<std::unique_ptr<Projectile>>& projList)
{
    for (int r = 0; r < gridRows; ++r) {
        for (int c = 0; c < gridCols; ++c) {
            if (plantGrid[r][c] != nullptr) {
                plantGrid[r][c]->Update(deltaTime);

                auto sunGiver = dynamic_cast<SunGives*>(plantGrid[r][c].get());
                if (sunGiver && sunGiver->CanProduceSun()) {
                    Vector2 sunPos(plantGrid[r][c]->GetPosition().GetX() + 4, plantGrid[r][c]->GetPosition().GetY());
                    sunsList.push_back(std::make_unique<Sun>(sunPos));
                    sunGiver->ResetProduction();
                }

                auto attacker = dynamic_cast<Attackers*>(plantGrid[r][c].get());
                if (attacker && attacker->CanShoot()) {
                    projList.push_back(std::make_unique<Projectile>(attacker->Shoot()));
                }
            }
        }
    }
}

void GridManager::RemoveDeadPlants()
{
    for (int r = 0; r < gridRows; ++r) {
        for (int c = 0; c < gridCols; ++c) {
            if (plantGrid[r][c] != nullptr && !plantGrid[r][c]->IsActive()) {
                plantGrid[r][c].reset();
            }
        }
    }
}
