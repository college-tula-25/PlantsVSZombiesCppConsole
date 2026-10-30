#include "InputManager.h"
#include <conio.h>

GameCommand InputManager::HandleInput()
{
    if (!_kbhit()) return GameCommand::None;

    char ch = _getch();

    switch (ch)
    {
    case 'w': case 'W': return GameCommand::MoveUp;
    case 's': case 'S': return GameCommand::MoveDown;
    case 'a': case 'A': return GameCommand::MoveLeft;
    case 'd': case 'D': return GameCommand::MoveRight;

    case '1': return GameCommand::SelectPeashooter;
    case '2': return GameCommand::SelectSunflower;
    case '3': return GameCommand::SelectWallNut;

    case 'e': case 'E': return GameCommand::CollectSuns;
    case 'z': case 'Z': return GameCommand::CheatSpawnZombie;
    case 'q': case 'Q': return GameCommand::Quit;

    default: return GameCommand::None;
    }
}
