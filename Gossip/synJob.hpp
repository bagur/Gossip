//
//  synJob.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/25/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef synJob_hpp
#define synJob_hpp

#include <stdio.h>
#include "clientJob.hpp"
#include "serverState.hpp"
#include "messageGossip.hpp"

class synJob : public clientJob {
public:
    synJob();
    ~synJob();
    bool processJob();
    bool sendSyn(messageSyn& msg, int idx);
};
#endif /* synJob_hpp */
