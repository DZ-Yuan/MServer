#ifndef PCH_H
#define PCH_H

// ------- sys -------
#include <iostream>
#include <string.h>
#include <thread>
// #include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
//socket
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/epoll.h>

// ------- usr --------
#include "m_socket.h"
#include "server.h"
#include "message_system.h"
#include "msg_def.h"
#include "common.h"
#include "DataPacket.h"
#include "network_system.h"

using namespace std;

#endif