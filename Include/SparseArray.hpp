//
// Created by arthur on 22/09/2022.
//

#ifndef MINEPLUSPLUS_SPARSEARRAY_HPP
#define MINEPLUSPLUS_SPARSEARRAY_HPP

#include <vector>
#include <any>
#include <optional>
#include <stdexcept>
#include <memory>

/**
 * @brief Sparse array
 * A container which offers fixed time access to individual elements in any order, its indices can contain gaps.
 * @tparam value_type Type of the elements
 * @tparam allocator Allocator used to allocate the elements
 */
template<typename value_type, typename allocator = std::allocator<std::optional<value_type>>>
class SparseArray
{
public:
	using container_type = std::vector<std::optional<value_type>, allocator>;
	using reference_type = value_type&;
	using const_reference_type = const value_type&;
	using size_type = typename container_type::size_type;
	using iterator = typename container_type::iterator;
	using const_iterator = typename container_type::const_iterator;

	SparseArray() = default;

	explicit SparseArray(size_type size) : _container(size)
	{
	}

	SparseArray(const SparseArray&) = delete;
	SparseArray(SparseArray&&) noexcept = default;
	~SparseArray() = default;
	SparseArray& operator=(const SparseArray&) = delete;
	SparseArray& operator=(SparseArray&&) noexcept = default;

	/**
	 * @brief Get the size of the SparseArray
	 * @return The size of the SparseArray
	 */
	size_type size() const
	{
		return _container.size();
	}

	/**
	 *  Returns true if the %SparseArray is empty.  (Thus begin() would equal end().)
	 * @return
	 */
	[[nodiscard]] bool empty() const
	{
		return _container.empty();
	}

	/**
	 * @return Returns a read/write iterator that points to the first element in the %SparseArray.
	 * Iteration is done in ordinary element order.
	 */
	iterator begin()
	{
		return _container.begin();
	}

	/**
	 * @return Returns a read/write iterator that points one past the last element in the %SparseArray.
	 * Iteration is done in ordinary element order.
	 */
	iterator end()
	{
		return _container.end();
	}

	/**
	 * @return Returns a read-only (constant) iterator that points to the first element in the %SparseArray.
	 * Iteration is done in ordinary element order.
	 */
	const_iterator begin() const
	{
		return _container.cbegin();
	}

	/**
	 * @return Returns a read-only (constant) iterator that points one past the last element in the %SparseArray.
	 * Iteration is done in ordinary element order.
	 */
	const_iterator end() const
	{
		return _container.cend();
	}

	/**
	 * @brief Subscript access to the data contained in the %SparseArray.
	 * @param index The index of the element for which data should be accessed.
	 * @return Returns a read/write reference to the element at specified location.
	 */
	reference_type operator[](size_type index)
	{
		if (index >= _container.size())
			throw std::out_of_range("Index out of range");
		if (!Has(index))
			throw std::runtime_error("Index Has no value");
		return _container[index].value();
	}

	/**
	 * @brief Subscript access to the data contained in the %SparseArray.
	 * @param index The index of the element for which data should be accessed.
	 * @return Read-only (constant) reference to data.
	 */
	const_reference_type operator[](size_type index) const
	{
		if (index >= _container.size())
			throw std::out_of_range("Index out of range");
		if (!Has(index))
			throw std::runtime_error("Index Has no value");
		return _container[index].value();
	}

	/**
	 * @brief Attempts to build and insert an element into the %SparseArray.
	 * @param index The index of the element to be inserted.
	 * @param args Arguments used to construct the element.
	 * @return
	 */
	template<typename... Args>
	reference_type Emplace(size_type index, Args&& ... args)
	{
		if (index >= _container.size())
			_container.resize(index + 1);
		_container[index] = std::make_optional<value_type>(std::forward<Args>(args)...);
		return _container.at(index).value();
	}

	/**
	 * @brief Removes the element at specified index from the %SparseArray.
	 * @param index The index of the element to be removed.
	 */
	void Erase(size_type index)
	{
		if (index >= _container.size() || !_container[index].has_value())
			throw std::out_of_range("Index out of range");
		_container[index].reset();
	}

	/**
	 * @param index The index of the element to be tested.
	 * @return Returns true if the element at specified location Has a value.
	 */
	bool Has(size_type index) const
	{
		return index < _container.size() && _container[index].has_value();
	}

	/**
	 * @brief Performs the specified lambda on each element of the SparseArray
	 * @tparam Lambda The lambda to perform
	 */
	template<typename Lambda>
	void ForEach(Lambda&& lambda)
	{
		for (auto& element : _container)
			if (element.has_value())
				lambda(element.value());
	}

private:
	container_type _container;
};
#endif //MINEPLUSPLUS_SPARSEARRAY_HPP
