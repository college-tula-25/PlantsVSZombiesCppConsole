#pragma once
#include <vector>
#include <list>
#include <memory>
#include "../Entities/Plant/Plant.h"

class Sun;
class Projectile;

class GridManager
{
private:
    const int gridRows = 5;
    const int gridCols = 9;

    const int cellWidth = 8;
    const int cellHeight = 3;
    const int gridOffsetX = 4;
    const int gridOffsetY = 4;


    int cursorRow{ 0 };
    int cursorCol{ 0 };

    std::vector<std::vector<std::unique_ptr<Plant>>> plantGrid;

public:
    GridManager();

    bool TrySpawnPlant(int row, int col, std::unique_ptr<Plant> newPlant);

    void UpdatePlants(float deltaTime, std::list<std::unique_ptr<Sun>>& sunsList, std::list<std::unique_ptr<Projectile>>& projList);
    void RemoveDeadPlants();

    void MoveCursorUp() { if (cursorRow > 0) cursorRow--; }
    void MoveCursorDown() { if (cursorRow < gridRows - 1) cursorRow++; }
    void MoveCursorLeft() { if (cursorCol > 0) cursorCol--; }
    void MoveCursorRight() { if (cursorCol < gridCols - 1) cursorCol++; }

    int GetCursorRow() const { return cursorRow; }
    int GetCursorCol() const { return cursorCol; }
    int GetRows() const { return gridRows; }
    int GetCols() const { return gridCols; }
    int GetCellHeight() const { return cellHeight; }
    int GetGridOffsetY() const { return gridOffsetY; }

    Plant* GetPlantAt(int row, int col) const;
    Vector2 GridToScreen(int row, int col) const;
    Vector2 GetCursorScreenPos() const { return GridToScreen(cursorRow, cursorCol); }
};
