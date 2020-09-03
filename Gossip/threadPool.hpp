//
//  threadPool.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/1/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef threadPool_hpp
#define threadPool_hpp

#include <stdio.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include "job.hpp"
#include "logger.hpp"
#include "synJob.hpp"


class threadPoolArg {
public:
    job* curJob;
    threadPoolArg(job* curJob);
    ~threadPoolArg();
};

class threadPool
{
private:
    std::queue<threadPoolArg *> arg_list;
    std::mutex arg_list_mtx;
    std::condition_variable wait_queue;
    bool terminate;
    unsigned int num_threads;
    std::vector<std::thread> threads_vec;
    std::thread timer_thread;

    bool wait_function();
    void process_loop();
    void timer_loop();
public:
    threadPool(unsigned int num_threads);
    ~threadPool();
    void teardown();
    void add_job(threadPoolArg* arg);
};

#endif /* threadPool_hpp */
