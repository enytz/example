#include "Client.h"

int main()
{
    Client_ my;
	std::thread thread_1([&my]() {my.Thread_read_string();});
	std::thread thread_2([&my]() {my.Thread_export_string();});

	my.Delegate_access(THREAD_READ);
	//for (int i = 1;i <= 10;i++)
	//{
	//	std::cout << std::this_thread::get_id() << "  " << i << std::endl;
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//}
	thread_1.join();
	thread_2.join();
	return 0;
}