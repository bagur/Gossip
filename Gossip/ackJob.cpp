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
    if (!sendData(ack_msg.getSenderIPAddr(), ack_msg.getSenderPort(), j_msg)) {
        log_error("failed to send ACK to " + ack_msg.getSenderIPAddr() + ":" + std::to_string(ack_msg.getSenderPort()));
        return false;
    }
    
    return true;
}

bool
ackJob::processJob() {
    std::vector<gossipInfo> new_list_peer;
    serverState            *p_server_state = getServerState();
    
    p_server_state->processNewStateList(syn->gossip_info_list, new_list_peer);
    
    if (!sendAckMessage(new_list_peer))
        return false;
    return true;
}
