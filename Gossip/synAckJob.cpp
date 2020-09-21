//
//  synAckJob.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 9/12/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "synAckJob.hpp"

synAckJob::synAckJob(messageAck *msg_ack) : ack(msg_ack) {
}

synAckJob::~synAckJob() {
    delete ack;
}

bool
synAckJob::processJob() {
    std::vector<gossipInfo> new_list_peer;
    serverState            *p_server_state = getServerState();
    clusterInfo            *cluster        = p_server_state->getCluster(ack->getSenderClusterId());
    
    if (!cluster) {
        log_error("Invalid message/cluster id" + ack->getSenderClusterId());
        return false;
    }
    else {
        if (true) {
            std::string ports = "";
            for (int i = 0; i < ack->gossip_info_new_list.size(); i++) {
                ports += std::to_string(ack->gossip_info_new_list[i].node_id.port);
                if (i < ack->gossip_info_new_list.size() - 1)
                    ports += " ";
            }
            std::cout << "Recv ACK(" + std::to_string(ack->getSenderPort()) + "): " + ports << std::endl;
        }
        
        cluster->processNodesList(ack->gossip_info_new_list, new_list_peer);
        return true;
    }
}
