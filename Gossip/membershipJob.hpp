//
//  membershipJob.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 9/26/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef membershipJob_hpp
#define membershipJob_hpp

#include <stdio.h>
#include "serverState.hpp"
#include "messageGossip.hpp"
#include "job.hpp"

class membershipJob : public job {
public:
    membershipJob();
    ~membershipJob();
    bool processJob();
};

#endif /* membershipJob_hpp */
