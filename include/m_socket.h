#ifndef M_SOCKET
#define M_SOCKET

#include <iostream>
#include <string.h>
#include <queue>

#include "common.h"

class MSocket
{
public:
    MSocket(std::string addr, int port = 80) : addr_(addr), port_(port)
    {
        sock_ = 0;
        net_id_ = 1;
    }
    ~MSocket()
    {
        Close();
    }

public:
    int Create();

    int GetSock()
    {
        return sock_;
    }

    void Close();

public:
    int sock_;
    int net_id_;
    int port_;
    std::string addr_;
};

#endif