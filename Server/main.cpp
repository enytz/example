#include "Server.h"

int main()
{
	std::string str;
	Server_ my;
	while (true)
	{
		while (my.Packet_size() >= 0)
		{
			my.receive_data();
		}
		std::cout << "The client is disconnected, to reconnect enter \"yes\" and wait connect client, to exit enter any key: ";
		std::cin >> str;
		if (str == "yes")
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			my.ServSock_accept();
			my.receive_data();
		}
			
		else break;

	}
	return 0;

}