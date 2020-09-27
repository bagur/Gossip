//
//  membership.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 9/26/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "membershipJob.hpp"


membershipJob::membershipJob() {
}

membershipJob::~membershipJob() {
}

bool
membershipJob::processJob() {
    std::unordered_map<std::string, clusterInfo *>& map = getServerState()->getClusterMap();
    
    for (auto it : map) {
        it.second->processTicksMap();
        it.second->logMembershipInfo();
    }
    
    return true;
}
