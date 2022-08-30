#include "Client.h"

void Client_::Thread_read_string()
{
	std::string str;
	while (Grand_access(THREAD_READ))
	{
		std::cout << "Enter string (for exit enter \"exit\"): ";
		std::cin >> str;
		std::system("clear");

		if (str == "exit" || str == "Exit" || str == "EXIT") { exit(0); }

		if (str.size() > 64 || str.size() == 0)
		{
			std::cout << "String length incorrect";
			exit(-5);
		}


		for (int i = 0;i < str.size();++i)
		{
			if (!(isdigit(str[i])))
			{
				std::cout << " Incorrect string" << std::endl;
				exit(-6);
			}
		}

		std::sort(str.begin(), str.end(), [](const char& x, const char& y) {return x > y;});
		for (int i = 0;i < str.size();++i)
		{
			if (!((str[i] - '0') % 2))
			{
				buffer.push_back('K');
				buffer.push_back('B');
			}
			else
				buffer.push_back(str[i]);

		}
		Delegate_access(THREAD_EXPORT);
	}
}

void Client_::Thread_export_string()
{
	while (Grand_access(THREAD_EXPORT))
	{
		int sum = 0;
		std::vector<char> str = buffer;

		std::cout << std::endl;
		for (int i=0;i < str.size();i++)
		{
			std::cout << str[i];
		}
		std::cout << std::endl;

		buffer.clear();

		for (int i = 0;i < str.size();i++)
		{
			if (isdigit(str[i]))
				sum += str[i] - '0';
		}

		if (sock_on.erStat_val() != 0)
		{
			sock_on.socket_init();
			sock_on.connect_to_server();
		}
		itoa_reverse(sum, buffer);
		sock_on.send_data(buffer);
		buffer.clear();
		std::cout <<"Value: " << sum << std::endl;
		Delegate_access(THREAD_READ);
	}
}

bool Client_::Grand_access(int Thread_id)
{
	std::unique_lock<std::mutex> _lck(mtx);
	while (active_thread != INVALID_THREAD_ID)
	{
		if (active_thread == Thread_id)
			return true;
		cv.wait(_lck);
	}
	return false;
}

void Client_::Delegate_access(int Thread_id)
{
	std::unique_lock<std::mutex> _lck(mtx);
	active_thread = Thread_id;
	cv.notify_all();
}


void Connect_to_server::socket_init()
{
    ClientSocket  = socket(AF_INET,SOCK_STREAM,0);

    if (ClientSocket < 0)
        {
           perror("Error initialization: ");
           std::cout<<'\n';
           close(ClientSocket);
        }
    else
     {
        std::cout<<"Client socket initialization is OK\n";
     }
}

void Connect_to_server::connect_to_server()
{

    ErStat = connect(ClientSocket,(sockaddr*)&ServerInfo, ServerInfo_size);
    if (ErStat !=0)
    {
        perror("Error connection to server. Error ");
           std::cout<<'\n';
           close(ClientSocket);
    }
    else 
        std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server\n";
}

Connect_to_server::Connect_to_server()
{
    socket_init();
    inet_pton(AF_INET,"127.0.0.1",&ip_to_num);
    ServerInfo.sin_port     = htons(PORT);
    ServerInfo.sin_family   = AF_INET;
    ServerInfo.sin_addr     = ip_to_num;
    connect_to_server();
}

void itoa_reverse(int value, std::vector <char>& buffer)
{
	do
	{
		buffer.push_back((value % 10)+'0');
	} while ((value = value / 10));

	int i = 0;
	int j = buffer.size() - 1;
	char tmp;
	while (j > i)
	{
		tmp = buffer[i];
		buffer[i] = buffer[j];
		buffer[j] = tmp;
		j--;
		i++;
	}
}