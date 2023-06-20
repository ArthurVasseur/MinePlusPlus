//
// Created by arthur on 25/09/2022.
//

#ifndef MINEPLUSPLUS_CIRCULARBUFFER_HPP
#define MINEPLUSPLUS_CIRCULARBUFFER_HPP

#include <memory>
#include <array>
#include <concepts>
#include <stdexcept>
#include <span>
#include <type_traits>

namespace MinePP
{
	/**
	 * @brief A circular buffer
	 * @tparam T The type of the buffer
	 * @tparam BufferSize The size of the buffer
	 */
	template<typename T, std::size_t BufferSize>
	requires (BufferSize > 2)
	class CircularBuffer
	{
	private:
		template<bool constant>
		struct Iterator;
	public:
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using iterator = Iterator<false>;
		using const_iterator = const Iterator<true>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		CircularBuffer() : _begin(0), _end(0), _size(0)
		{
		}

		~CircularBuffer() = default;

		CircularBuffer(const CircularBuffer& other) = default;

		CircularBuffer(CircularBuffer&& other) noexcept: _begin(other._begin), _end(other._end), _size(other._size)
		{
			std::move(other._buffer.begin(), other._buffer.end(), _buffer.begin());
		}

		CircularBuffer& operator=(const CircularBuffer& other) = default;

		void push_back(const_reference value)
		{
			if (_size == BufferSize)
				throw std::runtime_error("Buffer is full");
			_buffer[_end] = value;
			_end = (_end + 1) % BufferSize;
			_size++;
		}

		void push_back(value_type&& value)
		{
			if (_size == BufferSize)
				throw std::runtime_error("Buffer is full");
			_buffer[_end] = std::move(value);
			_end = (_end + 1) % BufferSize;
			_size++;
		}

		void push_back(std::span<value_type> values)
		{
			if (_size + values.size() > BufferSize)
				throw std::runtime_error("Buffer is full");
			for (size_type i = 0; i < values.size(); i++)
			{
				push_back(values[i]);
			}
		}

		void pop_front()
		{
			if (empty())
				throw std::runtime_error("Buffer is empty");
			_begin = (_begin + 1) % BufferSize;
			_size--;
		}

		[[nodiscard]] reference front()
		{
			if (empty())
				throw std::runtime_error("Buffer is empty");
			return _buffer[_begin];
		}

		[[nodiscard]] const_reference front() const
		{
			if (empty())
				throw std::runtime_error("Buffer is empty");
			return _buffer[_begin];
		}

		[[nodiscard]] reference back()
		{
			if (empty())
				throw std::runtime_error("Buffer is empty");
			return _buffer[(_end - 1) % BufferSize];
		}

		[[nodiscard]] const_reference back() const
		{
			if (empty())
				throw std::runtime_error("Buffer is empty");
			return _buffer[(_end - 1) % BufferSize];
		}

		[[nodiscard]] iterator begin()
		{
			iterator iterator;
			iterator._index = _begin;
			iterator._buffer = this;
			return iterator;
		}

		[[nodiscard]] const_iterator begin() const
		{
			const_iterator iterator(this, _begin);
			return iterator;
		}


		[[nodiscard]] const_iterator cbegin() const
		{
			const_iterator iterator(this, _begin);
			return iterator;
		}

		[[nodiscard]] iterator end()
		{
			iterator iterator;
			iterator._index = _end;
			iterator._buffer = this;
			return iterator;
		}

		[[nodiscard]] const_iterator end() const
		{
			const_iterator iterator(this, _begin);
			return iterator;
		}

		[[nodiscard]] const_iterator cend() const
		{
			const_iterator iterator(this, _begin);
			return iterator;
		}

		[[nodiscard]] size_type size() const
		{
			return _size;
		}

		[[nodiscard]] size_type capacity() const
		{
			return BufferSize;
		}

		[[nodiscard]] bool empty() const
		{
			return _size == 0;
		}

		[[nodiscard]] bool full() const
		{
			return _size == BufferSize;
		}

		[[nodiscard]] pointer data()
		{
			return _buffer.data();
		}

		[[nodiscard]] const_pointer data() const
		{
			return _buffer.data();
		}

		[[nodiscard]] reference operator[](size_type index)
		{
			return _buffer[(_begin + index) % BufferSize];
		}

		[[nodiscard]] const_reference operator[](size_type index) const
		{
			return _buffer[(_begin + index) % BufferSize];
		}

	private:
		std::array<T, BufferSize> _buffer;
		std::size_t _begin;
		std::size_t _end;
		std::size_t _size;

		template<bool constant>
		struct Iterator
		{
		public:
			friend class CircularBuffer<T, BufferSize>;

			using iterator_category = std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using bufferPointer = typename std::conditional_t<constant, const CircularBuffer<T, BufferSize>*, CircularBuffer<T, BufferSize>*>;
			using reference = typename std::conditional_t<constant, const value_type&, value_type&>;
			using pointer = typename std::conditional_t<constant, const value_type*, value_type*>;

			Iterator() : _buffer(nullptr), _index(0)
			{
			}

			Iterator(bufferPointer buffer, std::size_t index) : _buffer(buffer), _index(index)
			{
			}

			Iterator(const Iterator& other) = default;

			reference operator*()
			{
				return (*_buffer)[_index];
			}

			pointer operator->() const
			{
				return &(operator*());
			}

			Iterator& operator++()
			{
				_index = (_index + 1) % _buffer->capacity();
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator tmp = (*this);
				++_index;
				return tmp;
			}

			Iterator& operator--()
			{
				_index = (_index - 1) % _buffer->capacity();
				return *this;
			}

			Iterator operator--(int)
			{
				Iterator tmp(*this);
				--_index;
				return tmp;
			}

			Iterator& operator+=(difference_type n)
			{
				_index = (_index + n) % _buffer->capacity();
				return *this;
			}

			Iterator& operator-=(difference_type n)
			{
				_index = (_index - n) % _buffer->capacity();
				return *this;
			}

			Iterator operator+(difference_type n) const
			{
				Iterator tmp(*this);
				return tmp += n;
			}

			Iterator operator-(difference_type n) const
			{
				Iterator tmp(*this);
				return tmp -= n;
			}

			difference_type operator-(const Iterator& other) const
			{
				return _index - other._index;
			}

			reference operator[](difference_type n) const
			{
				return (*_buffer)[(_index + n) % _buffer->capacity()];
			}

			bool operator==(const Iterator& other) const
			{
				return _buffer == other._buffer && _index == other._index;
			}

			bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			bool operator<(const Iterator& other) const
			{
				return _index < other._index;
			}

			bool operator>(const Iterator& other) const
			{
				return other < *this;
			}

			bool operator<=(const Iterator& other) const
			{
				return other >= *this;
			}

			bool operator>=(const Iterator& other) const
			{
				return *this >= other;
			}

			reference operator[](difference_type n)
			{
				return (*_buffer)[(_index + n) % _buffer->capacity()];
			}

			reference operator[](size_type n) const
			{
				iterator it = *this;
				it._index += n;
				return *it;
			}

		private:
			bufferPointer _buffer;
			std::size_t _index;
		};
	};

} // MinePP

#endif //MINEPLUSPLUS_CIRCULARBUFFER_HPP
