# Програмний проєкт 5 - варіант 5

Виконання обчислень A1, A2, B1, B2, C, D, F у головному потоці та ще 2 потоках, запущених через `std::async`.
«Повільні» обчислення (7 с): A1, B2; решта «швидкі» (1 с). Залежності: A2(A1), B2(B1), F(C), D(A2, B2).
Найдовші ланцюжки A1→A2→D і B1→B2→D тривають по 9 с, тож мінімальний час - 9 с, і програма його досягає.

## Вимоги

- компілятор з підтримкою C++20 (перевірено на g++ 15.2.0, MSYS2 UCRT64);
- CMake 3.20+.

## Збирання

З папки проєкту:

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

Без CMake:

```powershell
g++ -std=c++20 -O2 -Wall -Wextra -Wpedantic -static src/main.cpp src/work.cpp -o work_demo.exe
```

## Запуск

```powershell
build\work_demo.exe
```

Записати вивід у файл:

```powershell
build\work_demo.exe > results.txt
```

## Що виводить програма

```text
  time  thread  computation
   1.0  T2      C
   1.0  T1      B1
   7.0  main    A1
   8.0  T1      B2
   8.0  main    A2
   9.0  T1      F
   9.0  main    D

Execution time: 9.04 s
Work is done!
```

Кожне обчислення друкується рівно один раз, одразу після завершення: секунда від початку роботи,
потік і назва. Порядок рядків у межах однієї секунди може відрізнятися між запусками,
бо потоки завершують обчислення майже одночасно.

## Як це працює

- `T2` запускається через `std::async` і виконує C; його `future` передається в `T1`,
  який викликає `get()` перед F.
- `T1` після B2 викликає `set_value()` на `promise`, а `main` чекає на пов'язаний `future` перед D.
- Наприкінці `main` викликає `get()` на `future` потоку `T1`.
- Вивід синхронізовано через `std::osyncstream`.

| Потік | Послідовність | Час, с |
| --- | --- | --- |
| main | A1, A2, D | 0–9 |
| T1 | B1, B2, F | 0–9 |
| T2 | C | 0–1 |

Діаграма розподілу і точок синхронізації: [docs/schedule.png](docs/schedule.png) / [docs/schedule.svg](docs/schedule.svg).

## Файли

- `src/main.cpp` - демонстрація роботи `work()`;
- `src/work.h`, `src/work.cpp` - функції `slow`, `quick`, `work`;
- `CMakeLists.txt` - збирання;
- `results.txt` - вивід програми;
- `docs/schedule.png`, `docs/schedule.svg` - діаграма розподілу обчислень.
