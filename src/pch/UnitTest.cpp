//
//  UnitTest.cpp
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/04/2017
//

#include <chrono>

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