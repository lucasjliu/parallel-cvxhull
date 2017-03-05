//
//  Exception.h
//
//  by jiahuan.liu
//  10/12/2016
//

#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>
#include <string>
#include <cerrno>
#include <execinfo.h>
#include <utility>

#define _NEED_BACKTRACE 0

#define __TRY__ try{
#define __CATCH__(a) }catch(a){
#define __END__ }

class Exception : public std::exception
{
public:
    explicit Exception(const std::string& str) noexcept
    : _str(str), _code(0)
    {
    #if _NEED_BACKTRACE
        backtrace();
    #endif
    }
    
    Exception(const std::string& str, int code) noexcept
    : _code(code)
    {
        _str = str + ": " + strerror(code);
    #if _NEED_BACKTRACE
        backtrace();
    #endif
    }

    explicit Exception(std::string& str) noexcept
    : _str(std::forward<std::string>(str)), _code(0)
    {
    #if _NEED_BACKTRACE
        backtrace();
    #endif
    }
    
    Exception(std::string& str, int code) noexcept
    : _code(code)
    {
        _str = std::forward<std::string>(str) + ": " + strerror(code);
    #if _NEED_BACKTRACE
        backtrace();
    #endif
    }
    
    virtual ~Exception() noexcept
    {
    }
    
    virtual const char* what() const noexcept
    {
        return _str.c_str();
    }
    
    int getErrCode()
    {
        return _code;
    }
private:
    void backtrace()
    {
        void * array[64];
        int nSize = ::backtrace(array, 64);
        char ** symbols = backtrace_symbols(array, nSize);
        
        for (int i = 0; i < nSize; i++)
        {
            _str += symbols[i];
            _str += "\n";
        }
        free(symbols);
    }
private:
    std::string _str;
    int _code;
};

#endif//_EXCEPTION_H
