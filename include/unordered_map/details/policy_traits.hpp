#pragma once

#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"
#include "unordered_map/details/utility/forward.hpp"
#include "unordered_map/pair.hpp"

namespace wiz::details {

    template <typename K, typename T, typename Hash, typename KeyEqual>
    struct WIZ_HIDDEN node_policy {
        using key_type = K;
        using value_type = pair<K, T>;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using storage_type = pointer;
        using reference = value_type&;
        using const_reference = value_type const&;
        using hasher = Hash;
        using key_equal = KeyEqual;

        static constexpr bool const is_node = true;

        WIZ_HIDE_FROM_ABI static inline reference indirection(storage_type* a) noexcept {
            return **a;
        }
        WIZ_HIDE_FROM_ABI static inline pointer addressof(storage_type* a) noexcept {
            return *a;
        }

        template <typename Key, typename... Args>
        WIZ_HIDE_FROM_ABI static inline storage_type construct(Key&& key, Args&&... args) noexcept {
            return new value_type{wiz::forward<Key>(key), wiz::forward<Args>(args)...};
        }
        WIZ_HIDE_FROM_ABI static inline storage_type construct(storage_type val) noexcept {
            return val;
        }
        template <typename Key, typename... Args>
        WIZ_HIDE_FROM_ABI static inline void construct_in_place(storage_type* ptr, Key&& key, Args&&... args) noexcept {
            *ptr = new value_type{wiz::forward<Key>(key), wiz::forward<Args>(args)...};
        }
        WIZ_HIDE_FROM_ABI static inline void construct_in_place(storage_type* ptr, storage_type val) noexcept {
            *ptr = val;
        }
        WIZ_HIDE_FROM_ABI static inline void destruct(storage_type* ptr) noexcept {
            delete *ptr;
        }
        WIZ_HIDE_FROM_ABI static inline void destruct_in_place(storage_type* ptr) noexcept {
            delete *ptr;
        }

        WIZ_HIDE_FROM_ABI static inline usize hash(const_reference val) noexcept {
            return hasher{}(val.first);
        }
        WIZ_HIDE_FROM_ABI static inline usize hash(storage_type ptr) noexcept {
            return hasher{}(ptr->first);
        }
        WIZ_HIDE_FROM_ABI static inline usize hash(key_type const& key) noexcept {
            return hasher{}(key);
        }

        WIZ_HIDE_FROM_ABI static inline bool equal(const_reference a, storage_type b) noexcept {
            return key_equal{}(a.first, b->first);
        }
        WIZ_HIDE_FROM_ABI static inline bool equal(const_reference a, storage_type* b) noexcept {
            return key_equal{}(a.first, (*b)->first);
        }
        WIZ_HIDE_FROM_ABI static inline bool equal(const_pointer a, storage_type b) noexcept {
            return key_equal{}(a->first, b->first);
        }
        WIZ_HIDE_FROM_ABI static inline bool equal(const_pointer a, storage_type* b) noexcept {
            return key_equal{}(a->first, (*b)->first);
        }
        WIZ_HIDE_FROM_ABI static inline bool equal(key_type const& a, storage_type b) noexcept {
            return key_equal{}(a, b->first);
        }
        WIZ_HIDE_FROM_ABI static inline bool equal(key_type const& a, storage_type* b) noexcept {
            return key_equal{}(a, (*b)->first);
        }
    };

    template <typename K, typename T, typename Hash, typename KeyEqual>
    struct WIZ_HIDDEN policy {
        using key_type = K;
        using value_type = pair<K, T>;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using storage_type = value_type;
        using reference = value_type&;
        using const_reference = value_type const&;
        using hasher = Hash;
        using key_equal = KeyEqual;

        static constexpr bool const is_node = false;

        WIZ_HIDE_FROM_ABI static inline reference indirection(storage_type* a) noexcept {
            return *a;
        }
        WIZ_HIDE_FROM_ABI static inline pointer addressof(storage_type* a) noexcept {
            return a;
        }

        template <typename Key, typename... Args>
        WIZ_HIDE_FROM_ABI static inline storage_type construct(Key&& key, Args&&... args) noexcept {
            return value_type{wiz::forward<Key>(key), wiz::forward<Args>(args)...};
        }
        template <typename Key, typename... Args>
        WIZ_HIDE_FROM_ABI static inline void construct_in_place(storage_type* ptr, Key&& key, Args&&... args) noexcept {
            new (ptr) value_type{wiz::forward<Key>(key), wiz::forward<Args>(args)...};
        }

        WIZ_HIDE_FROM_ABI static inline void destruct(storage_type* ptr) noexcept {
            ptr->~value_type();
        }
        WIZ_HIDE_FROM_ABI static inline void destruct_in_place(storage_type* ptr) noexcept {
            ptr->~value_type();
        }

        WIZ_HIDE_FROM_ABI static inline usize hash(const_reference val) noexcept {
            return hasher{}(val.first);
        }
        WIZ_HIDE_FROM_ABI static inline usize hash(key_type const& key) noexcept {
            return hasher{}(key);
        }

        WIZ_HIDE_FROM_ABI static inline bool equal(const_reference a, const_reference b) noexcept {
            return key_equal{}(a.first, b.first);
        }
        WIZ_HIDE_FROM_ABI static inline bool equal(const_reference a, const_pointer b) noexcept {
            return key_equal{}(a.first, b->first);
        }
        WIZ_HIDE_FROM_ABI static inline bool equal(key_type const& a, const_pointer b) noexcept {
            return key_equal{}(a, b->first);
        }
        WIZ_HIDE_FROM_ABI static inline bool equal(key_type const& a, const_reference b) noexcept {
            return key_equal{}(a, b.first);
        }
    };


}
