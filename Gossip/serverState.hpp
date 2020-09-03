//
//  serverState.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/19/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef serverState_hpp
#define serverState_hpp

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <mutex>
#include "message.hpp"
#include "logger.hpp"

#define STATE_UNKNOWN    0
#define STATE_BOOTSTRAP  1
#define STATE_INITED     2
#define STATE_NORMAL     3
#define STATE_TEARDOWN   4

static const char *state_str[] = {
    "STATE_UNKNOWN",
    "STATE_BOOTSTRAP",
    "STATE_INITED",
    "STATE_NORMAL",
    "STATE_TEARDOWN"
};

class serverState {
private:
    gossipInfo my_info;
    std::mutex global_state_list_mtx;
    std::vector<gossipInfo> global_state_list;
public:
    serverState();
    ~serverState();
    void init(const std::string& ip_addr, const int port, const int version);
    std::vector<gossipInfo> getGlobalStateList();
    void processNewStateList(std::vector<gossipInfo> peer_list, std::vector<gossipInfo>& peer_list_new);
    gossipInfo getInfo();
    void logInfo();
    void incHeartbeat();
    friend bool initServerState(const std::string& ip_addr, const int port, const int version,
                                std::vector<gossipInfo> endpoints_from_config);
};

bool initServerState(const std::string& ip_addr, const int port, const int version,
                     std::vector<gossipInfo> endpoints_from_config);
serverState *getServerState();

#endif /* serverState_hpp */
