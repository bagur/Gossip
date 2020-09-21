//
//  synJob.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/25/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "synJob.hpp"

synJob::synJob() : clientJob() {
}

synJob::~synJob() {
}


bool
synJob::sendSyn(messageSyn& msg, int idx) {
    std::string ip_addr = msg.gossip_info_list[idx].node_id.ip_addr;
    int         port    = msg.gossip_info_list[idx].node_id.port;
    bool        rval    = true;
    
    try {
        json        j_msg   = msg;
        
        if (!sendData(ip_addr, port, j_msg)) {
            log_error("failed to send SYN to " + ip_addr + ":" + std::to_string(port));
            rval = false;
        }
    }
    catch(...) {
        log_error("failed to send SYN to " + ip_addr + ":" + std::to_string(port));
        rval = false;
    }
    return rval;
}

bool
synJob::processJob() {
    messageSyn          syn_msg;
    bool                rval           = false;
    serverState        *p_server_state = getServerState();
    clusterInfo        *cluster        = p_server_state->getRandomCluster();
    
    /* Binary search for myInfo and syn heartbeat value */
    if (!!cluster) {
        syn_msg.setSenderIPAddr(cluster->getInfo().node_id.ip_addr);
        syn_msg.setSenderPort(cluster->getInfo().node_id.port);
        syn_msg.setSenderClusterId(cluster->getInfo().cluster_id);
        syn_msg.gossip_info_list = cluster->getNodesList();
        
        if (syn_msg.gossip_info_list.size() <= 1)
            return false;
        else {
            srand((unsigned)time(0));
            for (int i = 0; i < 1; i++) {
            retry:
                int idx = rand() % syn_msg.gossip_info_list.size();
                if (syn_msg.gossip_info_list[idx].node_id.ip_addr == syn_msg.getSenderIPAddr() &&
                    syn_msg.gossip_info_list[idx].node_id.port == syn_msg.getSenderPort())
                    goto retry;
                rval = sendSyn(syn_msg, idx);
            }
        }
    }
    
    return rval;
}
