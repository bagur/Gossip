//
//  synAckJob.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 9/12/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef synAckJob_hpp
#define synAckJob_hpp

#include <stdio.h>
#include "job.hpp"
#include "serverState.hpp"
#include "messageGossip.hpp"

using json = nlohmann::json;

class synAckJob : public job {
    messageAck *ack;
public:
    synAckJob(messageAck *msg_ack);
    ~synAckJob();
    bool processJob();
};
#endif /* synAckJob_hpp */
