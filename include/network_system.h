#ifndef NETWORK_SYS
#define NETWORK_SYS

#include <string>
#include <queue>
#include <unordered_map>
#include <thread>
#include <functional>
#include <sys/epoll.h>
#include "m_socket.h"
#include "common.h"

#define BUFFER_SEIZ 1024
#define PACK_DEFAULT_SIZE 128

#define SERVER_MAX_EPOLL_EVENTS 20
#define SERVER_CONN_RESPONES "Server is online..."

class MServer;

struct SockData
{
    int sock_;
    int total_size_;
    int used_;
    int remain_;
    char *data_;

    SockData(int sock);
    virtual ~SockData();

    void Resize(int need);
    void Reset(){};
    void Append(const char *src, int size);
};

struct SockDataReader : SockData
{
    ushort data_len_;
    //已解析长度
    int parsed_;

    SockDataReader(int sock);
    ~SockDataReader();

    void ParseData(void *pack_data, int &size);
    int ParseDataLen();
    // unused
    void Adjust();
};

struct NetPacket
{
    ushort len_;
    char *data_;
    char *offset_;

    NetPacket(const void *data, ushort len)
        : data_((char *)data), len_(len)
    {
        offset_ = data_;
    }
    ~NetPacket()
    {
        if (data_)
        {
            delete data_;
            data_ = nullptr;
        }
    }
};

/*
    threads：
    1 监听；1 epoll响应；1 sock数据处理
*/

//网络系统
class NetworkSystem
{
public:
    NetworkSystem(MServer *server);
    ~NetworkSystem();

    enum StatusType
    {
        sInActive = 0,
        sRunning,
        sTerminated,
    };

public:
    void SetupConfig(std::string addr, int port);
    //创建服务器sock
    void InitSerSock();
    void Start();

    // thread
    std::function<void()> OnListenThread();
    std::function<void()> OnEpollEventThread();
    std::function<void()> OnProcSockDataThread();
    //监听
    void OnListen();
    //处理epoll事件
    void OnEpollEvent();
    //处理Sock数据
    void OnProcSockData();
    // todo 发送sock数据
    void OnSendSockData();

    // func
    void SetStatus(int f)
    {
        status_ = f;
    }
    int GetStatus()
    {
        return status_;
    }
    void Terminate()
    {
        SetStatus(sTerminated);
    }

    void AddNewClientSock(int sock);

    bool AddEpollEvent(int fd);
    bool DelEpollEvent(int fd);

    //接收sock数据
    void RecvSockData(int sock);
    int RecvOnce(int sock, char *buf, int size);
    //发送sock数据
    void SendData(const void *data, ushort len);
    //推送数据
    void Post();

    void CloseOneSock(int sock);
    //清理
    void ClearSockDataQueue();
    void ClearSockContainer();

    // util
    void OutputPrint(std::string m)
    {
        std::cout << "[ NetworkSystem ] : " << m << std::endl;
    }

    void DisconnectSock(int s)
    {
        DelEpollEvent(s);
        shutdown(s, SHUT_RDWR);
        close(s);
    }

public:
    int status_;
    char buffer_[BUFFER_SEIZ];
    // server sock
    int ser_sock_;
    std::string address_;
    int port_;

    // epoll
    int epfd_;
    epoll_event ep_events[SERVER_MAX_EPOLL_EVENTS];

    // sock数据接收缓冲
    std::queue<SockData *> sock_data_queue_;
    // sock容器
    std::unordered_map<int, SockDataReader *> sock_map_;
    // sock数据发送队列
    std::queue<void *> sock_send_queue_;

    // MServer
    MServer *server_;

    //监听线程
    std::thread listen_thread_;
    // epoll处理线程
    std::thread epoll_thread_;
    // sock数据处理线程
    std::thread data_thread_;
};

#endif
