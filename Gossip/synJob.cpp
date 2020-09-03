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
    json        j_msg   = msg;
    
    if (!sendData(ip_addr, port, j_msg)) {
        log_error("failed to send SYN to " + ip_addr + ":" + std::to_string(port));
        return false;
    }
    
    return true;
}

bool
synJob::processJob() {
    messageSyn          syn_msg;
    serverState        *p_server_state = getServerState();
    
    syn_msg.setSenderIPAddr(p_server_state->getInfo().node_id.ip_addr);
    syn_msg.setSenderPort(p_server_state->getInfo().node_id.port);
    syn_msg.gossip_info_list = p_server_state->getGlobalStateList();
    
    for (int i = 0; i < syn_msg.gossip_info_list.size(); i++) {
        if (syn_msg.gossip_info_list[i].getKey() == p_server_state->getInfo().getKey()) {
            syn_msg.gossip_info_list[i].heartbeat = p_server_state->getInfo().heartbeat;
            syn_msg.gossip_info_list[i].version   = p_server_state->getInfo().version;
            syn_msg.gossip_info_list[i].state     = p_server_state->getInfo().state;
            break;
        }
    }
    
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
            sendSyn(syn_msg, idx);
        }
    }
    
    return true;
}
