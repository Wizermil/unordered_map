//
//  UnorderedMap.hpp
//  elixir
//
//  Created by stienb on 15/01/2019.
//  Copyright © 2019 Pretty Simple. All rights reserved.
//

#pragma once

#include <stddef.h>

#include <type_traits>
#include <vector>
#include <utility>
#include <functional>

#include "Vector.hpp"

namespace elixir {

    template <typename T>
    struct hash;

#define ELIXIR_HASH_SPECIALIZE_BY_VALUE(type)                           \
    template <>                                                         \
    struct hash<type> {                                                 \
        constexpr size_t operator()(type value) const noexcept {        \
            return static_cast<size_t>(value);                          \
        }                                                               \
    };

    ELIXIR_HASH_SPECIALIZE_BY_VALUE(bool)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(char)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(signed char)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(signed short)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(signed int)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(signed long)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(signed long long)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(unsigned char)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(unsigned short)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(unsigned int)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(unsigned long)
    ELIXIR_HASH_SPECIALIZE_BY_VALUE(unsigned long long)

#undef ELIXIR_HASH_SPECIALIZE_BY_VALUE

    template <typename T>
    struct hash<T*> {
        inline size_t operator()(T* value) const noexcept {
            size_t const x = reinterpret_cast<size_t>(value);
            return x + (x >> 3);
        }
    };

    template <typename T1, typename T2 = T1>
    struct equal_to {
        constexpr bool operator()(T1 x, T1 y) const noexcept(noexcept(x == y)) {
            return x == y;
        }
        constexpr bool operator()(T1 x, T2 y) const noexcept(noexcept(x == y)) {
            return x == y;
        }
        constexpr bool operator()(T2 x, T1 y) const noexcept(noexcept(x == y)) {
            return x == y;
        }
        constexpr bool operator()(T2 x, T2 y) const noexcept(noexcept(x == y)) {
            return x == y;
        }
    };

    template <typename T1>
    struct equal_to<T1, T1> {
        constexpr bool operator()(T1 x, T1 y) const noexcept(noexcept(x == y)) {
            return x == y;
        }
    };

    template <typename T1>
    struct equal_to<T1 const, T1> {
        constexpr bool operator()(T1 x, T1 y) const noexcept(noexcept(x == y)) {
            return x == y;
        }
    };

    template <typename T1>
    struct equal_to<T1, T1 const> {
        constexpr bool operator()(T1 x, T1 y) const noexcept(noexcept(x == y)) {
            return x == y;
        }
    };


    template <class K, class V, typename Hash = elixir::hash<K>, typename KeyEqual = elixir::equal_to<K>>
    class UnorderedMap {
    private:
        static constexpr size_t DEFAULT_BUCKET_COUNT = 23;

    public:
        using KeyType = std::remove_const_t<K>;
        using MappedType = V;
        using ValueType = std::pair<const KeyType, V>;
        using hasher = Hash;
        using key_equal = KeyEqual;

    private:
        struct Entry {
            ValueType kv;
            Entry* next = nullptr;
        };

        Vector<Entry*> _buckets;
        size_t _size = 0;

    public:
        class Iterator {
            using pointer = ValueType*;
            using reference = ValueType&;

            friend UnorderedMap;

        private:
            const UnorderedMap* _umap = nullptr;
            size_t _bucket = 0;
            Entry* _entry = nullptr;

        public:
            constexpr Iterator() noexcept = default;
            constexpr Iterator(const UnorderedMap* umap, size_t bucket, Entry* entry)
            : _umap(umap)
            , _bucket(bucket)
            , _entry(entry) {}
            constexpr Iterator(const Iterator& other) = default;
            constexpr Iterator& operator=(const Iterator&) = default;
            constexpr Iterator(Iterator&& other) noexcept = default;
            constexpr Iterator& operator=(Iterator&& other) noexcept = default;

            friend constexpr bool operator==(const Iterator& lhs, const Iterator& rhs) {
                return (lhs._bucket == rhs._bucket && lhs._entry == rhs._entry); // compare _umap and other._umap?
            }

            friend constexpr bool operator!=(const Iterator& lhs, const Iterator& rhs) { return !(lhs == rhs); }

            constexpr Iterator& operator++() { // ++IteratorBase
                if (_entry->next != nullptr) {
                    _entry = _entry->next;
                } else {
                    ++_bucket;
                    const size_t bucketCount = _umap->bucketCount();
                    while (_bucket < bucketCount && _umap->_buckets[_bucket] == nullptr) {
                        ++_bucket;
                    }
                    if (_bucket == bucketCount) {
                        _entry = nullptr;
                    } else {
                        _entry = _umap->_buckets[_bucket];
                    }
                }
                return *this;
            }

            constexpr Iterator operator++(int) { // IteratorBase++
                auto tmp = *this;
                ++(*this);
                return tmp;
            }

            constexpr reference operator*() const {
                assert(_entry != nullptr); // invalid iterator
                return _entry->kv;
            }

            constexpr pointer operator->() const {
                assert(_entry != nullptr); // invalid iterator
                return &operator*();
            }
        };

        class ConstIterator {
            Iterator _inner;

            friend UnorderedMap;

        public:
            using reference = const ValueType&;
            using pointer = const ValueType*;

            constexpr ConstIterator() noexcept = default;
            // Implicit construction from iterator.
            constexpr ConstIterator(Iterator i)
            : _inner(std::move(i)) {}
            constexpr ConstIterator(const ConstIterator&) = default;
            constexpr ConstIterator& operator=(const ConstIterator&) = default;
            constexpr ConstIterator(ConstIterator&& other) noexcept = default;
            constexpr ConstIterator& operator=(ConstIterator&& other) noexcept = default;

            constexpr reference operator*() const { return *_inner; }

            constexpr pointer operator->() const { return _inner.operator->(); }

            constexpr ConstIterator& operator++() {
                ++_inner;
                return *this;
            }

            constexpr ConstIterator operator++(int) { return _inner++; }

            friend constexpr bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) { return lhs._inner == rhs._inner; }

            friend constexpr bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) { return !(lhs == rhs); }
        };

    public:
        UnorderedMap()
        : UnorderedMap(DEFAULT_BUCKET_COUNT) {}

        template <typename InputIt>
        UnorderedMap(InputIt first, InputIt last, size_t bucket_count = DEFAULT_BUCKET_COUNT)
        : UnorderedMap(bucket_count) {
            insert(first, last);
        }

        UnorderedMap(std::initializer_list<ValueType> init, size_t bucket_count = DEFAULT_BUCKET_COUNT)
        : UnorderedMap(init.begin(), init.end(), bucket_count) {}

        UnorderedMap(size_t bucket_count) {
            _buckets.resize(bucket_count);
            _buckets.shrinkToFit();
        }

        UnorderedMap(const UnorderedMap& other)
        : UnorderedMap(other.bucketCount()) {
            _size = other._size;
            for (size_t i = 0, max = bucketCount(); i < max; ++i) {
                _buckets[i] = cloneChain(other._buckets[i]);
            }
        }

        UnorderedMap(UnorderedMap&& other) noexcept = default;

        ~UnorderedMap() { clear(); }

        UnorderedMap& operator=(const UnorderedMap& other) {
            for (auto& b : _buckets) {
                deleteChain(b);
                b = nullptr;
            }
            size_t new_bucket_count = other.bucketCount();
            _buckets.resize(new_bucket_count);
            _buckets.shrinkToFit();
            for (size_t i = 0; i < new_bucket_count; ++i) {
                _buckets[i] = cloneChain(other._buckets[i]);
            }
            _size = other._size;
            return (*this);
        }

        UnorderedMap& operator=(UnorderedMap&& other) {
            for (auto& b : _buckets) {
                deleteChain(b);
            }
            _buckets = move(other._buckets);
            _size = other._size;
            other._size = 0;
            return (*this);
        }

        Iterator begin() {
            for (size_t i = 0, max = bucketCount(); i < max; ++i) {
                if (_buckets[i] != nullptr) {
                    return Iterator(this, i, _buckets[i]);
                }
            }
            return end();
        }

        ConstIterator begin() const {
            for (size_t i = 0, max = bucketCount(); i < max; ++i) {
                if (_buckets[i] != nullptr) {
                    return Iterator{this, i, _buckets[i]};
                }
            }
            return end();
        }

        Iterator end() { return Iterator(this, bucketCount(), nullptr); }

        ConstIterator end() const { return Iterator{this, bucketCount(), nullptr}; }

        bool empty() const { return _size == 0; }

        size_t size() const { return _size; }

        size_t bucket_count() const { return DEFAULT_BUCKET_COUNT; }

        void clear() {
            for (auto& b : _buckets) {
                deleteChain(b);
                b = nullptr;
            }
            _size = 0;
        }

        bool insert(const ValueType& value) { return addEntry(value); }

        bool insert(ValueType&& value) { return addEntry(move(value)); }

        void insert(std::initializer_list<ValueType> list) { insert(list.begin(), list.end()); }

        template <typename InputIt>
        void insert(InputIt first, InputIt last) {
            for (; first != last; ++first) {
                addEntry(*first);
            }
        }

        Iterator erase(ConstIterator pos) {
            const Entry* const key = pos._inner._entry;
            const size_t index = pos._inner._bucket;
            ++pos;

            Entry* entry = _buckets[index];
            Entry* prev = nullptr;
            while (entry != nullptr) {
                if (entry == key) {
                    // erase
                    if (prev != nullptr) {
                        prev->next = entry->next;
                    } else {
                        _buckets[index] = entry->next;
                    }
                    --_size;
                    delete entry;
                    return pos._inner;
                }
                prev = entry;
                entry = entry->next;
            }

            return pos._inner;
        }

        Iterator erase(ConstIterator first, ConstIterator last) {
            for (ConstIterator p = first; first != last; p = first) {
                ++first;
                erase(p);
            }
            return last._inner;
        }

        size_t erase(const KeyType& key) {
            const size_t index = hasher{}(key) % _buckets.size();
            Entry* entry = _buckets[index];
            Entry* prev = nullptr;
            while (entry != nullptr) {
                if (key_equal{}(entry->kv.first, key)) {
                    // erase
                    if (prev != nullptr) {
                        prev->next = entry->next;
                    } else {
                        _buckets[index] = entry->next;
                    }
                    --_size;
                    delete entry;
                    return 1;
                }
                prev = entry;
                entry = entry->next;
            }
            return 0;
        }

        V& at(const KeyType& key) {
            Entry* found = findEntry(key);
            assert(found); // key not found
            return found->kv.second;
        }

        const V& at(const KeyType& key) const {
            Entry* found = findEntry(key);
            assert(found); // key not found
            return found->kv.second;
        }

        V& operator[](const KeyType& key) {
            Entry* found = findOrCreateEntry(key);
            return found->kv.second;
        }

        V& operator[](KeyType&& key) {
            Entry* found = findOrCreateEntry(move(key));
            return found->kv.second;
        }

        int count(const KeyType& key) const { return findEntry(key) != nullptr ? 1 : 0; }

        Iterator find(const KeyType& key) {
            const size_t index = hasher{}(key) % _buckets.size();
            Entry* entry = _buckets[index];
            while (entry != nullptr) {
                if (key_equal{}(entry->kv.first, key)) {
                    return {this, index, entry};
                }
                entry = entry->next;
            }
            return end();
        }

        ConstIterator find(const KeyType& key) const {
            const size_t index = hasher{}(key) % _buckets.size();
            Entry* entry = _buckets[index];
            while (entry != nullptr) {
                if (key_equal{}(entry->kv.first, key)) {
                    return Iterator{this, index, entry};
                }
                entry = entry->next;
            }
            return end();
        }

        size_t bucketCount() const { return _buckets.size(); }

        float loadFactor() const { return size() / bucketCount(); }

    private:
        Entry* findEntry(const KeyType& key) const {
            const size_t index = hasher{}(key) % _buckets.size();
            Entry* entry = _buckets[index];
            while (entry != nullptr) {
                if (key_equal{}(entry->kv.first, key)) {
                    return entry;
                }
                entry = entry->next;
            }
            return nullptr;
        }

        template <class U>
        Entry* findOrCreateEntry(U&& key) {
            const size_t index = hasher{}(key) % _buckets.size();
            Entry* entry = _buckets[index];
            if (entry == nullptr) {
                _buckets[index] = new Entry{{forward<U>(key), V{}}, nullptr};
                ++_size;
                return _buckets[index];
            }
            while (entry->next != nullptr) {
                if (key_equal{}(entry->kv.first, key)) {
                    return entry;
                }
                entry = entry->next;
            }
            if (key_equal{}(entry->kv.first, key)) {
                return entry;
            } else {
                entry->next = new Entry{{forward<U>(key), V{}}, nullptr};
                ++_size;
                return entry->next;
            }
        }

        template <typename P>
        bool addEntry(P&& value) {
            const size_t index = hasher{}(value.first) % _buckets.size();
            Entry* entry = _buckets[index];
            if (entry == nullptr) {
                _buckets[index] = new Entry{forward<P>(value), nullptr};
                ++_size;
                return true;
            }
            while (entry->next != nullptr) {
                if (key_equal{}(entry->kv.first, value.first)) {
                    return false;
                }
                entry = entry->next;
            }
            if (key_equal{}(entry->kv.first, value.first)) {
                return false;
            } else {
                entry->next = new Entry{forward<P>(value), nullptr};
                ++_size;
                return true;
            }
        }

        Entry* cloneChain(Entry* head) {
            if (head == nullptr) {
                return nullptr;
            }
            Entry* new_head = new Entry{head->kv, nullptr};
            Entry* current_src = head;
            Entry* current_dst = new_head;
            while (current_src->next != nullptr) {
                current_dst->next = new Entry{current_src->next->kv, nullptr};
                current_src = current_src->next;
                current_dst = current_dst->next;
            }
            return new_head;
        }

        void deleteChain(Entry* head) {
            while (head != nullptr) {
                Entry* next = head->next;
                delete head;
                head = next;
            }
        }
    };

} // namespace elixir
