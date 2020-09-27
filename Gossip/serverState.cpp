//
//  serverState.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/19/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "serverState.hpp"

serverState *
getServerState() {
    static std::unique_ptr<serverState> ss_glob = std::make_unique<serverState>();
    return ss_glob.get();
}

bool
initServerState(const std::string& ip_addr, const int port, const int version,
                std::vector<gossipInfo> endpoints_from_config) {
    serverState *pServerState = getServerState();
    
    for (gossipInfo gInfo : endpoints_from_config) {
        std::string key = gInfo.cluster_id;
        if (pServerState->cluster_map_table.count(key) == 0) {
            pServerState->cluster_map_table[key] = new clusterInfo(ip_addr, port, key, version);
            pServerState->cluster_map_table[key]->addNode(gInfo);
        }
        else {
            pServerState->cluster_map_table[key]->addNode(gInfo);
        }
    }
    
    for (auto it : pServerState->cluster_map_table)
        it.second->initState();
    
    return true;
}

bool initServerState(const std::string& ip_addr, const int port, const int version,
                     std::string primary_cluster_id) {
    getServerState()->cluster_map_table[primary_cluster_id] =
        new clusterInfo(ip_addr, port, primary_cluster_id, version);
    return true;
}

serverState::serverState() : cluster_map_table() {
}

serverState::~serverState() {
    for (auto it : cluster_map_table)
        delete it.second;
}

void
serverState::logState() {
    for (auto it : cluster_map_table)
        it.second->logNodesInfo();
}

clusterInfo *
serverState::getRandomCluster() {
    clusterInfo *cInfo = nullptr;
    /*
     * TODO: locking - Do we need to? will we be updating the hash
     * table at any point?
     */
    if (cluster_map_table.size() > 0) {
        srand((unsigned)time(0));
        auto it = cluster_map_table.begin();
        int random_index = rand() % cluster_map_table.size();
        std::advance(it, random_index);
        
        cInfo = it->second;
    }
    
    return cInfo;
}

clusterInfo *
serverState::getCluster(std::string key) {
    if (cluster_map_table.count(key) > 0)
        return cluster_map_table[key];
    else
        return nullptr;
}

void serverState::updateHeartbeat() {
    for (auto it : cluster_map_table)
        it.second->updateState();
}
