//
//  job.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/1/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef job_hpp
#define job_hpp

#include <stdio.h>

class job
{
public:
    job(){}
    virtual ~job(){}
    virtual bool processJob() = 0;
};

#endif /* job_hpp */
