//
//  clusterInfo.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 9/20/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef clusterInfo_hpp
#define clusterInfo_hpp

#include <stdio.h>
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

class clusterInfo {
private:
    gossipInfo my_info;
    std::string cluster_id;
    std::mutex nodes_list_mtx;
    std::vector<gossipInfo> nodes_list;
    
public:
    clusterInfo(std::string ip_addr, int port, std::string cluster_id, int version);
    ~clusterInfo();
    void initState();
    void updateState();
    gossipInfo& getInfo();
    std::vector<gossipInfo> getNodesList();
    void addNode(gossipInfo node);
    void processNodesList(std::vector<gossipInfo> peer_list, std::vector<gossipInfo>& peer_list_new);
    void logNodesInfo();
};
#endif /* clusterInfo_hpp */
