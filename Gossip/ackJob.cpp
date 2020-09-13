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
ackJob::sendAckMessage(std::vector<gossipInfo> new_list_peer) {
    messageAck          ack_msg;
    serverState        *p_server_state = getServerState();
    
    ack_msg.setSenderIPAddr(p_server_state->getInfo().node_id.ip_addr);
    ack_msg.setSenderPort(p_server_state->getInfo().node_id.port);
    ack_msg.gossip_info_new_list = new_list_peer;
    
    json j_msg = ack_msg;
    if (!sendData(syn->getSenderIPAddr(), syn->getSenderPort(), j_msg)) {
        log_error("failed to send ACK to " + syn->getSenderIPAddr() + ":" + std::to_string(syn->getSenderPort()));
        return false;
    }
    
    return true;
}

bool
ackJob::processJob() {
    std::vector<gossipInfo> new_list_peer;
    serverState            *p_server_state = getServerState();
    
    if (true) {
        std::string ports = "";
        for (int i = 0; i < syn->gossip_info_list.size(); i++) {
            ports += std::to_string(syn->gossip_info_list[i].node_id.port);
            if (i < syn->gossip_info_list.size() - 1)
                ports += " ";
        }
        std::cout << "Recv SYN(" + std::to_string(syn->getSenderPort()) + "): " + ports << std::endl;
    }
    
    p_server_state->processNewStateList(syn->gossip_info_list, new_list_peer);
    
    if (!sendAckMessage(new_list_peer))
        return false;
    return true;
}
