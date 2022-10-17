#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

//template<typename T>
//class BufferedChannel
//{
//public:
//	explicit BufferedChannel(int buffSize) :buffSize_(buffSize) {};
//	void send(T&& data)
//	{
//		std::unique_lock<std::mutex> locker(mutex);
//
//		conditionalVariable.wait(locker, [] {
//			return	channel_.size() != buffSize_;
//			});
//		
//		channel_.push_back(std::move(data));
//		++buffSize_;
//	}
//
//private:
//	std::mutex mutex;
//	int buffSize_;
//	std::queue<T> channel_;
//	std::condition_variable conditionalVariable;
//};
//
