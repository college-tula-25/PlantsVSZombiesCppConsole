#include "Core/GameEngine.h"
#include "Core/GameException.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h> 
#endif

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001); // Переключает консоль Windows в полноценный UTF-8
#endif

    try
    {
        GameEngine engine;
        engine.Run();
    }
    catch (const GameException& e)
    {
        std::cerr << "\n[ИГРОВОЕ ИСКЛЮЧЕНИЕ]: " << e.what() << "\n";
        std::cerr << "Подробности сохранены в файле crash_log.txt\n";
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n[СИСТЕМНАЯ ОШИБКА]: " << e.what() << "\n";
        return 2;
    }
    catch (...)
    {
        std::cerr << "\n[НЕИЗВЕСТНАЯ КРИТИЧЕСКАЯ ОШИБКА]\n";
        return 3;
    }

    return 0;
}
