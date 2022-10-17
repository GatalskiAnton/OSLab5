#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class BufferedChannel
{
public:
	explicit BufferedChannel(int buffSize) :buffSize_(buffSize), isClose(false) {};
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
		isClose.store(true);
		conditionalVariable.notify_all();
	}

private:
	std::mutex mutex;
	int buffSize_;
	std::queue<T> channel_;
	std::condition_variable conditionalVariable;
	std::atomic<bool> isClose;
};