# CSVReader

Программа для чтения и вычисления значений в CSV-таблицах с поддержкой формул вида `=Arg1 op Arg2` (`=A1 + B2` и т.д.).

Поддерживается валидация, обработка ошибок, вывод результата в консоль.

## 📦 Структура проекта

```txt
csvreader/
├── include/
│ ├── csv_parser.h
│ └── evaluator.h
├── src/
│ ├── main.cpp
│ ├── csv_parser.cpp
│ └── evaluator.cpp
├── tests/
│ ├── test_csv_parser.cpp
│ └── evaluator_test.cpp
└── CMakeLists.txt
```

---

## 🛠️ Сборка

### Требования

- **Linux:** `g++` или `clang++`, `cmake`, `make`
- **Windows:** `mingw-w64` или `Cygwin`, `CMake`, `Make` или `Ninja`
- **Общие зависимости:**
  - C++17
  - GTest (>= 1.14)

---

### 💡 Команды (Linux или Windows)

1. Создайте папку сборки:

```bash
mkdir build && cd build
```

Выполните конфигурацию проекта:

```bash
cmake ..
```
Соберите проект и тесты:

```bash
cmake --build .
```

🚀 Запуск

- Программа
```bash
./csvreader path_to_file.csv
```

- Тесты
```bash
./parser_test         # Тесты для структуры
./evaluator_test      # Тесты для выражений
```

⚙️ Кроссплатформенность

Проект написан в чистом стандарте C++17 без зависимостей от POSIX или WinAPI.

✅ Компилируется в:

- Linux (gcc / clang)

- Windows (MinGW или Cygwin)

> Если используете Windows:
<br>- Установите CMake, g++ (из MinGW), и добавьте их в PATH
<br>- Запускайте сборку из командной строки, например Git Bash или cmd.exe