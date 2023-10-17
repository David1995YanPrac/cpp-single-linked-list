#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <iostream>

template <typename Type>
class SingleLinkedList
{

    struct Node
    {
        Node() = default;

        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {

        friend class SingleLinkedList;

        explicit BasicIterator(Node* node)
        {
            node_ = node;
        }

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {}

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;

        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(node_ == rhs.node_);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(node_ == rhs.node_);
        }

        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator n(*this);
            operator++();
            return n;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };


public:

    SingleLinkedList() :size_(0u) { size_ = 0u; };

    ~SingleLinkedList() 
    {
        Clear();
    }

    void PushFront(const Type& value) 
    {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PushBack(const Type& value)
    {
        Node* lnode = new Node(value, nullptr);
        if (head_.next_node == nullptr)
        {
            head_.next_node = lnode;
        }
        else {
            Node* end_node = head_.next_node;
            for (int i = 0; i != size_ - 1; ++i) {
                end_node = end_node->next_node;
            }
            end_node->next_node = lnode;
        }
        ++size_;
    }

    void PopFront() noexcept
    {
        assert(size_ > 0);
        Node* temp_node = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        delete temp_node;
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            auto next_node_afte_delete = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = next_node_afte_delete;
            --size_;
        }
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return (size_ == 0);
    }

    
    SingleLinkedList(std::initializer_list<Type> values)
    {
        PushBackValues(values);
    }

    SingleLinkedList(const SingleLinkedList& other) {

        PushBackValues(other);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs)
    {
        if (this != &rhs) {
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept
    {
        std::swap(other.head_.next_node, head_.next_node);
        std::swap(other.size_, size_);
    }

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    Iterator InsertAfter(ConstIterator pos, const Type& value) {

        assert(pos.node_ != nullptr);
        Node* new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++size_;
        return Iterator(new_node);
    }

    Iterator EraseAfter(ConstIterator pos) noexcept
    {
        assert(pos.node_ != nullptr && pos.node_->next_node != nullptr && size_ > 0);
        Node* deleting_node = pos.node_->next_node;
        pos.node_->next_node = pos.node_->next_node->next_node;
        delete deleting_node;
        return Iterator(pos.node_->next_node);
    }

    [[nodiscard]] Iterator before_begin() noexcept
    {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept
    {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept
    {
        return cbefore_begin();
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cend() const {
        return ConstIterator{ nullptr };
    }

private:

    Node head_;
    size_t size_ = 0u;

    void PushBackValues(std::initializer_list<Type> values)
    {
        Node* tmp_head = &head_;

        for (const Type& value : values)
        {
            tmp_head->next_node = new Node(value, nullptr);
            tmp_head = tmp_head->next_node;
        }

        size_ = values.size();
    }

};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    if (&lhs == &rhs)
    {
        return true;
    }

    if (lhs.GetSize() != rhs.GetSize())
    {
        return false;
    }

    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());

}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs <= rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return !(lhs < rhs);
}