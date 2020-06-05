//
//  Vector.hpp
//  elixir
//
//  Created by stienb on 14/01/2019.
//  Copyright © 2019 Pretty Simple. All rights reserved.
//

#pragma once

#include <stdlib.h>

namespace elixir {
    
    template <typename T>
    class Vector {
    private:
        
        static constexpr float EXTRA_CAPACITY_FACTOR = 1.75f;
        
        size_t _capacity = 0;
        size_t _size = 0;
        T* _data = nullptr;
        
    public:
        using Iterator = T*;
        using ConstIterator = const T*;
        
        Vector() noexcept = default;
        
        explicit Vector(size_t count) {
            resize(count);
        }

        Vector(size_t count, const T& initialValue) {
            resize(count, initialValue);
        }

        template <typename U>
        Vector(U* first, U* last) {
            assign(first, last);
        }

        Vector(std::initializer_list<T> init) : Vector(init.begin(), init.end()) {
        }

        template <size_t N>
        using CArray = const T[N];

        template <size_t N>
        Vector(CArray<N> const& other) : Vector(other, other + N) {
        }
        
        Vector(const Vector& other) : Vector(other.begin(), other.end()) {
        }

        Vector(Vector&& other) noexcept {
            _size = other._size;
            _capacity = other._capacity;
            _data = other._data;
            other._size = 0;
            other._capacity = 0;
            other._data = nullptr;
        }

        ~Vector() {
            clear();
            free(_data);
        }
        
        Vector& operator=(const Vector& other) {
            assign(other.begin(), other.end());
            return (*this);
        }

        Vector& operator=(Vector&& other) noexcept {
            clear();
            free(_data);
            _size = other._size;
            _capacity = other._capacity;
            _data = other._data;
            other._size = 0;
            other._capacity = 0;
            other._data = nullptr;
            return (*this);
        }
        
        template <size_t N>
        Vector& operator=(CArray<N> const& other) {
            assign(other, other + N);
            return (*this);
        }
        
        template<typename U>
        void assign(U* first, U* last) {
            size_t count = static_cast<size_t>(last - first);
            if (count <= _size) {
                for (size_t i = 0; i < count; ++i) {
                    _data[i] = first[i];
                }
                for (size_t i = count; i < _size; ++i) {
                    (_data + i)->~T();
                }
            } else {
                reserve(static_cast<size_t>(count * EXTRA_CAPACITY_FACTOR));
                for (size_t i = 0; i < _size; ++i) {
                    _data[i] = first[i];
                }
                for (size_t i = _size; i < count; ++i) {
                    new(_data + i) T(first[i]);
                }
            }
            _size = count;
        }

        void assign(std::initializer_list<T> list) {
            assign(list.begin(), list.end());
        }

#if defined(__EMSCRIPTEN__)

        // Emscripten only
        // Reusing a pre-malloc'd pointer, used for bulk JS->CPP data transferts (usually uint8_t) without additional copy.
        void em_move(T* ptr, size_t size)
        {
            free(_data);
            _data = ptr;
            _size = size;
            _capacity = size;
        }

#endif

        T& at(size_t pos) {
            assert (pos < _size);
            return _data[pos];
        }
        
        const T& at(size_t pos) const {
            assert (pos < _size);
            return _data[pos];
        }

        T& operator[](size_t pos) {
            assert (pos < _size);
            return _data[pos];
        }
        
        const T& operator[](size_t pos) const {
            assert (pos < _size);
            return _data[pos];
        }
        
        T& front() {
            return *begin();
        }

        const T& front() const {
            return *begin();
        }

        T& back() {
            return *(end() - 1);
        }
        
        const T& back() const {
            return *(end() - 1);
        }
        
        T* data() {
            return _data;
        }
        
        const T* data() const {
            return _data;
        }

        T* begin() {
            return _data;
        }

        const T* begin() const {
            return _data;
        }

        T* end() {
            return _data + _size;
        }

        const T* end() const {
            return _data + _size;
        }
        
        bool empty() const {
            return (_size == 0);
        }
        
        size_t size() const {
            return _size;
        }
        
        size_t capacity() const {
            return _capacity;
        }

        void reserve(size_t capacity) {
            if (capacity > _capacity) {
                _capacity = capacity < 8 ? 8 : capacity;
                if (_data == nullptr) {
                    _data = static_cast<T*>(malloc(_capacity * sizeof(T)));
                } else {
                    _data = static_cast<T*>(realloc(_data, _capacity * sizeof(T)));
                }
            }
        }

        void clear() {
            resize(0);
        }

        void shrinkToFit() {
            _capacity = _size;
            _data = static_cast<T*>(realloc(_data, _size * sizeof(T)));
        }
        
        void insert(ConstIterator pos, const T& value) {
            assert(pos >= begin());
            assert(pos <= end());
            if (pos == end()) {
                pushBack(value);
            } else {
                size_t index = static_cast<size_t>(pos - begin());
                reserve(_size + 1);
                new(_data + _size) T(_data[_size - 1]);
                for (size_t i = _size - 1; i > index ; --i) {
                    _data[i] = _data[i - 1];
                }
                _data[index] = value;
                ++_size;
            }
        }

        void insert(ConstIterator pos, T&& value) {
            assert(pos >= begin());
            assert(pos <= end());
            if (pos == end()) {
                pushBack(move(value));
            } else {
                size_t index = static_cast<size_t>(pos - begin());
                reserve(_size + 1);
                new(_data + _size) T(move(_data[_size - 1]));
                for (size_t i = _size - 1; i > index ; --i) {
                    _data[i] = move(_data[i - 1]);
                }
                _data[index] = move(value);
                ++_size;
            }
        }
        
        template <typename U>
        void insert(ConstIterator pos, U* firstIt, U* lastIt) {
            assert(pos >= begin());
            assert(pos <= end());
            size_t count = static_cast<size_t>(lastIt - firstIt);
            size_t index = static_cast<size_t>(pos - begin());
            reserve(_size + count);
            for (size_t i = _size + count; i > index + count; --i) {
                _data[i - 1] = move(_data[i - 1 - count]);
            }
            for (size_t i = 0; i < count; ++i) {
                new(_data + index + i) T(firstIt[i]);
            }
            _size += count;
        }

        void insert(ConstIterator pos, std::initializer_list<T> list) {
            insert(pos, list.begin(), list.end());
        }

        Iterator erase(ConstIterator pos) {
            assert(pos >= begin());
            assert(pos != end());
            size_t index = static_cast<size_t>(pos - begin());
            for (size_t i = index; i < _size - 1 ; ++i) {
                _data[i] = move(_data[i + 1]);
            }
            (_data + _size - 1)->~T();
            --_size;
            return begin() + index;
        }

        Iterator erase(ConstIterator first, ConstIterator last) {
            assert(first <= last);
            const size_t res = static_cast<size_t>(first - begin());
            if (first != last) {
                size_t index = res;
                for (size_t i = static_cast<size_t>(last - begin()); i < _size; ++i, ++index) {
                    _data[index] = move(_data[i]);
                }
                for (size_t i = index; i < _size; ++i) {
                    (_data + i)->~T();
                }
                _size -= (last - first);
            }
            return begin() + res;
        }
        
        void pushBack(const T& value) {
            reserve(_size + 1);
            new(_data + _size) T(value);
            ++_size;
        }

        void pushBack(T&& value) {
            reserve(_size + 1);
            new(_data + _size) T(move(value));
            ++_size;
        }

        template <class... Args>
        void emplaceBack(Args&&... args) {
            reserve(_size + 1);
            new (_data + _size) T(forward<Args>(args)...);
            ++_size;
        }
        
        void popBack() {
            assert(_size > 0);
            --_size;
            (_data + _size)->~T();
        }

        void resize(size_t count) {
            for (size_t i = count; i < _size; ++i) {
                (_data + i)->~T();
            }
            if (_capacity < count) {
                reserve(static_cast<size_t>(count * EXTRA_CAPACITY_FACTOR));
            }
            for (size_t i = _size; i < count; ++i) {
                new(_data + i) T{};
            }
            _size = count;
        }
        
        void resize(size_t count, const T& value) {
            for (size_t i = count; i < _size; ++i) {
                (_data + i)->~T();
            }
            if (_capacity < count) {
                reserve(static_cast<size_t>(count * EXTRA_CAPACITY_FACTOR));
            }
            for (size_t i = _size; i < count; ++i) {
                new(_data + i) T(value);
            }
            _size = count;
        }
        
        bool contains(const T& value) const {
            for (size_t i = 0; i < _size; ++i) {
                if (_data[i] == value) {
                    return true;
                }
            }
            return false;
        }
        
    };
}
