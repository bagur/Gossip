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

static int NUM_THREADS;
static int VERSION;
static int BACKLOG;



std::vector<gossipInfo> seed_list;

threadPool *pool;
serverMain *mainServerJob;

using json = nlohmann::json;

void handle_sighup(int signum)
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

void read_config(std::string config_file) {
    std::ifstream f(config_file);
    json j;
    f >> j;
    
    NUM_THREADS = j.at("NUM_THREADS");
    VERSION = j.at("VERSION");
    BACKLOG = j.at("BACKLOG");
    
    for (const auto& elem : j["SEED_LIST"])
    {
        seed_list.push_back(gossipInfo(elem["node_id"]["ip_addr"],
                                       elem["node_id"]["port"],
                                       elem["version"],
                                       0,
                                       STATE_UNKNOWN));
    }
}

void init() {
    pool = new threadPool(NUM_THREADS);
    initLog("/Users/pbagur/serverLog" + std::to_string(::getpid()));
    mainServerJob = new serverMain(BACKLOG, pool);
    
    signal(SIGINT, handle_sighup);
}

bool process() {
    bool rval = true;
    
    if (!mainServerJob->init())
        rval = false;
    else if (!initServerState(mainServerJob->getIpAddr(), mainServerJob->getPort(), VERSION, seed_list))
        rval = false;
    else if (!mainServerJob->process())
        rval = false;
    
    return rval;
}

void teardown() {
    delete mainServerJob;
    
    pool->teardown();
    delete pool;
}

int main(int argc, const char * argv[]) {
    int rval = 0;
    std::string config_name = "/Users/pbagur/serverConfig.json";
    
    /* TODO Bounds check */
    if (argc == 3 && strcmp(argv[1], "-c") == 0) {
        config_name = argv[2];
    }
    std::cout << config_name << std::endl;
    read_config(config_name);
    init();
    rval = !!process();
    teardown();
    return rval;
}
