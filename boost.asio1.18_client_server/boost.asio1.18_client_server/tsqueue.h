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
			}
		};
	}

}