//
//  serverMain.cpp
//  Gossip
//
//  Created by PRANAV D BAGUR on 7/30/20.
//  Copyright © 2020 Pranav Bagur. All rights reserved.
//

#include "serverMain.hpp"
#include "serverJob.hpp"
#include "logger.hpp"

serverMain::serverMain(unsigned int backlog, threadPool* pool) :
backlog(backlog), pool(pool), keep_processing(true) {
}

serverMain::~serverMain() {
}

void
serverMain::teardown() {
    keep_processing = false;
    close(socket_fd);
}

bool
serverMain::init() {
    if (!createSocket())
        return false;
    //else if (!setSocketOpt())
    //    return false;
    else if (!bindSocket())
        return false;
    else if (!getSocketName())
        return false;
    else if (!listen())
        return false;
    else
        return true;
}

bool
serverMain::createSocket() {
    /*
     * create  a socket
     * sockfd(ret): socket descriptor
     * domain: communication domain (AF_INET IPv4 protocol)
     * type: communication type (SOCK_STREAM TCP)
     * protocol: IP Protocol value 0. This is the same number which appears
     *           on protocol field in the IP header of a packet
     */
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        log_error("failed to create a socket");
        return false;
    }
    
    return true;
}

bool
serverMain::setSocketOpt() {
    int opt = 1;
    if (!!setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        log_error("setSocketOpt failed");
        return false;
    }
    return true;
}

bool
serverMain::bindSocket() {
    /*
     * After creation of the socket, bind function binds the socket to the
     * address and port number specified in sockaddr_in.
     * INADDR_ANY = LOCALHOST
     */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(0);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    
    if (::bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        log_error("listen failure");
        return false;
    }
    
    return true;
}

bool
serverMain::getSocketName() {
    struct sockaddr_in sock_addr;
    unsigned int sock_addr_len;
    
    /* We must put the length in a variable */
    sock_addr_len = sizeof(sock_addr);
    
    /*
     * Ask getsockname to fill in this socket's local
     * address.
     */
    if (::getsockname(socket_fd, (struct sockaddr *)&sock_addr, &sock_addr_len) == -1) {
       log_error("getsockname failed");
       return false;
    }

    log_error("Local IP address is: " + std::string(inet_ntoa(sock_addr.sin_addr)));
    log_error("Local port is: " + std::to_string(ntohs(sock_addr.sin_port)));
    std::cout << "Local IP address is: " + std::string(inet_ntoa(sock_addr.sin_addr)) << std::endl;
    std::cout << "Local port is: " + std::to_string(ntohs(sock_addr.sin_port)) << std::endl;
    ip_addr = inet_ntoa(sock_addr.sin_addr);
    port = ntohs(sock_addr.sin_port);
    return true;
}

bool
serverMain::listen() {
    /*
     * listen puts the server socket in a passive mode, where it waits
     * for the client to approach the server to make a connection. The
     * backlog, defines the maximum length to which the queue of pending
     * connections for socket_fd may grow.
     */
    if (::listen(socket_fd, backlog) < 0) {
        log_error("listen failure");
        return false;
    }
    
    return true;
}

bool
serverMain::process() {
    while (keep_processing) {
        /*
         * accept extracts the first connection request on the queue of
         * pending connections for the listening socket, sockfd, creates
         * a new connected socket, and returns a new file descriptor
         * referring to that socket. At this point, connection is established
         * between client and server, and they are ready to transfer data.
         */
        sockaddr_in client;
        socklen_t client_size = sizeof(client);
        int client_socket_fd = accept(socket_fd, (struct sockaddr *)&client, &client_size);
        if (client_socket_fd == -1) {
            if (keep_processing == false) {
                log_trace("terminating..");
                return true;
            }
            else {
                log_error("accept failure");
                return false;
            }
        }
        else {
            threadPoolArg *arg = new threadPoolArg(new serverJob(client, client_size, client_socket_fd, pool));
            pool->add_job(arg); // Will be deleted by the thread after processing
        }
    }
    
    close(socket_fd);
    return true;
}
