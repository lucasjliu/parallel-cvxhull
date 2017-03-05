//
//  UnitTest.cpp
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/04/2017
//

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <utility>

#include "UnitTest.h"

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
    auto null_task0 = [](Timer& t, int a){return a;};
    auto t0 = UnitTestFactory::create<int>(null_task0);
    t0.addCase(1, 1);
    t0.addCase(2, 2);
    t0.addCase(1, 0);
    __TRY__
    t0.run();
    __CATCH__(decltype(t0)::Exception& e)
    std::cout << e.what() << std::endl;
    __END__

    auto null_task1 = [](Timer& t, std::vector<int> v){usleep(1000); return v;};
    auto t1 = UnitTestFactory::create<std::vector<int>>(null_task1);
    t1.addCase({}, std::vector<int>({}));
    t1.addCase({1,2,3}, std::vector<int>({1,2,3}));
    t1.addCase({1,2,4}, std::vector<int>({1,2,3}));
    t1.addCase({}, std::vector<int>({}));
    __TRY__
    t1.run();
    __CATCH__(decltype(t1)::Exception& e)
    std::cout << e.what() << std::endl;
    __END__
}