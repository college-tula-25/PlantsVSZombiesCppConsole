#include "GameEngine.h"
#include <conio.h>
#include <cstdlib> // Для rand()

GameEngine::GameEngine()
	: renderer(screenWidth, screenHeight)
{
	plantGrid.resize(gridRows);
	for (int i = 0; i < gridRows; ++i)
	{
		plantGrid[i].resize(gridCols);
	}
	// Инициализация генератора случайных чисел для солнца с неба
	srand(static_cast<unsigned int>(time(nullptr)));
}

void GameEngine::SpawnPlant(int row, int col, char type)
{
	if (row < 0 || row >= gridRows || col < 0 || col >= gridCols) return;
	if (plantGrid[row][col] != nullptr) return;

	Vector2 screenPos = GridToScreen(row, col);

	if (type == 's' && playerSun >= 50) {
		plantGrid[row][col] = std::make_unique<Sunflower>(screenPos);
		playerSun -= 50;
	}
	else if (type == 'p' && playerSun >= 100) {
		plantGrid[row][col] = std::make_unique<Peashooter>(screenPos);
		playerSun -= 100;
	}
	else if (type == 'w' && playerSun >= 50) {
		plantGrid[row][col] = std::make_unique<WallNut>(screenPos);
		playerSun -= 50;
	}
}

void GameEngine::SpawnZombie(int row)
{
	if (row < 0 || row >= gridRows) return;

	// Вычисляем позицию появления зомби
	Vector2 screenPos(screenWidth - 7, GridToScreen(row, gridCols - 1).GetY());

	// ИСПРАВЛЕНИЕ: Создаем именно RegularZombie, а не BaseZombie!
	// Конструктор RegularZombie принимает ОДИН аргумент (Vector2 pos),
	// а внутренний спрайт, HP и урон он подставит автоматически.
	zombies.push_back(std::make_unique<RegularZombie>(screenPos));
}


void GameEngine::ProcessInput()
{
	if (_kbhit())
	{
		char ch = _getch();

		// 1. Управление курсором (WASD)
		if (ch == 'w' || ch == 'W') { if (cursorRow > 0) cursorRow--; }
		if (ch == 's' || ch == 'S') { if (cursorRow < gridRows - 1) cursorRow++; }
		if (ch == 'a' || ch == 'A') { if (cursorCol > 0) cursorCol--; }
		if (ch == 'd' || ch == 'D') { if (cursorCol < gridCols - 1) cursorCol++; }

		// 2. Посадка растений на выбранную клетку
		if (ch == '1') SpawnPlant(cursorRow, cursorCol, 'p'); // Горохострел
		if (ch == '2') SpawnPlant(cursorRow, cursorCol, 's'); // Подсолнух
		if (ch == '3') SpawnPlant(cursorRow, cursorCol, 'w'); // Стенорех

		// 3. Сбор солнышек (Автосбор по кнопке 'e')
		if (ch == 'e' || ch == 'E') {
			if (!droppedSuns.empty()) {
				playerSun += 25 * droppedSuns.size();
				droppedSuns.clear();
			}
		}

		// Спавн зомби для тестов
		if (ch == 'z' || ch == 'Z') SpawnZombie(rand() % gridRows);
		if (ch == 'q' || ch == 'Q') isRunning = false;
	}
}

void GameEngine::UpdatePhysics()
{
	static int tickCounter = 0;
	tickCounter++;

	// --- 1. ЛОГИКА СОЛНЦА ---
	// Падение солнца с неба (каждые 80 тиков)
	if (tickCounter % 80 == 0) {
		int randomCol = rand() % gridCols;
		int randomRow = rand() % gridRows;
		// Солнце появляется сверху соответствующей плитки газона
		Vector2 skySunPos(GridToScreen(randomRow, randomCol).GetX(), GridToScreen(randomRow, randomCol).GetY());
		droppedSuns.push_back(Sun(skySunPos));
	}

	// Генерация солнца Подсолнухами (каждые 60 тиков)
	if (tickCounter % 60 == 0) {
		for (int r = 0; r < gridRows; ++r) {
			for (int c = 0; c < gridCols; ++c) {
				if (plantGrid[r][c] != nullptr) {
					auto sunGiver = dynamic_cast<SunGives*>(plantGrid[r][c].get());
					if (sunGiver) {
						// Спавним солнце чуть правее подсолнуха
						Vector2 sunPos(plantGrid[r][c]->GetPosition().GetX() + 4, plantGrid[r][c]->GetPosition().GetY());
						droppedSuns.push_back(Sun(sunPos));
					}
				}
			}
		}
	}

	// --- 2. СТРЕЛЬБА РАСТЕНИЙ ---
	if (tickCounter % 20 == 0) {
		for (int r = 0; r < gridRows; ++r) {
			for (int c = 0; c < gridCols; ++c) {
				if (plantGrid[r][c] != nullptr) {
					auto attacker = dynamic_cast<Attackers*>(plantGrid[r][c].get());
					if (attacker) projectiles.push_back(attacker->Shoot());
				}
			}
		}
	}

	// --- 3. ДВИЖЕНИЕ СНАРЯДОВ ---
	for (auto it = projectiles.begin(); it != projectiles.end();) {
		Vector2 curPos = it->GetPosition();
		*it = Projectile(Vector2(curPos.GetX() + 2, curPos.GetY())); // Снаряд летит чуть быстрее (+2)
		if (it->GetPosition().GetX() >= screenWidth) it = projectiles.erase(it);
		else ++it;
	}

	// --- 4. ДВИЖЕНИЕ И АТАКА ЗОМБИ (Механика «Едят, а не проходят») ---
	if (tickCounter % 5 == 0) {
		for (auto& zombie : zombies) {
			int zX = zombie->GetPosition().GetX();
			int zY = zombie->GetPosition().GetY();

			// Проверяем, есть ли перед зомби растение в этой строке
			bool obstacleFound = false;
			for (int c = 0; c < gridCols; ++c) {
				int logicalRow = (zY - gridOffsetY) / cellHeight;
				if (logicalRow >= 0 && logicalRow < gridRows && plantGrid[logicalRow][c] != nullptr) {
					int pX = plantGrid[logicalRow][c]->GetPosition().GetX();

					// Если зомби подошел вплотную к растению слева (пересечение текстур)
					if (zX <= pX + plantGrid[logicalRow][c]->GetWidth() && zX + zombie->GetWidth() > pX) {
						obstacleFound = true;
						zombie->SetEating(true);

						// Наносим урон растению
						plantGrid[logicalRow][c]->TakeDamage(zombie->Attack());

						// Если растение съедено полностью — удаляем его
						if (plantGrid[logicalRow][c]->GetHp() <= 0) {
							plantGrid[logicalRow][c].reset(); // Удаляет умный указатель
							zombie->SetEating(false); // Зомби снова может идти
						}
						break;
					}
				}
			}

			if (!obstacleFound) {
				zombie->SetEating(false);
				zombie->MoveLeft();
			}

			// Условие проигрыша
			if (zombie->GetPosition().GetX() <= 0) {
				std::cout << "\n!!! ZOMBIES ATE YOUR BRAINS !!!\n";
				isRunning = false;
				return;
			}
		}
	}

	// --- 5. КОЛЛИЗИИ (Снаряды бьют зомби) ---
	for (auto projIt = projectiles.begin(); projIt != projectiles.end();) {
		bool hit = false;
		for (auto& zombie : zombies) {
			int pX = projIt->GetPosition().GetX();
			int pY = projIt->GetPosition().GetY();
			int zX = zombie->GetPosition().GetX();
			int zY = zombie->GetPosition().GetY();

			if (pY >= zY && pY < zY + zombie->GetHeight() && pX >= zX && pX <= zX + zombie->GetWidth()) {
				zombie->TakeDamage(projIt->GetDamage());
				hit = true;
				break;
			}
		}
		if (hit) projIt = projectiles.erase(projIt);
		else ++projIt;
	}

	// Очистка мертвых зомби
	zombies.remove_if([](const std::unique_ptr<BaseZombie>& z) { return z->GetHp() <= 0; });
}

void GameEngine::RenderAll()
{
	renderer.ClearBuffer();

	// 1. Отрисовка сетки растений
	for (int r = 0; r < gridRows; ++r) {
		for (int c = 0; c < gridCols; ++c) {
			if (plantGrid[r][c] != nullptr) renderer.DrawObject(*plantGrid[r][c]);
		}
	}

	// 2. Отрисовка динамических объектов
	for (const auto& proj : projectiles) renderer.DrawObject(proj);
	for (const auto& zombie : zombies) renderer.DrawObject(*zombie);
	for (const auto& sun : droppedSuns) renderer.DrawObject(sun);

	// 3. ОТРИСОВКА КУРСОРA (Подсвечиваем выбранную плитку символами `[ ]`)
	Vector2 cursorScreenPos = GridToScreen(cursorRow, cursorCol);
	// Простой рендеринг рамки курсора прямо в буфер
	int cX = cursorScreenPos.GetX();
	int cY = cursorScreenPos.GetY();

	// Передаем временный объект GameObject в качестве курсора
	GameObject cursorObj(cursorScreenPos, {
		"+----+",
		"|    |",
		"+----+"
		});
	renderer.DrawObject(cursorObj);

	renderer.Display();

	// Вывод интерфейса под игровым полем
	std::cout << " Suns: " << playerSun
		<< " | Cursor: [" << cursorRow << "," << cursorCol << "]"
		<< " | [1] Pea(100) [2] Sun(50) [3] Wall(50) | [E] Collect Suns | [Z] Spawn Zombie\n";
}

void GameEngine::Run()
{
	while (isRunning)
	{
		ProcessInput();
		UpdatePhysics();
		RenderAll();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
