//
//  main.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 7/26/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "serverMain.hpp"
#include "threadPool.hpp"
#include "nlohmann_json.hpp"
#include "logger.hpp"
#include "serverState.hpp"

#define NUM_THREADS_DEFAULT  8
#define VERSION_DEFAULT      1
#define BACKLOG_DEFAULT      8
#define NODE_TIMEOUT_DEFAULT 90

int NUM_THREADS;
int VERSION;
int BACKLOG;
int NODE_TIMEOUT;
std::string PRIMARY_CLUSTER_ID = "";

std::vector<gossipInfo> seed_list;

threadPool *pool;
serverMain *mainServerJob;

using json = nlohmann::json;

void
handle_sighup(int signum)
{
    /* in case we registered this handler for multiple signals */
    switch (signum) {
        case SIGINT:
        case SIGTERM:
            std::cout << "signal received" << std::endl;
            mainServerJob->teardown();
            break;
    }
}

bool
read_config(std::string config_file) {
    try {
        std::ifstream f(config_file);
        json j;
        f >> j;
        
        NUM_THREADS  = NUM_THREADS_DEFAULT;
        VERSION      = VERSION_DEFAULT;
        BACKLOG      = BACKLOG_DEFAULT;
        NODE_TIMEOUT = NODE_TIMEOUT_DEFAULT;
        
        if (j.contains("NUM_THREADS"))
            NUM_THREADS = j.at("NUM_THREADS");
        
        if (j.contains("VERSION"))
            VERSION = j.at("VERSION");
        
        if (j.contains("BACKLOG"))
            BACKLOG = j.at("BACKLOG");
        
        if (j.contains("NODE_TIMEOUT"))
            NODE_TIMEOUT = j.at("NODE_TIMEOUT");
        
        if (j.contains("PRIMARY_CLUSTER_ID"))
            PRIMARY_CLUSTER_ID = j.at("PRIMARY_CLUSTER_ID");
        
        if (j.contains("SEED_LIST")) {
            for (const auto& elem : j["SEED_LIST"])
            {
                seed_list.push_back(gossipInfo(elem["node_id"]["ip_addr"],
                                               elem["node_id"]["port"],
                                               elem["cluster_id"],
                                               elem["version"],
                                               0,
                                               STATE_UNKNOWN));
            }
        }
        return true;
    }
    catch (const std::exception& e) {
        // Logging hasn't been initialized yet. Log to console.
        std::cout << "Invalid config file" << e.what() << std::endl;
    }
    return false;
}

void
init() {
    pool = new threadPool(NUM_THREADS);
    initLog("/Users/pbagur/serverLog" + std::to_string(::getpid()), TRACE);
    mainServerJob = new serverMain(BACKLOG, pool);
    
    signal(SIGINT, handle_sighup);
}


bool
init_server() {
    if (seed_list.size() == 0 && PRIMARY_CLUSTER_ID.size() == 0)
        return false;
    else if (seed_list.size() > 0)
        return initServerState(mainServerJob->getIpAddr(), mainServerJob->getPort(), VERSION, seed_list);
    else
        return initServerState(mainServerJob->getIpAddr(), mainServerJob->getPort(), VERSION, PRIMARY_CLUSTER_ID);
}

bool
process() {
    bool rval = true;
    
    if (!mainServerJob->init())
        rval = false;
    else if (!init_server())
        rval = false;
    else if (!mainServerJob->process())
        rval = false;
    
    return rval;
}

void
teardown() {
    delete mainServerJob;
    
    pool->teardown();
    delete pool;
}

int
main(int argc, const char * argv[]) {
    int rval = 0;
    std::string config_name = "/Users/pbagur/serverConfig.json";
    
    /* TODO Bounds check */
    if (argc == 3 && strcmp(argv[1], "-c") == 0) {
        config_name = argv[2];
    }
    
    std::cout << config_name << std::endl;
    if (!read_config(config_name))
        rval = -EINVAL;
    else {
        init();
        rval = !!process();
        teardown();
    }
    
    return rval;
}
