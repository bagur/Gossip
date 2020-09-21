//
//  messageGossip.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/16/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef messageSyn_hpp
#define messageSyn_hpp

#include <stdio.h>
#include <vector>
#include "message.hpp"
#include "nlohmann_json.hpp"

using json = nlohmann::json;

class messageSyn : public message {
public:
    std::vector<gossipInfo> gossip_info_list;
    
    messageSyn() : message(MSG_SYN) {
    }
    
    messageSyn(const messageSyn &obj) : message(MSG_SYN) {
        this->setSenderIPAddr(obj.getSenderIPAddr());
        this->setSenderPort(obj.getSenderPort());
        this->setSenderClusterId(obj.getSenderClusterId());
        this->gossip_info_list = obj.gossip_info_list;
    }
    
    ~messageSyn() {
    }
};

void to_json(json& j, const messageSyn& msg);
void from_json(const json& j, messageSyn& msg);

class messageAck : public message {
public:
    std::vector<gossipInfo> gossip_info_new_list;
    messageAck() : message(MSG_ACK) {
    }
    ~messageAck() {
    }
};

void to_json(json& j, const messageAck& msg);
void from_json(const json& j, messageAck& msg);


#endif /* messageSyn_hpp */
