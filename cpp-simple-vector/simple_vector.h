#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include "array_ptr.h"

class ReserveProxy {
public:
    explicit ReserveProxy(size_t capacity) : capacity_(capacity) {}

    size_t GetCapacity() const noexcept {
        return capacity_;
    }

private:
    size_t capacity_;
};

ReserveProxy Reserve(size_t capacity) {
    return ReserveProxy(capacity);
}

template <typename T>
class SimpleVector {
public:
    using Iterator = T*;
    using ConstIterator = const T*;

    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size) : size_(size), capacity_(size), data_(size) {
        std::fill(data_.Get(), data_.Get() + size_, T());
    }

    SimpleVector(size_t size, const T& value) : size_(size), capacity_(size), data_(size) {
        std::fill(data_.Get(), data_.Get() + size_, value);
    }

    SimpleVector(std::initializer_list<T> init) : size_(init.size()), capacity_(init.size()), data_(init.size()) {
        std::copy(init.begin(), init.end(), data_.Get());
    }

    SimpleVector(const SimpleVector& other) : size_(other.size_), capacity_(other.capacity_), data_(other.size_) {
        std::copy(other.data_.Get(), other.data_.Get() + size_, data_.Get());
    }

    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            SimpleVector temp(other);
            Swap(temp);
        }
        return *this;
    }

    SimpleVector(SimpleVector&& other) noexcept {
        Swap(other);
    }

    SimpleVector& operator=(SimpleVector&& other) noexcept {
        if (this != &other) {
            data_.Swap(other.data_);
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    SimpleVector(ReserveProxy proxy) {
        Reserve(proxy.GetCapacity());
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ArrayPtr<T> new_data(new_capacity);
            std::move(data_.Get(), data_.Get() + size_, new_data.Get());
            data_.Swap(new_data);
            capacity_ = new_capacity;
        }
    }

    size_t Size() const noexcept {
        return size_;
    }

    size_t Capacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    T& operator[](size_t index) noexcept {
        return data_[index];
    }

    const T& operator[](size_t index) const noexcept {
        return data_[index];
    }

    T& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    const T& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    void Clear() noexcept {
        size_ = 0;
    }

    void Resize(size_t new_size) {
        if (new_size > capacity_) {
            Reserve(new_size);
        }
        for (size_t i = size_; i < new_size; ++i) {
            data_[i] = T();
        }
        size_ = new_size;
    }

    Iterator begin() noexcept {
        return data_.Get();
    }

    Iterator end() noexcept {
        return data_.Get() + size_;
    }

    ConstIterator begin() const noexcept {
        return data_.Get();
    }

    ConstIterator end() const noexcept {
        return data_.Get() + size_;
    }

    ConstIterator cbegin() const noexcept {
        return data_.Get();
    }

    ConstIterator cend() const noexcept {
        return data_.Get() + size_;
    }

    void PushBack(T&& value) {
        if (size_ == capacity_) {
            Reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_] = std::move(value);
        ++size_;
    }

    Iterator Insert(ConstIterator pos, T&& value) {
        size_t index = pos - begin();
        if (size_ == capacity_) {
            Reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        std::move_backward(begin() + index, end(), end() + 1);
        data_[index] = std::move(value);
        ++size_;
        return begin() + index;
    }

    void PopBack() noexcept {
        if (size_ > 0) {
            --size_;
        }
    }

    Iterator Erase(ConstIterator pos) {
        size_t index = pos - begin();
        std::move(begin() + index + 1, end(), begin() + index);
        --size_;
        return begin() + index;
    }

    void Swap(SimpleVector& other) noexcept {
        data_.Swap(other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    ArrayPtr<T> data_;
};

template <typename T>
bool operator==(const SimpleVector<T>& lhs, const SimpleVector<T>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T>
bool operator!=(const SimpleVector<T>& lhs, const SimpleVector<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const SimpleVector<T>& lhs, const SimpleVector<T>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T>
bool operator<=(const SimpleVector<T>& lhs, const SimpleVector<T>& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

template <typename T>
bool operator>(const SimpleVector<T>& lhs, const SimpleVector<T>& rhs) {
    return rhs < lhs;
}

template <typename T>
bool operator>=(const SimpleVector<T>& lhs, const SimpleVector<T>& rhs) {
    return (lhs > rhs) || (lhs == rhs);
}
