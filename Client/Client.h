#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <algorithm>
#include <vector>

#include <thread>
#include <mutex>
#include <condition_variable>

#define INVALID_THREAD_ID	-1
#define THREAD_READ			1	
#define THREAD_EXPORT		2

#define PORT 1122

struct Connect_to_server
{
    Connect_to_server();
    ~Connect_to_server(){ close(ClientSocket);}
    Connect_to_server(const Connect_to_server&) = delete;
    Connect_to_server(Connect_to_server&&)=delete;
    Connect_to_server& operator=(const Connect_to_server&) = delete;
    Connect_to_server& operator=(Connect_to_server&&) = delete;

    void socket_init();
    void connect_to_server();
    void send_data(std::vector<char>& buffer){ short packet_size = send(ClientSocket, buffer.data(), buffer.size(), 0); }
    int erStat_val() { return ErStat; }

    private:
        int ErStat;
        int ClientSocket;
        sockaddr_in ServerInfo;
        socklen_t ServerInfo_size = sizeof(ServerInfo);
        in_addr ip_to_num;


};

class Client_
{
public:
	void Thread_read_string();
	void Thread_export_string();

	bool Grand_access(int Thread_id);
	void Delegate_access(int Thread_id);


private:
	std::vector<char> buffer;
	std::mutex mtx;
	std::condition_variable cv;
	int active_thread = 0;
	Connect_to_server sock_on;

};

void itoa_reverse(int value, std::vector <char>& buffer);