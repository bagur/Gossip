//
//  clientJob.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 8/25/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "clientJob.hpp"

int
clientJob::createSocket() {
    int socket_fd = 0;
    
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        log_error("Socket creation error");
        return -1;
    }
    
    return socket_fd;
}

bool
clientJob::getSockAddr(struct sockaddr_in *server_addr, char *ip_addr, const int port) {
    
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(::inet_pton(AF_INET, ip_addr, &server_addr->sin_addr) <= 0)
    {
        log_error("Invalid address/Address not supported");
        server_addr = nullptr;
        return false;
    }
    
    return true;
}

std::string
clientJob::addHeader(std::string s) {
    std::string magic = HDR_MAGIC;
    char hdr[HDR_SIZE + 1];
    int idx = HDR_SIZE;
    
    auto data_json = json::parse(s);
    std::string data_str = data_json.dump();
    int data_size = (int)data_str.length();
    memset(hdr, '$', HDR_SIZE);
    
    hdr[idx--] = '\0';
    hdr[idx--] = '#';
    while (data_size > 0) {
        hdr[idx--] = '0' + (data_size % 10);
        data_size = data_size / 10;
    }
    hdr[idx--] = '#';
    
    for (int i = 0; i < magic.size(); i++) {
        hdr[i] = magic.at(i);
    }
    
    std::string data(hdr);
    data += data_str;
    return data;
}

bool
clientJob::sendMsg(int socket_fd, json& msg) {
    std::string data = addHeader(msg.dump());
    int len = (int)data.size() + 1;
    char buffer[len];
    
    memset(buffer, 0, len);
    strcpy(buffer, data.c_str());
    ::send(socket_fd, buffer, len, 0);
    return true;
}

bool
clientJob::connect(int socket_fd, sockaddr_in server_addr) {
    if (::connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        log_error("Connection Failed");
        return false;
    }
    
    return true;
}

void
clientJob::disconnect(int socket_fd) {
    close(socket_fd);
}

bool
clientJob::sendData(std::string ip_addr, int port, json& msg) {
    int                 socket_fd;
    struct sockaddr_in  server_addr;
    char                ip_addr_arr[ip_addr.length() + 1];
      
    strcpy(ip_addr_arr, ip_addr.c_str());
    if ((socket_fd = createSocket()) < 0)
        return false;
    else if (!getSockAddr(&server_addr, ip_addr_arr, port))
        return false;
    else if (!connect(socket_fd, server_addr))
        return false;
    else if (!sendMsg(socket_fd, msg))
        return false;
    else
        disconnect(socket_fd);
    
    return true;
}
