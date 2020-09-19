//
//  logger.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/9/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "logger.hpp"


inline logger *getLog() {
    static std::unique_ptr<logger> log_glob = std::make_unique<logger>();
    return log_glob.get();
}

void
initLog(const std::string& filepath, level minLevel) {
    logger *pLog = getLog();
    pLog->initLog(filepath, minLevel);
}

void
log_error(std::string const& msg) {
    getLog()->log(level::ERROR, msg);
}

void
log_warn(std::string const& msg) {
    getLog()->log(level::WARNING, msg);
}

void
log_trace(std::string const& msg) {
    getLog()->log(level::TRACE, msg);
}

void
log_verbose(std::string const& msg) {
    getLog()->log(level::VERBOSE, msg);
}

bool
logsPending() {
    bool tmp = true;
    if (!!logs_in_queue.compare_exchange_weak(tmp, false)) {
        return true;
    }
    
    return false;
}

logger::logger() : logstream(), queue_mtx(), pending_logs(), minLevel(TRACE) {
}

logger::~logger() {
    log(level::TRACE, "Stopped logging..");
    logstream.close();
}

void logger::initLog(const std::string &filepath, level minLevel) {
    this->minLevel = minLevel;
    logstream.open(filepath);
}

void logger::log(level level, const std::string &msg) {
    if (level <= minLevel) {
        /*
         * TODO hash table with a (queue + lock) in each slot
         * to reduce contention b/w writers
         */
        queue_mtx.lock();
        pending_logs.push(logInfo(msg));
        queue_mtx.unlock();
        // We don't care if two threads enter writeToFile
        // ocasionally
        logs_in_queue = true;
    }
}

void logger::writeToFile() {
    queue_mtx.lock();
    if (logstream.is_open()) {
        //logstream << msg << std::endl;
        while (!pending_logs.empty()) {
            logInfo& logEntry = pending_logs.front();
            pending_logs.pop();
            logstream << logEntry.getData() << std::endl;
        }
    }
    queue_mtx.unlock();
}

logInfo::logInfo(std::string data) {
    this->data = currentDateTime() + " " + data;
}

logInfo::~logInfo() {
}

std::string logInfo::getData() {
    return data;
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
std::string logInfo::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    
    return buf;
}

bool loggerJob::processJob() {
    getLog()->writeToFile();
    return true;
}
