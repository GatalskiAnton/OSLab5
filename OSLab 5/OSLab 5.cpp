#include <iostream>
#include "bufferedChannel.h"

template<typename T>
class BufferedChannel
{
public:
	explicit BufferedChannel(int buffSize) :buffSize_(buffSize),isClose(false) {};
	void send(T&& data)
	{
		if (isClose)
		{
			throw new std::runtime_error("Channel is closed");
		}

		std::unique_lock<std::mutex> locker(mutex);

		conditionalVariable.wait(locker, [this] {
			return	channel_.size() < buffSize_;
			});

		channel_.push(std::move(data));
		conditionalVariable.notify_one();
	}

	std::pair<T, bool> recv()
	{
		
		if (isClose && channel_.empty())
		{
			return std::make_pair(T(), false);
		}

		std::unique_lock<std::mutex> locker(mutex);

		conditionalVariable.wait(locker, [this] {
			return	!channel_.empty();
			});
		T result = channel_.front();
		channel_.pop();
		conditionalVariable.notify_one();
		return std::make_pair(result, true);
	}

	void close()
	{
		isClose = true;
	}

private:
	std::mutex mutex;
	int buffSize_;
	std::queue<T> channel_;
	std::condition_variable conditionalVariable;
	bool isClose;
};



int main()
{
	BufferedChannel<std::string> channel(5);
	channel.send("1");
	channel.send("2");
	channel.send("3");
	channel.send("4");
	channel.close();
	std::pair<std::string, bool> p1 = channel.recv();
	std::pair<std::string, bool> p2 = channel.recv();
	std::pair<std::string, bool> p3 = channel.recv();
	std::pair<std::string, bool> p4 = channel.recv();
	std::pair<std::string, bool> p5 = channel.recv();

	std::cout << p1.first << " " << p1.second << '\n';
	std::cout << p2.first << " " << p2.second << '\n';
	std::cout << p3.first << " " << p3.second << '\n';
	std::cout << p4.first << " " << p4.second << '\n';
	std::cout << p5.first << " " << p5.second << '\n';
}

