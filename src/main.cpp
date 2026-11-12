// Компілятор: g++ 15.2.0 (MSYS2 UCRT64), C++20 (<future>, <syncstream>)
// Програмний проєкт 5, варіант 5: демонстрація роботи функції work().
#include "work.h"

#include <exception>
#include <iostream>

using namespace std;

int main()
{
    cout << "Program project 5, variant 5\n"
         << "Computations: A1, A2, B1, B2, C, D, F   (slow, 7 s: A1, B2; quick, 1 s: others)\n"
         << "Dependencies: A2(A1), B2(B1), F(C), D(A2, B2)\n"
         << "\n"
         << "Schedule:\n"
         << "  main: A1 -> A2 -> wait B2 -> D -> wait T1\n"
         << "  T1:   B1 -> B2 -> signal B2 -> wait C -> F\n"
         << "  T2:   C\n"
         << "Expected: ~9 s; 2 threads launched besides main;\n"
         << "          3 threads work at once only during 0..1 s, then at most 2.\n"
         << "\n"
         << "  time  thread  computation\n";

    try
    {
        work();
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
