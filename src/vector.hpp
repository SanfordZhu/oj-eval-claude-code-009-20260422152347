#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{

template<typename T>
class vector
{
public:
	class const_iterator;

	class iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		T **ptr;
		T *cur;
	public:
		iterator(T **p = nullptr, T *c = nullptr) : ptr(p), cur(c) {}

		iterator operator+(const int &n) const
		{
			return iterator(ptr, cur + n);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr, cur - n);
		}
		int operator-(const iterator &rhs) const
		{
			if (ptr != rhs.ptr)
				throw invalid_iterator();
			return static_cast<int>(cur - rhs.cur);
		}
		iterator& operator+=(const int &n)
		{
			cur += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			cur -= n;
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++cur;
			return tmp;
		}
		iterator& operator++()
		{
			++cur;
			return *this;
		}
		iterator operator--(int)
		{
			iterator tmp = *this;
			--cur;
			return tmp;
		}
		iterator& operator--()
		{
			--cur;
			return *this;
		}
		T& operator*() const
		{
			return *cur;
		}
		bool operator==(const iterator &rhs) const
		{
			return cur == rhs.cur;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return cur == rhs.cur;
		}
		bool operator!=(const iterator &rhs) const
		{
			return cur != rhs.cur;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return cur != rhs.cur;
		}

		friend class const_iterator;
		friend class vector;
	};

	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		T **ptr;
		const T *cur;
	public:
		const_iterator(T **p = nullptr, const T *c = nullptr) : ptr(p), cur(c) {}
		const_iterator(const iterator &it) : ptr(it.ptr), cur(it.cur) {}

		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr, cur + n);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr, cur - n);
		}
		int operator-(const const_iterator &rhs) const
		{
			if (ptr != rhs.ptr)
				throw invalid_iterator();
			return static_cast<int>(cur - rhs.cur);
		}
		const_iterator& operator+=(const int &n)
		{
			cur += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			cur -= n;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			++cur;
			return tmp;
		}
		const_iterator& operator++()
		{
			++cur;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator tmp = *this;
			--cur;
			return tmp;
		}
		const_iterator& operator--()
		{
			--cur;
			return *this;
		}
		const T& operator*() const
		{
			return *cur;
		}
		bool operator==(const iterator &rhs) const
		{
			return cur == rhs.cur;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return cur == rhs.cur;
		}
		bool operator!=(const iterator &rhs) const
		{
			return cur != rhs.cur;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return cur != rhs.cur;
		}

		friend class iterator;
		friend class vector;
	};

private:
	T *begin_;
	T *end_;
	T *capacity_;

public:
	vector() : begin_(nullptr), end_(nullptr), capacity_(nullptr) {}

	vector(const vector &other)
	{
		size_t sz = other.size();
		if (sz == 0)
		{
			begin_ = nullptr;
			end_ = nullptr;
			capacity_ = nullptr;
			return;
		}
		begin_ = static_cast<T*>(::operator new(sz * sizeof(T)));
		for (size_t i = 0; i < sz; ++i)
			new(begin_ + i) T(*(other.begin_ + i));
		end_ = begin_ + sz;
		capacity_ = end_;
	}

	~vector()
	{
		for (size_t i = 0; i < size(); ++i)
			(begin_ + i)->~T();
		::operator delete(begin_);
	}

	vector &operator=(const vector &other)
	{
		if (this == &other)
			return *this;
		size_t sz = size();
		for (size_t i = 0; i < sz; ++i)
			(begin_ + i)->~T();
		::operator delete(begin_);

		size_t other_sz = other.size();
		if (other_sz == 0)
		{
			begin_ = nullptr;
			end_ = nullptr;
			capacity_ = nullptr;
			return *this;
		}
		begin_ = static_cast<T*>(::operator new(other_sz * sizeof(T)));
		for (size_t i = 0; i < other_sz; ++i)
			new(begin_ + i) T(*(other.begin_ + i));
		end_ = begin_ + other_sz;
		capacity_ = end_;
		return *this;
	}

	T & at(const size_t &pos)
	{
		if (pos >= size())
			throw index_out_of_bound();
		return *(begin_ + pos);
	}

	const T & at(const size_t &pos) const
	{
		if (pos >= size())
			throw index_out_of_bound();
		return *(begin_ + pos);
	}

	T & operator[](const size_t &pos)
	{
		if (pos >= size())
			throw index_out_of_bound();
		return *(begin_ + pos);
	}

	const T & operator[](const size_t &pos) const
	{
		if (pos >= size())
			throw index_out_of_bound();
		return *(begin_ + pos);
	}

	const T & front() const
	{
		if (size() == 0)
			throw container_is_empty();
		return *begin_;
	}

	const T & back() const
	{
		if (size() == 0)
			throw container_is_empty();
		return *(end_ - 1);
	}

	iterator begin()
	{
		return iterator(&begin_, begin_);
	}

	const_iterator begin() const
	{
		return const_iterator(const_cast<T**>(&begin_), begin_);
	}

	const_iterator cbegin() const
	{
		return const_iterator(const_cast<T**>(&begin_), begin_);
	}

	iterator end()
	{
		return iterator(&begin_, end_);
	}

	const_iterator end() const
	{
		return const_iterator(const_cast<T**>(&begin_), end_);
	}

	const_iterator cend() const
	{
		return const_iterator(const_cast<T**>(&begin_), end_);
	}

	bool empty() const
	{
		return size() == 0;
	}

	size_t size() const
	{
		return static_cast<size_t>(end_ - begin_);
	}

	void clear()
	{
		for (size_t i = 0; i < size(); ++i)
			(begin_ + i)->~T();
		end_ = begin_;
	}

	iterator insert(iterator pos, const T &value)
	{
		size_t idx = static_cast<size_t>(pos.cur - begin_);
		size_t sz = size();
		if (end_ == capacity_)
		{
			size_t new_cap = capacity_ == begin_ ? 1 : static_cast<size_t>(capacity_ - begin_) * 2;
			T *new_begin = static_cast<T*>(::operator new(new_cap * sizeof(T)));
			for (size_t i = 0; i < idx; ++i)
				new(new_begin + i) T(*(begin_ + i));
			new(new_begin + idx) T(value);
			for (size_t i = idx; i < sz; ++i)
				new(new_begin + i + 1) T(*(begin_ + i));
			for (size_t i = 0; i < sz; ++i)
				(begin_ + i)->~T();
			::operator delete(begin_);
			begin_ = new_begin;
			end_ = begin_ + sz + 1;
			capacity_ = begin_ + new_cap;
		}
		else
		{
			new(end_) T(*(end_ - 1));
			for (T *p = end_ - 1; p > pos.cur; --p)
				*p = *(p - 1);
			*pos.cur = value;
			++end_;
		}
		return iterator(&begin_, begin_ + idx);
	}

	iterator insert(const size_t &ind, const T &value)
	{
		if (ind > size())
			throw index_out_of_bound();
		return insert(begin() + static_cast<int>(ind), value);
	}

	iterator erase(iterator pos)
	{
		size_t idx = static_cast<size_t>(pos.cur - begin_);
		for (T *p = pos.cur; p < end_ - 1; ++p)
			*p = *(p + 1);
		(end_ - 1)->~T();
		--end_;
		return iterator(&begin_, begin_ + idx);
	}

	iterator erase(const size_t &ind)
	{
		if (ind >= size())
			throw index_out_of_bound();
		return erase(begin() + static_cast<int>(ind));
	}

	void push_back(const T &value)
	{
		if (end_ == capacity_)
		{
			size_t new_cap = capacity_ == begin_ ? 1 : static_cast<size_t>(capacity_ - begin_) * 2;
			size_t sz = size();
			T *new_begin = static_cast<T*>(::operator new(new_cap * sizeof(T)));
			for (size_t i = 0; i < sz; ++i)
				new(new_begin + i) T(*(begin_ + i));
			for (size_t i = 0; i < sz; ++i)
				(begin_ + i)->~T();
			::operator delete(begin_);
			begin_ = new_begin;
			end_ = begin_ + sz;
			capacity_ = begin_ + new_cap;
		}
		new(end_) T(value);
		++end_;
	}

	void pop_back()
	{
		if (size() == 0)
			throw container_is_empty();
		--end_;
		end_->~T();
	}
};

}

#endif
