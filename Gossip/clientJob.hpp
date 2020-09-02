//
//  clientJob.hpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/25/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#ifndef clientJob_hpp
#define clientJob_hpp

#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "job.hpp"
#include "message.hpp"
#include "logger.hpp"
#include "nlohmann_json.hpp"

using json = nlohmann::json;

#define HDR_SIZE       16
#define HDR_MAGIC      "XB3DS"
#define MAX_DATA       1024

class clientJob : public job {
public:
    clientJob() {}
    ~clientJob() {}
    bool sendData(std::string ip_addr, int port, json& msg);
protected:
    int  createSocket();
    bool getSockAddr(struct sockaddr_in *server_addr, char *ip_addr, const int port);
    bool sendMsg(int socket_fd, json& msg);
    bool connect(int socket_fd, sockaddr_in server_addr);
    void disconnect(int socket_fd);
    std::string addHeader(std::string s);
};
#endif /* clientJob_hpp */
