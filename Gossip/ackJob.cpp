//
//  AckJob.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 9/2/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "ackJob.hpp"

ackJob::ackJob(messageSyn *msg_syn) : syn(msg_syn) {
}

ackJob::~ackJob() {
    delete syn;
}

bool
ackJob::sendAckMessage(clusterInfo *cluster, std::vector<gossipInfo> new_list_peer) {
    messageAck          ack_msg;
    bool                rval           = true;
    
    ack_msg.setSenderIPAddr(cluster->getInfo().node_id.ip_addr);
    ack_msg.setSenderPort(cluster->getInfo().node_id.port);
    ack_msg.setSenderClusterId(cluster->getInfo().cluster_id);
    ack_msg.gossip_info_new_list = new_list_peer;
    
    try {
        json j_msg = ack_msg;
        if (!sendData(syn->getSenderIPAddr(), syn->getSenderPort(), j_msg)) {
            log_error("failed to send ACK to " + syn->getSenderIPAddr() + ":" + std::to_string(syn->getSenderPort()));
            rval = false;
        }
    }
    catch (...) {
        log_error("failed to send ACK to " + syn->getSenderIPAddr() + ":" + std::to_string(syn->getSenderPort()));
        rval = false;
    }
    
    return rval;
}

bool
ackJob::processJob() {
    std::vector<gossipInfo> new_list_peer;
    serverState            *p_server_state = getServerState();
    clusterInfo            *cluster        = p_server_state->getCluster(syn->getSenderClusterId());
    
    if (!cluster) {
        log_error("Invalid message/cluster id" + syn->getSenderClusterId());
        return false;
    }
    else {
        
        if (true) {
            std::string ports = "";
            for (int i = 0; i < syn->gossip_info_list.size(); i++) {
                ports += std::to_string(syn->gossip_info_list[i].node_id.port);
                if (i < syn->gossip_info_list.size() - 1)
                    ports += " ";
            }
            std::cout << "Recv SYN(" + std::to_string(syn->getSenderPort()) + "): " + ports << std::endl;
        }
        
        cluster->processNodesList(syn->gossip_info_list, new_list_peer);
        if (!sendAckMessage(cluster, new_list_peer))
            return false;
        return true;
    }
}
