#ifndef ARRAY_PTR_H
#define ARRAY_PTR_H

#include <cassert>
#include <cstdlib>
#include <utility> // Для std::swap

template <typename T>
class ArrayPtr {
public:
    ArrayPtr() = default;

    explicit ArrayPtr(size_t size) {
        data_ = (size == 0) ? nullptr : new T[size];
    }

    explicit ArrayPtr(T* raw_ptr) noexcept : data_(raw_ptr) {}

    ArrayPtr(const ArrayPtr&) = delete;
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    ArrayPtr(ArrayPtr&& other) noexcept {
        data_ = other.data_;
        other.data_ = nullptr;
    }

    ArrayPtr& operator=(ArrayPtr&& other) noexcept {
        if (this != &other) {
            delete[] data_; 
            data_ = other.data_; 
            other.data_ = nullptr; 
        }
        return *this;
    }

    ~ArrayPtr() {
        delete[] data_;
    }

    T* Release() noexcept {
        T* temp = data_;
        data_ = nullptr;
        return temp;
    }

    T& operator[](size_t index) noexcept {
        return data_[index];
    }

    const T& operator[](size_t index) const noexcept {
        return data_[index];
    }

    explicit operator bool() const noexcept {
        return data_ != nullptr;
    }

    T* Get() const noexcept {
        return data_;
    }

    void Swap(ArrayPtr& other) noexcept {
        std::swap(data_, other.data_);
    }

private:
    T* data_ = nullptr;
};

#endif // ARRAY_PTR_H
