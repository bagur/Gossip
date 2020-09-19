//
//  logger.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/9/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef logger_hpp
#define logger_hpp

#include <fstream>
#include <string>
#include <stdio.h>
#include <string>
#include <queue>
#include <mutex>
#include <time.h>
#include <atomic>
#include "job.hpp"

enum level {
    ERROR,
    WARNING,
    TRACE,
    VERBOSE
};

class logInfo {
private:
    std::string data;
    std::string currentDateTime();
public:
    logInfo(std::string data);
    ~logInfo();
    std::string getData();
};

class logger
{
    // TODO: convert to a singleton?
private:
    // File for logging.
    std::ofstream logstream;
    std::mutex queue_mtx;
    std::queue<logInfo> pending_logs;
    level minLevel;
    // TODO - priority based logging
    
public:
    logger();
    ~logger();
    void log(level level, const std::string& msg);
    void initLog(const std::string& filepath, level minLevel);
    void writeToFile();
};

class loggerJob : public job {
public:
    loggerJob() {}
    ~loggerJob() {}
    bool processJob();
};

static std::atomic<bool> logs_in_queue{false};
void    log_error(std::string const& msg);
void    log_warn(std::string const& msg);
void    log_trace(std::string const& msg);
void    log_verbose(std::string const& msg);
void    initLog(const std::string& filepath, level minLevel);
bool    logsPending();
#endif /* logger_hpp */
