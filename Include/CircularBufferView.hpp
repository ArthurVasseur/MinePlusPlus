////
//// Created by arthur on 25/09/2022.
////
//
//#ifndef MINEPLUSPLUS_CIRCULARBUFFERVIEW_HPP
//#define MINEPLUSPLUS_CIRCULARBUFFERVIEW_HPP
//
//#include "CircularBuffer.hpp"
//
//namespace MinePP
//{
//	template<typename T, std::size_t BufferSize, typename constBuffer, typename mutableBuffer>
//	requires (BufferSize > 2)
//	class CircularBufferView
//	{
//	public:
//		using buffer_type = CircularBuffer<T, BufferSize, constBuffer, mutableBuffer>;
//		using const_buffers_type = typename buffer_type::const_buffers_type;
//		using mutable_buffers_type = typename buffer_type::mutable_buffers_type;
//
//		explicit CircularBufferView(buffer_type& buffer) : _buffer(buffer)
//		{
//		}
//
//		~CircularBufferView() = default;
//
//		void commit(std::size_t size)
//		{
//			_buffer.commit(size);
//		}
//
//		void consume(std::size_t size)
//		{
//			_buffer.consume(size);
//		}
//
//		auto prepare(std::size_t n)
//		{
//			return _buffer.prepare(n);
//		}
//
//		auto data() const
//		{
//			return _buffer.data();
//		}
//
//		[[nodiscard]] std::size_t size() const
//		{
//			return _buffer.size();
//		}
//
//		[[nodiscard]] std::size_t max_size() const
//		{
//			return _buffer.max_size();
//		}
//
//		[[nodiscard]] constexpr std::size_t capacity() const
//		{
//			return _buffer.capacity();
//		}
//
//		[[nodiscard]] bool empty() const
//		{
//			return _buffer.empty();
//		}
//
//		T& operator[](std::size_t index)
//		{
//			return _buffer[index];
//		}
//
//		const T& operator[](std::size_t index) const
//		{
//			return _buffer[index];
//		}
//
//	private:
//		buffer_type& _buffer;
//	};
//
//	template<typename T, std::size_t BufferSize, typename constBuffer, typename mutableBuffer>
//	CircularBufferView<T, BufferSize, constBuffer, mutableBuffer>
//	make_view(CircularBuffer<T, BufferSize, constBuffer, mutableBuffer> &buffer)
//	{
//		return CircularBufferView<T, BufferSize, constBuffer, mutableBuffer>(buffer);
//	}
//} // MinePP
//#endif //MINEPLUSPLUS_CIRCULARBUFFERVIEW_HPP
