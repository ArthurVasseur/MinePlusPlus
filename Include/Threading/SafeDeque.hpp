//
// Created by arthur on 22/09/2022.
//

#ifndef MINEPLUSPLUS_SAFEDEQUE_HPP
#define MINEPLUSPLUS_SAFEDEQUE_HPP

#include <memory>
#include <deque>
#include <mutex>
#include <utility>

namespace MinePP::Threading
{
	template<typename T, typename Allocator = std::allocator<T>>
	class SafeDeque
	{
	public:
		SafeDeque() = default;
		~SafeDeque() = default;

		std::pair<T, bool> PopFront()
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_deque.empty())
				return {{}, false };
			auto element = _deque.front();
			_deque.pop_front();
			return { std::move(element), true };
		}

		void PushBack(T element)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_deque.push_back(std::move(element));
		}

	private:
		std::deque<T, Allocator> _deque;
		std::mutex _mutex;
	};
}

#endif //MINEPLUSPLUS_SAFEDEQUE_HPP
