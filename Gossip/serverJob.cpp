//
//  serverJob.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 7/30/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "serverJob.hpp"
#include "logger.hpp"
#include "nlohmann_json.hpp"
#include "message.hpp"
#include "messageGossip.hpp"

using json = nlohmann::json;


serverJob::serverJob(sockaddr_in client, socklen_t client_size, int client_socket_fd, threadPool *pool) :
client(client),
client_size(client_size),
client_socket_fd(client_socket_fd),
pool(pool){
}

serverJob::~serverJob() {
}

bool
validateHeader(std::string s) {
    if (s.size() != HDR_SIZE)
        return false;
    else if (s.at(s.size() - 1) != '#')
        return false;
    else if (s.substr (0, sizeof(HDR_MAGIC) - 1) != HDR_MAGIC)
        return false;
    
    return true;
}

bool
serverJob::processJob() {
    char hdr_buf[HDR_SIZE + 1];
    int data_size = 0;
    int bytes_read = 0;
    std::string data = "";
    bool rval = true;
    
    printClientDetails();
    
    try {
        /* read the header */
        memset(hdr_buf, 0, HDR_SIZE + 1);
        recv(client_socket_fd, hdr_buf, HDR_SIZE, 0);
        std::string hdr(hdr_buf);
        
        /* validate the header */
        log_trace(hdr);
        if (!validateHeader(hdr)) {
            log_error("invalid header");
            throw "Invalid header";
        }
        else {
            /* extract data size */
            std::string delimiter = "#";
            size_t pos = 0;
            std::string token;
            while ((pos = hdr.find(delimiter)) != std::string::npos) {
                token = hdr.substr(0, pos);
                hdr.erase(0, pos + delimiter.length());
            }
            data_size = std::stoi(token);
        }
        
        while (bytes_read < data_size) {
            // clear the buffer
            memset(data_buf, 0, CHUNK_DATA + 1);
            
            // receive data
            ssize_t bytes_received = recv(client_socket_fd, data_buf, CHUNK_DATA, 0);
            if (bytes_received <= 0) {
                log_error("recv failure");
                close(client_socket_fd);
                log_warn("client disconnected");
                return false;
            }
            else if (bytes_received > (data_size - bytes_read + 1)) {
                log_error("invalid data");
                close(client_socket_fd);
                log_warn("client disconnected");
                return false;
            }
            else {
                data += std::string(data_buf);
                bytes_read += bytes_received;
            }
        }
        
        log_trace(data);
        log_verbose(data);
        json j  = json::parse(data);
        log_verbose(j.dump());
        
        /* opcode should exist if it is a valid message */
        switch ((int)j.at("opcode")) {
            case MSG_SYN:
            {
                log_trace("RECEIVED SYN MESSAGE");
                messageSyn *msg_syn = new messageSyn(j);
                pool->add_job(new threadPoolArg(new ackJob(msg_syn)));
                break;
            }
            case MSG_ACK:
            {
                log_trace("RECEIVED ACK MESSAGE");
                messageAck *msg_ack = new messageAck(j);
                pool->add_job(new threadPoolArg(new synAckJob(msg_ack)));
                break;
            }
            default:
                break;
        }
    }
    catch (...) {
        log_error("Invalid message recieved from " + std::string(hostname) + " . Disconnecting..");
        rval = false;
    }
    
    close(client_socket_fd);
    log_warn("client disconnected");
    return rval;
}

void
serverJob::printClientDetails() {
    /* print client information */
    memset(hostname, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
    
    if (!!getnameinfo((struct sockaddr *)&client, client_size, hostname,
                      NI_MAXHOST, service, NI_MAXSERV, 0)) {
        log_trace(std::string(hostname) + " connected on " + std::string(service));
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, hostname, NI_MAXHOST);
        log_trace(std::string(hostname) + " connected on " + std::to_string(ntohs(client.sin_port)));
    }
}
