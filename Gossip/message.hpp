//
//  message.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/16/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef message_hpp
#define message_hpp

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "nlohmann_json.hpp"

using json = nlohmann::json;

#define MSG_SYN  0
#define MSG_ACK  1
#define MSG_ACK2 2

static const char *message_type_str[] = {
    "MSG_SYN",
    "MSG_ACK",
    "MSG_ACK2"
};

class message {
protected:
    int opcode;
    std::string opcode_str;
    std::string sender_ip_addr;
    int sender_port;
public:
    message(int opcode) : opcode(opcode) {
        opcode_str = message_type_str[opcode];
    }
    
    ~message() {
    }
    
    int getOpcode() const {
        return opcode;
    }
    
    void setOpcode(int opcode) {
        this->opcode = opcode;
    }
    
    std::string getOpcodeStr() const {
        return opcode_str;
    }
    
    void setOpcodeStr(std::string opcode_str) {
        this->opcode_str = opcode_str;
    }
    
    std::string getSenderIPAddr() const {
        return this->sender_ip_addr;
    }
    
    void setSenderIPAddr(std::string sender_ip_addr) {
        this->sender_ip_addr = sender_ip_addr;
    }
    
    int getSenderPort() const {
        return sender_port;
    }
    
    void setSenderPort(int sender_port) {
        this->sender_port = sender_port;
    }
};

class addr {
public:
    std::string ip_addr;
    int         port;
    
    addr(std::string ip_addr, int port) : ip_addr(ip_addr), port(port) {
    }
    
    ~addr() {
    }
};

class gossipInfo {
public:
    addr node_id;
    int  version;
    long heartbeat;
    int  state;
    
    gossipInfo(std::string ip_addr, int port, int version, long heartbeat, int state) :
    node_id(ip_addr, port), version(version), heartbeat(heartbeat), state(state) {
    }
    
    ~gossipInfo() {
    }
    
    std::string getKey() const {
        return node_id.ip_addr + ":" + std::to_string(node_id.port);
    }
    
    bool operator<(gossipInfo other) const
    {
        return getKey() > other.getKey();
    }
};

#endif /* message_hpp */
