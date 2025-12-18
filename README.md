# Wrapper System - Система динамического вызова методов C++17

## Требования

- C++17 компилятор (g++, clang++, MSVC)
- CMake 3.15 или выше

## Сборка на Windows

### С MinGW (g++)
```cmd
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
main_program.exe
```

### С MSYS2/MinGW64
```bash
mkdir build
cd build
cmake .. -G "MSYS Makefiles"
make
./main_program.exe
```

## Сборка на macOS/Linux

```bash
mkdir build
cd build
cmake ..
make
./main_program
```

## Структура проекта

- `iwrapper.h` - Базовый интерфейс для обёрток методов
- `engine.h` - Движок для регистрации и выполнения команд
- `wrapper.h` - Шаблонная обёртка для методов класса
- `wrapper.tpp` - Реализация шаблонов
- `tests.h/cpp` - Автоматические тесты
- `main.cpp` - Точка входа

