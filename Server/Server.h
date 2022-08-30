# pragma once
#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#include <chrono>
#include <thread>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>


#define PORT 1122

class Server_
{
public:
	Server_();
	~Server_()
	{
		close(ServSocket);
		close(Client_Conn);
	}
	

	void socket_init();
	void ServSock_bind();
	void ServSock_listen();
	void ServSock_accept();
	void receive_data();
	int erStat_val() const{ return ErStat; }
	int Packet_size() const { return packet_size; }
private:
	const short BUFF_SIZE = 64;

	int ServSocket;
	int Client_Conn;

	in_addr ip_to_num;

	sockaddr_in serv_Info;
	sockaddr_in client_Info;

	socklen_t client_Info_size = sizeof(client_Info);

	std::vector<char> servBuffer;
	int ErStat;
	short packet_size = 0;

	bool check_on_remains();

};