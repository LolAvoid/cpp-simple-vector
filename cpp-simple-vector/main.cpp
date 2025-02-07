#include "simple_vector.h"
#include <cassert>
#include <iostream>
#include <numeric>

using namespace std;

class X {
public:
    X() : X(5) {}
    explicit X(size_t num) : x_(num) {}
    X(const X&) = delete;
    X& operator=(const X&) = delete;
    X(X&& other) noexcept {
        x_ = other.x_;
        other.x_ = 0;
    }
    X& operator=(X&& other) noexcept {
        if (this != &other) {
            x_ = other.x_;
            other.x_ = 0;
        }
        return *this;
    }
    size_t GetX() const {
        return x_;
    }

private:
    size_t x_;
};

SimpleVector<int> GenerateVector(size_t size) {
    SimpleVector<int> v(size);
    iota(v.begin(), v.end(), 1);
    return v;
}

void TestTemporaryObjConstructor() {
    const size_t size = 1000000;
    cout << "Testing temporary object constructor with copy elision" << endl;
    SimpleVector<int> moved_vector(GenerateVector(size));
    assert(moved_vector.Size() == size);
    cout << "Test passed!" << endl << endl;
}

void TestTemporaryObjOperator() {
    const size_t size = 1000000;
    cout << "Testing temporary object assignment operator" << endl;
    SimpleVector<int> moved_vector;
    assert(moved_vector.Size() == 0);
    moved_vector = GenerateVector(size);
    assert(moved_vector.Size() == size);
    cout << "Test passed!" << endl << endl;
}

void TestNamedMoveConstructor() {
    const size_t size = 1000000;
    cout << "Testing named object move constructor" << endl;
    SimpleVector<int> vector_to_move(GenerateVector(size));
    assert(vector_to_move.Size() == size);

    SimpleVector<int> moved_vector(std::move(vector_to_move));
    assert(moved_vector.Size() == size);
    assert(vector_to_move.Size() == 0);
    cout << "Test passed!" << endl << endl;
}

void TestNamedMoveOperator() {
    const size_t size = 1000000;
    cout << "Testing named object move assignment operator" << endl;
    SimpleVector<int> vector_to_move(GenerateVector(size));
    assert(vector_to_move.Size() == size);

    SimpleVector<int> moved_vector = std::move(vector_to_move);
    assert(moved_vector.Size() == size);
    assert(vector_to_move.Size() == 0);
    cout << "Test passed!" << endl << endl;
}

void TestNoncopiableMoveConstructor() {
    const size_t size = 5;
    cout << "Testing non-copiable object move constructor" << endl;
    SimpleVector<X> vector_to_move;
    for (size_t i = 0; i < size; ++i) {
        vector_to_move.PushBack(X(i));
    }

    SimpleVector<X> moved_vector = std::move(vector_to_move);
    assert(moved_vector.Size() == size);
    assert(vector_to_move.Size() == 0);

    for (size_t i = 0; i < size; ++i) {
        assert(moved_vector[i].GetX() == i);
    }
    cout << "Test passed!" << endl << endl;
}

void TestNoncopiablePushBack() {
    const size_t size = 5;
    cout << "Testing non-copiable push back" << endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.PushBack(X(i));
    }

    assert(v.Size() == size);

    for (size_t i = 0; i < size; ++i) {
        assert(v[i].GetX() == i);
    }
    cout << "Test passed!" << endl << endl;
}

void TestNoncopiableInsert() {
    const size_t size = 5;
    cout << "Testing non-copiable insert" << endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.PushBack(X(i));
    }

    v.Insert(v.begin(), X(size + 1));
    assert(v.Size() == size + 1);
    assert(v.begin()->GetX() == size + 1);

    v.Insert(v.end(), X(size + 2));
    assert(v.Size() == size + 2);
    assert((v.end() - 1)->GetX() == size + 2);

    v.Insert(v.begin() + 3, X(size + 3));
    assert(v.Size() == size + 3);
    assert((v.begin() + 3)->GetX() == size + 3);
    cout << "Test passed!" << endl << endl;
}

void TestNoncopiableErase() {
    const size_t size = 3;
    cout << "Testing non-copiable erase" << endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.PushBack(X(i));
    }

    auto it = v.Erase(v.begin());
    assert(it->GetX() == 1);
    cout << "Test passed!" << endl << endl;
}

int main() {
    TestTemporaryObjConstructor();
    TestTemporaryObjOperator();
    TestNamedMoveConstructor();
    TestNamedMoveOperator();
    TestNoncopiableMoveConstructor();
    TestNoncopiablePushBack();
    TestNoncopiableInsert();
    TestNoncopiableErase();

    return 0;
}