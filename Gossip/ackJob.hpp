//
//  AckJob.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 9/2/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef AckJob_hpp
#define AckJob_hpp

#include <stdio.h>
#include "clientJob.hpp"
#include "serverState.hpp"
#include "messageGossip.hpp"

using json = nlohmann::json;

class ackJob : public clientJob {
    messageSyn syn;
public:
    ackJob(json data);
    ~ackJob();
    bool processJob();
};
#endif /* AckJob_hpp */
