//
//  clusterInfo.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 9/20/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "clusterInfo.hpp"

clusterInfo::clusterInfo(std::string ip_addr, int port, std::string cluster_id, int version) :
my_info(ip_addr, port, cluster_id, version, 1, STATE_BOOTSTRAP), cluster_id(cluster_id),
nodes_list_mtx(), nodes_list(), ticks_map(), nodes_down_map() {
    nodes_list.push_back(my_info);
}

clusterInfo::~clusterInfo() {
}

std::vector<gossipInfo>
clusterInfo::getNodesList() {
    return nodes_list;
}

void
clusterInfo::updateTicksMap(std::string key) {
    if (ticks_map.count(key) > 0) {
        ticks_map[key] = clock();
        if (nodes_down_map.count(key) > 0) {
            log_error(key + " is alive again!");
            nodes_down_map.erase(key);
        }
    }
    else {
        ticks_map.insert(std::make_pair(key, clock()));
        if (nodes_down_map.count(key) > 0) {
            log_error("Assertion hit: " + key + " in nodes_down_map and not in ticks_map");
            nodes_down_map.erase(key);
        }
    }
}

void
clusterInfo::processTicksMap() {
    for (auto it : ticks_map) {
        if (nodes_down_map.count(it.first) > 0)
            continue;
        else if ((clock() - it.second) > NODE_TIMEOUT) {
            nodes_down_map.insert(std::make_pair(it.first, it.second));
        }
    }
}

void
clusterInfo::addToNodesDownMap(std::string key) {
    if (nodes_down_map.count(key) == 0)
        nodes_down_map.insert(std::make_pair(key, clock()));
}

void clusterInfo::processNodesList(std::vector<gossipInfo> peer_list, std::vector<gossipInfo>& peer_list_new) {
    {
        int i = 0;
        int j = 0;
        std::vector<gossipInfo> new_list;
        
        std::sort(peer_list.begin(), peer_list.end());
        
        /*
         * When the unique_lock is constructed it will lock the mutex,
         * and when it gets destructed, it will unlock the mutex.
         */
        std::unique_lock<std::mutex> state_lock(nodes_list_mtx);
        
        /* lock held by the thread */
        std::sort(nodes_list.begin(), nodes_list.end());
        
        while (i < nodes_list.size() && j < peer_list.size()) {
            gossipInfo& cur_info  = nodes_list[i];
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
                updateTicksMap(peer_info.getKey());
                j++;
            }
            else {
                // We both have this entry. update info if necessary
                if (cur_info.version != peer_info.version) {
                    if (cur_info.version < peer_info.version) {
                        new_list.push_back(peer_info);
                        updateTicksMap(peer_info.getKey());
                    }
                    else {
                        new_list.push_back(cur_info);
                        peer_list_new.push_back(cur_info);
                    }
                }
                else if (cur_info.heartbeat != peer_info.heartbeat) {
                    if (cur_info.heartbeat < peer_info.heartbeat) {
                        new_list.push_back(peer_info);
                        updateTicksMap(peer_info.getKey());
                    }
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
        
        while (i < nodes_list.size()) {
            new_list.push_back(nodes_list[i]);
            peer_list_new.push_back(nodes_list[i]);
            i++;
        }
        
        while (j < peer_list.size()) {
            new_list.push_back(peer_list[j]);
            updateTicksMap(peer_list[j].getKey());
            j++;
        }
        
        this->nodes_list.clear();
        this->nodes_list = new_list;
    }
}


void
clusterInfo::logNodesInfo() {
    {
        std::string log = cluster_id + " : ";
        std::unique_lock<std::mutex> state_lock(nodes_list_mtx);
        for (gossipInfo& info : nodes_list) {
            log += "\n";
            log += info.getKey() + " " + std::to_string(info.heartbeat);
        }
        log_trace(log);
    }
}

void clusterInfo::initState() {
    std::sort(nodes_list.begin(), nodes_list.end());
    my_info.state = STATE_INITED;
}


gossipInfo&
clusterInfo::getInfo() {
    return this->my_info;
}

void
clusterInfo::updateState() {
    // TODO: move ipaddr, port, version and heartbeat to serverstate
    getInfo().heartbeat++;
}

void clusterInfo::addNode(gossipInfo node) {
    this->nodes_list.push_back(node);
}

void
clusterInfo::logMembershipInfo() {
    {
        clock_t cur_time = clock();
        
        std::string log = cluster_id + " : ";
        std::unique_lock<std::mutex> state_lock(nodes_list_mtx);
        for (gossipInfo& info : nodes_list) {
            if (info.getKey() == my_info.getKey())
                continue;
            
            if (ticks_map.count(info.getKey()) == 0) {
                log_error("Assertion: " + info.getKey() + " in nodes_list but not in ticks_map");
            }
            else {
                double last_update = ticks_map[info.getKey()];
                bool is_dead = (nodes_down_map.count(info.getKey()) > 0);
                log += "\n";
                log += info.getKey() + ": last update: " + std::to_string(((double)cur_time - last_update) / CLOCKS_PER_SEC);
                if (is_dead)
                    log += " <presumed dead>";
            }
        }
        log_trace(log);
        std::cout << log << std::endl;
    }
}
