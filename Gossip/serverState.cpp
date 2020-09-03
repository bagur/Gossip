//
//  serverState.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/19/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "serverState.hpp"

serverState *
getServerState() {
    static std::unique_ptr<serverState> ss_glob = std::make_unique<serverState>();
    return ss_glob.get();
}

bool
initServerState(const std::string& ip_addr, const int port, const int version,
                std::vector<gossipInfo> endpoints_from_config) {
    serverState *pServerState = getServerState();
    pServerState->init(ip_addr, port, version);
    
    for (gossipInfo& gInfo : endpoints_from_config) {
        pServerState->global_state_list.push_back(gInfo);
    }
    pServerState->global_state_list.push_back(pServerState->my_info);
    return true;
}

serverState::serverState() : my_info("", 0, 0, 1, STATE_BOOTSTRAP), global_state_list_mtx() {
}

serverState::~serverState() {
}

void
serverState::init(const std::string &ip_addr, const int port, const int version) {
    if (this->my_info.state == STATE_BOOTSTRAP) {
        this->my_info.node_id.ip_addr = ip_addr;
        this->my_info.node_id.port = port;
        this->my_info.version = version;
        this->my_info.state = STATE_INITED;
    }
}

std::vector<gossipInfo>
serverState::getGlobalStateList() {
    return this->global_state_list;
}

void
serverState::processNewStateList(std::vector<gossipInfo> peer_list, std::vector<gossipInfo>& peer_list_new) {
    {
        int i = 0;
        int j = 0;
        std::vector<gossipInfo> new_list;
        
        std::sort(peer_list.begin(), peer_list.end());
        
        /*
         * When the unique_lock is constructed it will lock the mutex,
         * and it gets destructed it will unlock the mutex.
         */
        std::unique_lock<std::mutex> state_lock(global_state_list_mtx);
        
        /* lock held by the thread */
        std::sort(global_state_list.begin(), global_state_list.end());
        
        while (i < global_state_list.size() && j < peer_list.size()) {
            gossipInfo& cur_info  = global_state_list[i];
            gossipInfo& peer_info = peer_list[j];
            
            if (cur_info.getKey() < peer_info.getKey()) {
                // I have some information that my peer doesn't
                
                // retrive what I have
                new_list.push_back(cur_info);
                
                // Let peer know
                peer_list_new.push_back(cur_info);
                i++;
            }
            else if (cur_info.getKey() > peer_info.getKey()) {
                // peer has information that I don't
                new_list.push_back(peer_info);
                j++;
            }
            else {
                // We both have this entry. update info if necessary
                if (cur_info.version != peer_info.version) {
                    if (cur_info.version < peer_info.version)
                        new_list.push_back(peer_info);
                    else {
                        new_list.push_back(cur_info);
                        peer_list_new.push_back(cur_info);
                    }
                }
                else if (cur_info.heartbeat != peer_info.heartbeat) {
                    if (cur_info.heartbeat < peer_info.heartbeat)
                        new_list.push_back(peer_info);
                    else {
                        new_list.push_back(cur_info);
                        peer_list_new.push_back(cur_info);
                    }
                }
                else {
                    new_list.push_back(cur_info);
                }
                
                i++;
                j++;
            }
        }
        
        while (i < global_state_list.size()) {
            new_list.push_back(global_state_list[i]);
            peer_list_new.push_back(global_state_list[i]);
            i++;
        }
        
        while (j < peer_list.size()) {
            new_list.push_back(peer_list[j]);
            j++;
        }
        
        this->global_state_list.clear();
        this->global_state_list = new_list;
        
        if (this->global_state_list.size() > 1 && my_info.state == STATE_INITED)
            my_info.state = STATE_NORMAL;
    }
}

gossipInfo
serverState::getInfo() {
    return this->my_info;
}

void
serverState::incHeartbeat() {
    this->my_info.heartbeat++;
}

void
serverState::logInfo() {
    {
        std::unique_lock<std::mutex> state_lock(global_state_list_mtx);
        for (gossipInfo& info : global_state_list) {
            log_trace(info.getKey() + " " + std::to_string(info.heartbeat));
            std::cout << info.getKey() + " " + std::to_string(info.heartbeat) << std::endl;
        }
    }
}

