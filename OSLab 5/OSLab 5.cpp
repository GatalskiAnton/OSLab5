#include <iostream>
#include "bufferedChannel.h"


void Worker1(BufferedChannel<std::string>& ch)
{
	ch.send("213");
	ch.send("231");
	ch.send("2313");
	ch.send("2313");
	ch.send("12321");
}

void Worker2(BufferedChannel<std::string>& ch)
{
	ch.recv();
	ch.recv();
	ch.recv();
	ch.recv();
}

int main()
{
	//BufferedChannel<std::string> channel(5);
	//channel.send("1");
	//channel.send("2");
	//channel.send("3");
	//channel.send("4");
	//channel.close();
	//std::pair<std::string, bool> p1 = channel.recv();
	//std::pair<std::string, bool> p2 = channel.recv();
	//std::pair<std::string, bool> p3 = channel.recv();
	//std::pair<std::string, bool> p4 = channel.recv();
	//std::pair<std::string, bool> p5 = channel.recv();

	//std::cout << p1.first << " " << p1.second << '\n';
	//std::cout << p2.first << " " << p2.second << '\n';
	//std::cout << p3.first << " " << p3.second << '\n';
	//std::cout << p4.first << " " << p4.second << '\n';
	//std::cout << p5.first << " " << p5.second << '\n';

	BufferedChannel<std::string> ch(5);
	std::vector<std::thread> threads;
	for (int i = 0; i < 5; ++i)
	{
		if (i % 2 != 0)
		{
			threads.emplace_back(Worker1, std::ref(ch));
		}
		else
		{
			threads.emplace_back(Worker2, std::ref(ch));
		}
	}
	for (auto& thread : threads)
	{
		thread.join();
	}
	return 0;
}

