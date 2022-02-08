#pragma once
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <initializer_list>

template <typename Type>
class SingleLinkedList
{
	// ���� ������
	struct Node
	{
		Node() = default;
		Node(const Type& val, Node* next)
			: value(val)
			, next_node(next)
		{
		}
		Type value{};
		Node* next_node = nullptr;
	};

	template <typename ValueType>
	class BasicIterator
	{
		// ����� ������ ����������� �������������, ����� �� ������� ������
		// ��� ������ � ��������� ������� ���������
		friend class SingleLinkedList;

		// �������������� ����������� ��������� �� ��������� �� ���� ������
		explicit BasicIterator(Node* node)
			: node_(node)
		{
		}

	public:
		// ����������� ���� ���� �������� ����������� ���������� � ��������� ����� ���������

		// ��������� ��������� - forward iterator
		// (��������, ������� ������������ �������� ���������� � ������������ �������������)
		using iterator_category = std::forward_iterator_tag;
		// ��� ���������, �� ������� ������������ ��������
		using value_type = Type;
		// ���, ������������ ��� �������� �������� ����� �����������
		using difference_type = std::ptrdiff_t;
		// ��� ��������� �� ����������� ��������
		using pointer = ValueType*;
		// ��� ������ �� ����������� ��������
		using reference = ValueType&;

		BasicIterator() = default;

		// �������������� �����������/����������� �����������
		// ��� ValueType, ����������� � Type, ������ ���� ����������� ������������
		// ��� ValueType, ����������� � const Type, ������ ���� ��������������� ������������
		BasicIterator(const BasicIterator<Type>& other) noexcept
			: node_(other.node_)
		{
		}

		// ����� ���������� �� ������� �������������� �� ���������� ��������� = ��� �������
		// ����������������� ������������ �����������, ���� ������� �������� = �
		// �������� ���������� ������������� ��� �� ���.
		BasicIterator& operator=(const BasicIterator& rhs) = default;

		// �������� ��������� ���������� (� ���� ������� ��������� ��������� ����������� ��������)
		// ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������, ���� �� end()
		[[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept
		{
			return node_ == rhs.node_;
		}

		// �������� �������� ���������� �� �����������
		// �������������� !=
		[[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept
		{
			return node_ != rhs.node_;
		}

		// �������� ��������� ���������� (� ���� ������� ��������� ��������)
		// ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������, ���� �� end()
		[[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept
		{
			return node_ == rhs.node_;
		}

		// �������� �������� ���������� �� �����������
		// �������������� !=
		[[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept
		{
			return node_ != rhs.node_;
		}

		// �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
		// ���������� ������ �� ������ ����
		// ��������� ���������, �� ������������ �� ������������ ������� ������, �������� � �������������� ���������
		BasicIterator& operator++() noexcept
		{
			assert(node_ != nullptr);
			node_ = node_->next_node;
			return *this;
		}

		// �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������.
		// ���������� ������� �������� ���������
		// ��������� ���������, �� ������������ �� ������������ ������� ������,
		// �������� � �������������� ���������
		BasicIterator operator++(int) noexcept
		{
			assert(node_ != nullptr);
			auto old_value(*this);
			++(*this);
			return old_value;
		}

		// �������� �������������. ���������� ������ �� ������� �������
		// ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
		// �������� � �������������� ���������
		[[nodiscard]] reference operator*() const noexcept
		{
			assert(node_ != nullptr);
			return node_->value;
		}

		// �������� ������� � ����� ������. ���������� ��������� �� ������� ������� ������.
		// ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
		// �������� � �������������� ���������
		[[nodiscard]] pointer operator->() const noexcept
		{
			assert(node_ != nullptr);
			return &node_->value;
		}

	private:
		Node* node_ = nullptr;
	};

public:
	using value_type = Type;
	using reference = value_type&;
	using const_reference = const value_type&;

	// ��������, ����������� ��������� ��������� ������
	using Iterator = BasicIterator<Type>;
	// ����������� ��������, ��������������� ������ ��� ������ � ��������� ������
	using ConstIterator = BasicIterator<const Type>;

	// ���������� ��������, ����������� �� ������ �������
	// ���� ������ ������, ������������ �������� ����� ����� end()
	[[nodiscard]] Iterator begin() noexcept
	{
		return Iterator(head_.next_node);
	}

	// ���������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
	// �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
	[[nodiscard]] Iterator end() noexcept
	{
		return Iterator(nullptr);
	}

	// ���������� ����������� ��������, ����������� �� ������ �������
	// ���� ������ ������, ������������ �������� ����� ����� end()
	// ��������� ������ ������������ ������ ������ cbegin()
	[[nodiscard]] ConstIterator begin() const noexcept
	{
		return ConstIterator(head_.next_node);
	}

	// ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
	// �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
	// ��������� ������ ������������ ������ ������ cend()
	[[nodiscard]] ConstIterator end() const noexcept
	{
		return ConstIterator(nullptr);
	}

	// ���������� ����������� ��������, ����������� �� ������ �������
	// ���� ������ ������, ������������ �������� ����� ����� cend()
	[[nodiscard]] ConstIterator cbegin() const noexcept
	{
		return begin();
	}

	// ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
	// �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
	[[nodiscard]] ConstIterator cend() const noexcept
	{
		return end();
	}

	// ���������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
	// �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
	[[nodiscard]] Iterator before_begin() noexcept
	{
		return Iterator(&head_);
	}

	// ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
	// �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
	[[nodiscard]] ConstIterator before_begin() const noexcept
	{
		return ConstIterator(const_cast<Node*>(&head_));
	}

	// ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
	// �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
	[[nodiscard]] ConstIterator cbefore_begin() const noexcept
	{
		return before_begin();
	}

	SingleLinkedList() = default;

	template <typename InputIterator>
	void CopyAndSwap(InputIterator from, InputIterator to)
	{
		SingleLinkedList<Type> tmp;

		Node** node_ptr = &tmp.head_.next_node;
		while (from != to)
		{
			*node_ptr = new Node(*from, nullptr);
			node_ptr = &(*node_ptr)->next_node;
			++tmp.size_;
			++from;
		}

		swap(tmp);
	}

	SingleLinkedList(std::initializer_list<Type> values)
	{
		SingleLinkedList tmp;
		CopyAndSwap(values.begin(), values.end());
	}

	SingleLinkedList(const SingleLinkedList& other)
	{
		assert(size_ == 0 && head_.next_node == nullptr);
		CopyAndSwap(other.begin(), other.end());
	}

	SingleLinkedList& operator=(const SingleLinkedList& rhs)
	{
		if (this != &rhs)
		{
			SingleLinkedList rhs_copy(rhs);
			swap(rhs_copy);
		}
		return *this;
	}

	// ���������� ���������� ������� �� ����� O(1)
	void swap(SingleLinkedList& other) noexcept
	{
		std::swap(head_.next_node, other.head_.next_node);
		std::swap(size_, other.size_);
	}

	~SingleLinkedList()
	{
		Clear();
	}
	// ���������� ���������� ��������� � ������ �� ����� O(1)
	[[nodiscard]] size_t GetSize() const noexcept
	{
		return size_;
	}

	// ��������, ������ �� ������ �� ����� O(1)
	[[nodiscard]] bool IsEmpty() const noexcept
	{
		return (size_ == 0);
	}

	void PushFront(const Type& value)
	{
		head_.next_node = new Node(value, head_.next_node);
		++size_;
	}

	// ��������� ������� value ����� ��������, �� ������� ��������� pos.
	// ���������� �������� �� ����������� �������
	// ���� ��� �������� �������� ����� ��������� ����������, ������ ��������� � ������� ���������
	Iterator InsertAfter(ConstIterator pos, const Type& value)
	{
		assert(pos.node_ != nullptr);
		Node* new_node = new Node(value, pos.node_->next_node);
		pos.node_->next_node = new_node;
		++size_;
		return Iterator(new_node);
	}

	void PopFront() noexcept {
		assert(size_ > 0);
		Node* second_node = begin().node_->next_node;
		delete begin().node_;
		head_.next_node = second_node;
		--size_;
	}

	// ������� �������, ��������� �� pos.
	// ���������� �������� �� �������, ��������� �� ��������
	Iterator EraseAfter(ConstIterator pos) noexcept
	{
		assert(pos.node_ != nullptr);
		Node* deletion_node = pos.node_->next_node;
		pos.node_->next_node = deletion_node->next_node;
		delete deletion_node;
		--size_;
		return Iterator(pos.node_->next_node);
	}

	void Clear() noexcept
	{
		while (head_.next_node)
		{
			Node* buf = head_.next_node->next_node;
			delete head_.next_node;
			head_.next_node = buf;
		}
		size_ = 0;
	}

private:
	// ��������� ����, ������������ ��� ������� "����� ������ ���������"
	Node head_;
	size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept
{
	lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
	return !(rhs == lhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
	return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
	return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
	return !(lhs < rhs);
}
