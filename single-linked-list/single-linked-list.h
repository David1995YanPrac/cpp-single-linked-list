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
            if (node_ != nullptr) {
                node_ = node_->next_node;
            }
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator n(*this);
            operator++();
            return n;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };


public:

    SingleLinkedList() :size_(0u) { size_ = 0u; };

    ~SingleLinkedList() {
        Clear();
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PushBack(const Type& value) {
        Node* lnode = new Node(value, nullptr);
        if (head_.next_node == nullptr) {
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
        return (size_ == 0) ? true : false;
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        size_ = 0;
        for (auto i = values.begin(); i != values.end(); ++i) {
            PushBack(*i);
        }
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);

        SingleLinkedList tmp;

        for (auto i = other.begin(); i != other.end(); ++i) {
            tmp.PushBack(*i);
        }
        swap(tmp);
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
        if (pos == end()) {
            PushBack(value);
            return Iterator(end());
        }

        Node* new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++size_;
        return Iterator(new_node);
    }

    Iterator EraseAfter(ConstIterator pos) noexcept
    {
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
        auto end_node = head_.next_node;
        while (end_node != nullptr) {
            end_node = end_node->next_node;
        }
        return Iterator{ end_node };
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        auto end_node = head_.next_node;
        while (end_node != nullptr) {
            end_node = end_node->next_node;
        }
        return ConstIterator{ end_node };
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        auto end_node = head_.next_node;
        while (end_node != nullptr) {
            end_node = end_node->next_node;
        }
        return ConstIterator{ end_node };
    }

private:

    Node head_;
    size_t size_ = 0u;

};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());

}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())
        || std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end())
        || std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

void Test4()
{
    struct DeletionSpy {
        ~DeletionSpy() {
            if (deletion_counter_ptr) {
                ++(*deletion_counter_ptr);
            }
        }
        int* deletion_counter_ptr = nullptr;
    };

    {
        SingleLinkedList<int> numbers{ 3, 14, 15, 92, 6 };
        numbers.PopFront();
        assert((numbers == SingleLinkedList<int>{14, 15, 92, 6}));

        SingleLinkedList<DeletionSpy> list;
        list.PushFront(DeletionSpy{});
        int deletion_counter = 0;
        list.begin()->deletion_counter_ptr = &deletion_counter;
        assert(deletion_counter == 0);
        list.PopFront();
        assert(deletion_counter == 1);
    }

    {
        SingleLinkedList<int> empty_list;
        const auto& const_empty_list = empty_list;
        assert(empty_list.before_begin() == empty_list.cbefore_begin());
        assert(++empty_list.before_begin() == empty_list.begin());
        assert(++empty_list.cbefore_begin() == const_empty_list.begin());

        SingleLinkedList<int> numbers{ 1, 2, 3, 4 };
        const auto& const_numbers = numbers;
        assert(numbers.before_begin() == numbers.cbefore_begin());
        assert(++numbers.before_begin() == numbers.begin());
        assert(++numbers.cbefore_begin() == const_numbers.begin());
    }

    {
        {
            SingleLinkedList<int> lst;
            const auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
            assert((lst == SingleLinkedList<int>{123}));
            assert(inserted_item_pos == lst.begin());
            assert(*inserted_item_pos == 123);
        }

        {
            SingleLinkedList<int> lst{ 1, 2, 3 };
            auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);

            assert(inserted_item_pos == lst.begin());
            assert(inserted_item_pos != lst.end());
            assert(*inserted_item_pos == 123);
            assert((lst == SingleLinkedList<int>{123, 1, 2, 3}));

            inserted_item_pos = lst.InsertAfter(lst.begin(), 555);
            assert(++SingleLinkedList<int>::Iterator(lst.begin()) == inserted_item_pos);
            assert(*inserted_item_pos == 555);
            assert((lst == SingleLinkedList<int>{123, 555, 1, 2, 3}));
        };
    }

    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                }
                else {
                    --(*countdown_ptr);
                }
            }
        }

        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;

        int* countdown_ptr = nullptr;
    };

    {
        bool exception_was_thrown = false;
        for (int max_copy_counter = 10; max_copy_counter >= 0; --max_copy_counter) {
            SingleLinkedList<ThrowOnCopy> list{ ThrowOnCopy{}, ThrowOnCopy{}, ThrowOnCopy{} };
            try {
                int copy_counter = max_copy_counter;
                list.InsertAfter(list.cbegin(), ThrowOnCopy(copy_counter));
                assert(list.GetSize() == 4u);
            }
            catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                assert(list.GetSize() == 3u);
                break;
            }
        }
        assert(exception_was_thrown);
    }

    {
        {
            SingleLinkedList<int> lst{ 1, 2, 3, 4 };
            const auto& const_lst = lst;
            const auto item_after_erased = lst.EraseAfter(const_lst.cbefore_begin());
            assert((lst == SingleLinkedList<int>{2, 3, 4}));
            assert(item_after_erased == lst.begin());
        }
        {
            SingleLinkedList<int> lst{ 1, 2, 3, 4 };
            const auto item_after_erased = lst.EraseAfter(lst.cbegin());
            assert((lst == SingleLinkedList<int>{1, 3, 4}));
            assert(item_after_erased == (++lst.begin()));
        }
        {
            SingleLinkedList<int> lst{ 1, 2, 3, 4 };
            const auto item_after_erased = lst.EraseAfter(++(++lst.cbegin()));
            assert((lst == SingleLinkedList<int>{1, 2, 3}));
            assert(item_after_erased == lst.end());
        }
        {
            SingleLinkedList<DeletionSpy> list{ DeletionSpy{}, DeletionSpy{}, DeletionSpy{} };
            auto after_begin = ++list.begin();
            int deletion_counter = 0;
            after_begin->deletion_counter_ptr = &deletion_counter;
            assert(deletion_counter == 0u);
            list.EraseAfter(list.cbegin());
            assert(deletion_counter == 1u);
        }
    }
}

int main()
{
    Test4();
}
