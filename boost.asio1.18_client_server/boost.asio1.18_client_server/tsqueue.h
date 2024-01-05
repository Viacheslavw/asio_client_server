#pragma once

#include <Net_libs.h>

namespace olc 
{
	namespace net
	{
		template<class T>
		class tsqueue
		{
		public:
			tsqueue() = default;
			tsqueue(const tsqueue<t>&) = delete;
			virtual ~tsqueue() { clear(); }


			const T& front() {
				std::scoped_lock lock(maxQueue);
				return deqQueue.front();
			}

			const T& back() {
				std::scoped_lock lock(muxQueue);
				return deqQueue.back();
			}

			T pop_front() {

				std::scoped_lock lock(maxQueue);
				auto t = std::move(deqQueue.front());
				deqQueue.pop_front();
				return t;
			}

			T pop_back() {
				std::scoped_lock lock(maxQueue);
				auto t = std::move(deqQueue.back());
				deqQueue.pop_back();
				return t;
			}

			void push_back(const T& item) {
				std::scoped_lock lock(maxQueue);
				deqQueue.emplace_back(std::move(item));

				std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.notify_one();
			}

			void push_front(const T& item) {
				std::scoped_lock lock(muxQueue);
				deqQueue.emplace_front(std::move(item));

				std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.notify_one();
			}

			bool empty() {
				std::scoped_lock lock(naxQueue);
				return deqQueue.empty();
			}

			size_t count() {
				std::scoped_lock lock(maxQueue);
				return deqQueue.size();
			}

			void clear() {
				std::scoped_lock lock(maxQueue);
				deqQueue.clear();
			}

			void wait() {
				while (empty()) {
					std::unique_lock<std::mutex> ul(muxBlocking);
					cvBlocking.wait(ul);
				}
			}

			protected:
				std::mutex muxQueue;
				std::deque<T> deqQueue;
				std::condition_variable cvBlocking;
				std::mutex muxBlocking;
		};
	}

}