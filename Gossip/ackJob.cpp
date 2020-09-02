//
//  AckJob.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 9/2/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "ackJob.hpp"

ackJob::ackJob(json data) {
    syn = data;
}

ackJob::~ackJob() {
}

bool
ackJob::processJob() {
    std::vector<gossipInfo> new_list_peer;
    serverState            *p_server_state = getServerState();
    
    p_server_state->processNewStateList(syn.gossip_info_list, new_list_peer);
    
    
    return true;
}
