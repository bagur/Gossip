//
//  threadPool.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/1/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "threadPool.hpp"


threadPool::threadPool(unsigned int num_threads) :
arg_list(), arg_list_mtx(),
terminate(false), num_threads(num_threads),
threads_vec() {
    for (int i = 0; i < num_threads; i++) {
        threads_vec.push_back(std::thread(&threadPool::process_loop, this));
    }
    timer_thread = std::thread(&threadPool::timer_loop, this);
}

threadPool::~threadPool() {
    timer_thread.join();
    for (int i = 0; i < num_threads; i++) {
        threads_vec[i].join();
    }
}

void
threadPool::teardown() {
    std::unique_lock<std::mutex> arg_lock(arg_list_mtx);
    terminate = true;
    arg_lock.unlock();
    wait_queue.notify_all();
}

bool
threadPool::wait_function() {
    // arg_list_mtx must be held by the caller
    return terminate || !arg_list.empty();
}

void
threadPool::process_loop() {
    while (true) {
        threadPoolArg *arg;
        {
            /*
             * When the unique_lock is constructed it will lock the mutex,
             * and it gets destructed it will unlock the mutex.
             */
            std::unique_lock<std::mutex> arg_lock(arg_list_mtx);
            
            /* lock held by the thread */
            
            /* pass the lock to the wait queue */
            wait_queue.wait(arg_lock, [this]() {return wait_function();});
            if (terminate && arg_list.empty()) {
                /* arg_lock will be released automatically on exiting the scope */
                return;
            }
            
            /* pick the first item on the queue */
            arg = arg_list.front();
            arg_list.pop();
            /* arg_lock will be released automatically on exiting the scope */
        }
        arg->curJob->processJob();
        delete arg;
    }
}

void
threadPool::timer_loop() {
    int nticks = 0;
    while (!terminate) {
        nticks++;
        //getServerState()->incHeartbeat();
        this->add_job(new threadPoolArg(new synJob()));
        
        if (logsPending()) {
            this->add_job(new threadPoolArg(new loggerJob()));
        }
        //getServerState()->logInfo();
        
        if (nticks % 20 == 0) {
            this->add_job(new threadPoolArg(new membershipJob()));
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}

void
threadPool::add_job(threadPoolArg* arg) {
    std::unique_lock<std::mutex> arg_lock(arg_list_mtx);
    arg_list.push(arg);
    arg_lock.unlock();
    wait_queue.notify_one();
}

threadPoolArg::threadPoolArg(job *curJob) : curJob(curJob) {
}

threadPoolArg::~threadPoolArg() {
    delete curJob;
}

