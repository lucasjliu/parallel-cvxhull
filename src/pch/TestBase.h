#include <chrono>
#include <functional>
#include <vector>

#include "Exception.h"
#include "Logger.h"

class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using time_t= std::chrono::time_point<clock_t>;
public:
    Timer();
    void start();
    int stop();
    void pause();
    void resume();
private:
    time_t _beg;
    time_t _pause;
    bool _started;
    bool _paused;
};

void testTimer();

template<typename ret_type>
class UnitTest
{
    using Task = std::function<void()>;
    using Test = std::function<ret_type(Timer&)>;
public:
    struct Exception: public ::Exception
    {
        Exception(const std::string& strerr) : ::Exception(strerr) {}
        Exception(const std::string& strerr, int ierr)
        : ::Exception(strerr, ierr) {}
        ~Exception() noexcept {}
    };
public:
    void run();
    void addCase(Test& test, ret_type& truth = ret_type());
private:
    std::vector<Test> _tests;
    std::vector<ret_type> _truths;
    Timer _timer;
};

template<typename ret_type>
void UnitTest<ret_type>::run()
{
    assert(_tests.size() == _truths.size());
    for (int i = 0; i < _tests.size(); ++i)
    {
        _timer.start();
        ret_type result = _tests[i](_timer);
        int cost = _timer.stop();
        if (!(result == _truths[i]))
        {
            char err_msg[40] = {0};
            sprintf(err_msg, "Case #%d fails: %d ms", i, cost);
            throw Exception(err_msg);
        }
        else
        {
            printf("Case #%d OK: %d ms\n", cost);
        }
    }
}

template<typename ret_type>
void UnitTest<ret_type>::addCase(Test& test, ret_type& truth)
{
    _tests.push_back(std::forward<Test>(test));
    _truths.push_back(std::forward<ret_type>(truth));
}

void testUnitTest();