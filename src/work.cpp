#include "work.h"

#include <exception>
#include <future>
#include <iomanip>
#include <iostream>
#include <syncstream>
#include <thread>
#include <utility>

using namespace std;
using namespace std::chrono;

namespace
{
constexpr seconds kSlowDuration{7};
constexpr seconds kQuickDuration{1};

steady_clock::time_point g_workStart = steady_clock::now();

thread_local string t_threadName = "main";

double secondsSinceWorkStart()
{
    return duration<double>(steady_clock::now() - g_workStart).count();
}
}

void compute(const string& name, seconds duration)
{
    this_thread::sleep_for(duration);

    osyncstream out(cout);
    out << fixed << setprecision(1) << setw(6) << secondsSinceWorkStart() << "  "
        << left << setw(8) << t_threadName << name << '\n';
}

void slow(const string& name)
{
    compute(name, kSlowDuration);
}

void quick(const string& name)
{
    compute(name, kQuickDuration);
}

void work()
{
    const auto start = steady_clock::now();
    g_workStart = start;

    future<void> cDone = async(launch::async, [] {
        t_threadName = "T2";
        quick("C");
    });

    promise<void> b2Promise;
    future<void> b2Done = b2Promise.get_future();

    future<void> t1 = async(launch::async, [&b2Promise, cDone = std::move(cDone)]() mutable {
        t_threadName = "T1";
        try
        {
            quick("B1");
            slow("B2");
        }
        catch (...)
        {
            b2Promise.set_exception(current_exception());
            throw;
        }
        b2Promise.set_value();

        cDone.get();
        quick("F");
    });

    slow("A1");
    quick("A2");
    b2Done.get();
    quick("D");
    t1.get();

    const duration<double> elapsed = steady_clock::now() - start;
    osyncstream out(cout);
    out << "\nExecution time: " << fixed << setprecision(2) << elapsed.count() << " s\n"
        << "Work is done!\n";
}
