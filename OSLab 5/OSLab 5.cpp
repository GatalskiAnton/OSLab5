#include <iostream>
#include "bufferedChannel.h"
#include <vector>
#include <thread>
#include <random>

void test1(BufferedChannel<std::string> & bch)
{
	bch.send("a");
	std::cout << "send\n";
}

void test2(BufferedChannel<std::string>& bch)
{
	bch.recv();
	std::cout << "recv\n";
}

int main()
{
	BufferedChannel<std::string> bch(15);

	std::vector<std::thread> threads;

	srand(time(NULL));
	for (int i = 0; i < 10000; i++)
	{
		if (rand() % 2 == 0)
		{
			threads.emplace_back(test1, std::ref(bch));
		}
		else
		{
			threads.emplace_back(test2, std::ref(bch));
		}
	}

	for (auto& thread: threads)
	{
		thread.join();
	}
	
	return 0;
}

