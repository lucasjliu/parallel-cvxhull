#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <utility>

#include "TestBase.h"

Timer::Timer() :_started(0), _paused(0) {}

void Timer::start()
{
    _started = true;
    _beg = clock_t::now();
}

int Timer::stop()
{
    int res = !_started ? 0 :
        std::chrono::duration_cast<std::chrono::milliseconds>(clock_t::now() - _beg).count();
    _started = false;
    return res;
}

void Timer::pause()
{
    if (_started)
    {
        _pause = clock_t::now();
        _paused = true;
    }
}

void Timer::resume()
{
    if (_started && _paused)
    {
        _paused = false;
        _beg += clock_t::now() - _pause;
    }
}

void testTimer()
{
    Timer t1;
    t1.start();
    assert(t1.stop() == 0);

    t1.start();
    usleep(1000);
    assert(t1.stop() == 1);

    t1.start();
    usleep(1000);
    t1.pause();
    usleep(5000);
    t1.resume();
    usleep(1000);
    assert(t1.stop() == 2);
}

void testUnitTest()
{
    auto null_task0 = [](int a, Timer t){return a;};
    UnitTest<int> t0;
    t0.add(1, 1);
    t0.add(1, 0);
    t0.add(2, 2);
    __TRY__
    t0.run();
    __CATCH__(UnitTest::Exception& e)
    std::cout << e.what() << std::endl;
    __END__


    auto null_task1 = [](std::vector<int> v, Timer t){return v;};
    UnitTest<std::vector<int>> t1;
    test.addCase({})
}