//
//  serverJob.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 7/30/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef serverJob_hpp
#define serverJob_hpp

#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include "job.hpp"
#include "ackJob.hpp"
#include "threadPool.hpp"

#define CHUNK_DATA     512
#define HDR_SIZE       16
#define HDR_MAGIC      "XB3DS"

class serverJob : public job {
public:
    serverJob(sockaddr_in client, socklen_t client_size, int client_socket_fd, threadPool *pool);
    ~serverJob();
    bool processJob();
    
    pthread_t tid;
    
private:
    sockaddr_in client;
    socklen_t client_size;
    int client_socket_fd;
    threadPool *pool;
    char hostname[NI_MAXHOST];
    char service[NI_MAXSERV];
    char data_buf[CHUNK_DATA + 1];
    
    void printClientDetails();
};

#endif /* serverJob_hpp */
