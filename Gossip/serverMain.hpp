//
//  serverMain.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 7/30/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef serverMain_hpp
#define serverMain_hpp

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <pthread.h>
#include "threadPool.hpp"

class serverMain {
public:
    serverMain(unsigned int backlog, threadPool *pool);
    ~serverMain();
    bool init();
    bool process();
    void teardown();
    
    std::string getIpAddr() {
        return ip_addr;
    }
    
    int getPort() {
        return port;
    }
    
private:
    std::string ip_addr;
    unsigned int port;
    unsigned int backlog;
    int socket_fd;
    struct sockaddr_in address;
    threadPool *pool;
    bool keep_processing;
    
    bool createSocket();
    bool setSocketOpt();
    bool bindSocket();
    bool getSocketName();
    bool listen();
};

#endif /* serverMain_hpp */
