//
//  messageGossip.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/16/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "messageGossip.hpp"

void to_json(json& j, const messageSyn& msg) {
    j = json{{"opcode", msg.getOpcode()}, {"opcode_str", msg.getOpcodeStr()}};
    for (int i = 0; i < msg.gossip_info_list.size(); i++) {
        j["gossip_info_list"].push_back({
            {"node_id", {
                {"ip_addr", msg.gossip_info_list[i].node_id.ip_addr},
                {"port", msg.gossip_info_list[i].node_id.port}
            }},
            {"version", msg.gossip_info_list[i].version},
            {"heartbeat", msg.gossip_info_list[i].heartbeat},
            {"state", msg.gossip_info_list[i].state}
        });
    }
}

void from_json(const json& j, messageSyn& msg) {
    msg.setOpcode(j.at("opcode"));
    msg.setOpcodeStr(j.at("opcode_str"));
    
    for (const auto& elem : j["gossip_info_list"])
    {
        msg.gossip_info_list.push_back(gossipInfo(elem["node_id"]["ip_addr"],
                                                  elem["node_id"]["port"],
                                                  elem["version"],
                                                  elem["heartbeat"],
                                                  elem["state"]));
    }
}


void to_json(json& j, const messageAck& msg) {
    j = json{{"opcode", msg.getOpcode()}, {"opcode_str", msg.getOpcodeStr()}};
    for (int i = 0; i < msg.gossip_info_new_list.size(); i++) {
        j["gossip_info_new_list"].push_back({
            {"node_id", {
                {"ip_addr", msg.gossip_info_new_list[i].node_id.ip_addr},
                {"port", msg.gossip_info_new_list[i].node_id.port}
            }},
            {"version", msg.gossip_info_new_list[i].version},
            {"heartbeat", msg.gossip_info_new_list[i].heartbeat},
            {"state", msg.gossip_info_new_list[i].state}
        });
    }
    
    for (int i = 0; i < msg.gossip_info_needed_list.size(); i++) {
        j["gossip_info_needed_list"].push_back({
            {"node_id", {
                {"ip_addr", msg.gossip_info_needed_list[i].node_id.ip_addr},
                {"port", msg.gossip_info_needed_list[i].node_id.port}
            }},
            {"version", msg.gossip_info_needed_list[i].version},
            {"heartbeat", msg.gossip_info_needed_list[i].heartbeat},
            {"state", msg.gossip_info_needed_list[i].state}
        });
    }
}

void from_json(const json& j, messageAck& msg) {
    msg.setOpcode(j.at("opcode"));
    msg.setOpcodeStr(j.at("opcode_str"));
    
    for (const auto& elem : j["gossip_info_new_list"])
    {
        msg.gossip_info_new_list.push_back(gossipInfo(elem["node_id"]["ip_addr"],
                                                      elem["node_id"]["port"],
                                                      elem["version"],
                                                      elem["heartbeat"],
                                                      elem["state"]));
    }
    
    for (const auto& elem : j["gossip_info_needed_list"])
    {
        msg.gossip_info_needed_list.push_back(gossipInfo(elem["node_id"]["ip_addr"],
                                                         elem["node_id"]["port"],
                                                         elem["version"],
                                                         elem["heartbeat"],
                                                         elem["state"]));
    }
}

void to_json(json& j, const messageAck2& msg) {
    j = json{{"opcode", msg.getOpcode()}, {"opcode_str", msg.getOpcodeStr()}};
    for (int i = 0; i < msg.gossip_info_needed_reply_list.size(); i++) {
        j["gossip_info_needed_reply_list"].push_back({
            {"node_id", {
                {"ip_addr", msg.gossip_info_needed_reply_list[i].node_id.ip_addr},
                {"port", msg.gossip_info_needed_reply_list[i].node_id.port}
            }},
            {"version", msg.gossip_info_needed_reply_list[i].version},
            {"heartbeat", msg.gossip_info_needed_reply_list[i].heartbeat},
            {"state", msg.gossip_info_needed_reply_list[i].state}
        });
    }
}

void from_json(const json& j, messageAck2& msg) {
    msg.setOpcode(j.at("opcode"));
    msg.setOpcodeStr(j.at("opcode_str"));
    
    for (const auto& elem : j["gossip_info_needed_reply_list"])
    {
        msg.gossip_info_needed_reply_list.push_back(gossipInfo(elem["node_id"]["ip_addr"],
                                                               elem["node_id"]["port"],
                                                               elem["version"],
                                                               elem["heartbeat"],
                                                               elem["state"]));
    }
}