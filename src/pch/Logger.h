//
//  Logger.h
//
//  by jiahuan.liu
//  10/12/2016
//

#ifndef _LOGGER_H
#define _LOGGER_H

#include <iostream>
#include <sstream>
#include <functional>
#include <string>

//#include "Common.h"
#include "Exception.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

typedef std::ostringstream Stream;

#define LOG std::cout
#define END std::endl

class Logger
{
public:
    //output file error
    struct Exception: public ::Exception
    {
        Exception(const std::string& strerr) : ::Exception(strerr) {}
        Exception(const std::string& strerr, int ierr)
        : ::Exception(strerr, ierr) {}
        ~Exception() noexcept {}
    };
public:
    enum Level
    {
        L_DEBUG,
        L_INFO,
        L_WARN,
        L_ERROR,
        L_FATAL,
        L_DATA
    };
    static std::string levelName[];
    //log to default output
    Logger(std::string file, int line, Level level = L_INFO);
    Logger(std::string file, int line, std::string func, Level level);
    //log to file output
    Logger(FILE* ofile, std::string file, int line, Level level = L_INFO);
    Logger(FILE* ofile, std::string file, int line, std::string func, Level level);
    Logger(FILE* ofile);
    ~Logger();
    //encode beginning and ending of a log line
    void init(Level level, std::string file, int line);
    void finish();
    
    Stream& stream() {return _stream;}
protected:
    //stdout
    static void defaultOutput(const std::string& str);
    //throw if file not opened or error
    static void fileOutput(FILE* file, const std::string& str);
protected:
    Stream _stream;
    std::function<void(const std::string&)> _output;
};

#define LOG_DEBUG Logger(__FILENAME__, __LINE__, __func__, Logger::Level::L_DEBUG).stream()
#define LOG_INFO Logger(__FILENAME__, __LINE__, Logger::Level::L_INFO).stream()
#define LOG_WARN Logger(__FILENAME__, __LINE__, Logger::Level::L_WARN).stream()
#define LOG_ERROR Logger(__FILENAME__, __LINE__, Logger::Level::L_ERROR).stream()
#define LOG_FATAL Logger(__FILENAME__, __LINE__, Logger::Level::L_FATAL).stream()

#define FLOG_DEBUG(x) Logger(x, __FILENAME__, __LINE__, __func__, Logger::Level::L_DEBUG).stream()
#define FLOG_INFO(x) Logger(x, __FILENAME__, __LINE__, Logger::Level::L_INFO).stream()
#define FLOG_WARN(x) Logger(x, __FILENAME__, __LINE__, Logger::Level::L_WARN).stream()
#define FLOG_ERROR(x) Logger(x, __FILENAME__, __LINE__, Logger::Level::L_ERROR).stream()
#define FLOG_FATAL(x) Logger(x, __FILENAME__, __LINE__, Logger::Level::L_FATAL).stream()
#define FLOG_DATA(x) Logger(x).stream()

class Common
{
public:
    static std::string now2str(const std::string &sFormat = "%Y%m%d%H%M%S");
};

#endif//_LOGGER_H
