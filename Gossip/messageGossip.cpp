//
//  messageGossip.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/16/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "messageGossip.hpp"

void to_json(json& j, const messageSyn& msg) {
    j = json{{"opcode", msg.getOpcode()}, {"opcode_str", msg.getOpcodeStr()}, {"sender_ip_addr", msg.getSenderIPAddr()}, {"sender_port", msg.getSenderPort()}, {"sender_cluster_id", msg.getSenderClusterId()}, {"sender_key", msg.getSenderKey()}};
    for (int i = 0; i < msg.gossip_info_list.size(); i++) {
        j["gossip_info_list"].push_back({
            {"node_id", {
                {"ip_addr", msg.gossip_info_list[i].node_id.ip_addr},
                {"port", msg.gossip_info_list[i].node_id.port}
            }},
            {"cluster_id", msg.gossip_info_list[i].cluster_id},
            {"version", msg.gossip_info_list[i].version},
            {"heartbeat", msg.gossip_info_list[i].heartbeat},
            {"state", msg.gossip_info_list[i].state}
        });
    }
}

void from_json(const json& j, messageSyn& msg) {
    msg.setOpcode(j.at("opcode"));
    msg.setOpcodeStr(j.at("opcode_str"));
    msg.setSenderIPAddr(j.at("sender_ip_addr"));
    msg.setSenderPort(j.at("sender_port"));
    msg.setSenderClusterId(j.at("sender_cluster_id"));
    msg.setSenderKey(j.at("sender_key"));
    
    if (j.contains("gossip_info_list")) {
        for (const auto& elem : j["gossip_info_list"])
        {
            msg.gossip_info_list.push_back(gossipInfo(elem["node_id"]["ip_addr"],
                                                      elem["node_id"]["port"],
                                                      elem["cluster_id"],
                                                      elem["version"],
                                                      elem["heartbeat"],
                                                      elem["state"]));
        }
    }
}


void to_json(json& j, const messageAck& msg) {
    j = json{{"opcode", msg.getOpcode()}, {"opcode_str", msg.getOpcodeStr()}, {"sender_ip_addr", msg.getSenderIPAddr()}, {"sender_port", msg.getSenderPort()}, {"sender_cluster_id", msg.getSenderClusterId()}, {"sender_key", msg.getSenderKey()}};
    for (int i = 0; i < msg.gossip_info_new_list.size(); i++) {
        j["gossip_info_new_list"].push_back({
            {"node_id", {
                {"ip_addr", msg.gossip_info_new_list[i].node_id.ip_addr},
                {"port", msg.gossip_info_new_list[i].node_id.port}
            }},
            {"cluster_id", msg.gossip_info_new_list[i].cluster_id},
            {"version", msg.gossip_info_new_list[i].version},
            {"heartbeat", msg.gossip_info_new_list[i].heartbeat},
            {"state", msg.gossip_info_new_list[i].state}
        });
    }
}

void from_json(const json& j, messageAck& msg) {
    msg.setOpcode(j.at("opcode"));
    msg.setOpcodeStr(j.at("opcode_str"));
    msg.setSenderIPAddr(j.at("sender_ip_addr"));
    msg.setSenderPort(j.at("sender_port"));
    msg.setSenderClusterId(j.at("sender_cluster_id"));
    msg.setSenderKey(j.at("sender_key"));
    
    if (j.contains("gossip_info_new_list")) {
        for (const auto& elem : j["gossip_info_new_list"])
        {
            msg.gossip_info_new_list.push_back(gossipInfo(elem["node_id"]["ip_addr"],
                                                          elem["node_id"]["port"],
                                                          elem["cluster_id"],
                                                          elem["version"],
                                                          elem["heartbeat"],
                                                          elem["state"]));
        }
    }
}
