#include "pch.h"

int MSocket::Create()
{
    if (sock_ > 0)
    {
        return 0;
    }

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // set host address
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(addr_.c_str());
    server_addr.sin_port = htons(port_);

    bind(sock, (sockaddr *)&server_addr, sizeof(server_addr));
    sock_ = sock;

    return sock_;
}

void MSocket::Close()
{
    if (net_id_ < 0)
    {
        return;
    }

    net_id_ = -1;
    shutdown(sock_, SHUT_RDWR);
    close(sock_);
}
