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
#include <unordered_map>
#include "message.hpp"
#include "logger.hpp"
#include "clusterInfo.hpp"


class serverState {
private:
    std::unordered_map<std::string, clusterInfo *> cluster_map_table;
public:
    serverState();
    ~serverState();
    
    std::unordered_map<std::string, clusterInfo *>& getClusterMap() {
        return cluster_map_table;
    }
    
    void logState();
    void updateHeartbeat();
    clusterInfo *getCluster(std::string key);
    clusterInfo *getRandomCluster();
    friend bool initServerState(const std::string& ip_addr, const int port, const int version,
                                std::vector<gossipInfo> endpoints_from_config);
    friend bool initServerState(const std::string& ip_addr, const int port, const int version,
                                std::string primary_cluster_id);
};

bool initServerState(const std::string& ip_addr, const int port, const int version,
                     std::vector<gossipInfo> endpoints_from_config);
bool initServerState(const std::string& ip_addr, const int port, const int version, std::string primary_cluster);
serverState *getServerState();

#endif /* serverState_hpp */
