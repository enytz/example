#include "Server.h"


Server_::Server_()
	:servBuffer(BUFF_SIZE)
{
	socket_init();

	in_addr ip_to_num;
	ErStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);					// string ip address translate in ip_to_num
	if (ErStat <= 0)
	{
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		return;
	}
	//ZeroMemory(&serv_Info, sizeof(serv_Info));

	serv_Info.sin_family = AF_INET;
	serv_Info.sin_port = htons(PORT);
	serv_Info.sin_addr = ip_to_num;

	ServSock_bind();

	ServSock_listen();

	//ZeroMemory(&client_Info, sizeof(client_Info));

	ServSock_accept();
	packet_size = 0;
}

void Server_::socket_init()
{
	ServSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServSocket <0)
	{
		 perror("Error initialization: ");
        std::cout<<'\n';
        close(ServSocket);
	}
	else
		std::cout << "Server socket initialization is OK" << std::endl;
}

void Server_::ServSock_bind()
{
	ErStat = bind(ServSocket, (sockaddr*)&serv_Info, sizeof(serv_Info));		// bind socket to ip/port
	if (ErStat != 0)
	{
        perror("Error socket binding to server info. Error: ");
		close(ServSocket);
		return;
	}

	else
		std::cout << "Binding socket to Server info is OK" << std::endl;
}

void Server_::ServSock_listen()
{
	ErStat = listen(ServSocket, 1);
	if (ErStat != 0)
	{
        perror("Can't start to listen. Error: ");
		close(ServSocket);
		return;
	}
	else
		std::cout << "Listening..." << std::endl;
}

void Server_::ServSock_accept()
{
	Client_Conn = accept(ServSocket, (sockaddr*)&client_Info, &client_Info_size);
	if (Client_Conn < 0)
	{
        perror("Client detected, but can't connect to a client.. Error: ");
		close(ServSocket);
		close(Client_Conn);
		return;
	}
	else
	{
		std::cout << "Connection to a client established succesfully" << std::endl;
		char clientIP[22];

		inet_ntop(AF_INET, &client_Info.sin_addr, clientIP, INET_ADDRSTRLEN);
		std::cout << "Client connected with IP address " << clientIP << std::endl;
	}
}

void Server_::receive_data()
{
		packet_size = recv(Client_Conn, servBuffer.data(), servBuffer.size(), 0);
		if (packet_size <= 0)
			packet_size = -1;
		//if (packet_size > 2 && check_on_remains())
			std::cout << "Client message: " << servBuffer.data() << std::endl;
	//	else
	//		std::cout << "The message does not meet the conditions" << std::endl;
		if (packet_size > 0)
			std::fill(servBuffer.begin(), servBuffer.end(), 0);
}

bool Server_::check_on_remains()
{
	int val = 0;
	for (int i = 0;i < packet_size;++i)
		val += (servBuffer[i]-'0') * pow(10, packet_size - i-1);
	if (!(val % 32))
		return true;
	else
		return false;

}