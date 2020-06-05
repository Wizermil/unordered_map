#pragma once

#include <assert.h>

#include <initializer_list>

#include "unordered_map/details/algorithm/fill.hpp"
#include "unordered_map/details/align_helper.hpp"
#include "unordered_map/details/bit/ceil2.hpp"
#include "unordered_map/details/config.hpp"
#include "unordered_map/details/memalign.hpp"
#include "unordered_map/details/memory/addressof.hpp"
#include "unordered_map/details/meta.hpp"
#include "unordered_map/details/min.hpp"
#include "unordered_map/details/policy_traits.hpp"
#include "unordered_map/details/type.hpp"
#include "unordered_map/details/utility/forward.hpp"
#include "unordered_map/details/utility/move.hpp"
#include "unordered_map/details/utility/swap.hpp"
#include "unordered_map/equal_to.hpp"
#include "unordered_map/hash.hpp"
#include "unordered_map/pair.hpp"

namespace wiz::linear_probing {

    template <typename K, typename T, typename Hash, typename KeyEqual, typename Policy>
    class raw_flat_hash_map {
        class _iterator;
        class _const_iterator;

    public:
        using key_type = K;
        using mapped_type = T;
        using size_type = usize;
        using value_type = pair<K, T>;
        using iterator = _iterator;
        using const_iterator = _const_iterator;
        using hasher = Hash;
        using key_equal = KeyEqual;
        using reference = value_type&;
        using const_reference = value_type const&;
        using pointer = value_type*;

    private:
        using policy = Policy;
        using storage_type = typename policy::storage_type;

        WIZ_HIDE_FROM_ABI static constexpr usize const META_ALIGN{16ul};
        WIZ_HIDE_FROM_ABI static constexpr usize const VALUE_PER_BUCKET{
            details::next_aligned(details::next_aligned(1ul, alignof(storage_type) - 1ul), META_ALIGN - 1ul)};
        WIZ_HIDE_FROM_ABI static constexpr usize const MASK{VALUE_PER_BUCKET - 1ul};
        WIZ_HIDE_FROM_ABI static constexpr usize const H2_MASK{0x7f};
        WIZ_HIDE_FROM_ABI static constexpr usize const BIT_OFFSET{__builtin_popcount(MASK)};
        WIZ_HIDE_FROM_ABI static constexpr usize const SIZEOF_VALUE{details::next_aligned(sizeof(storage_type), alignof(storage_type) - 1ul)};

        struct WIZ_HIDDEN key_or_value_hash {
            WIZ_HIDE_FROM_ABI inline size_type operator()(value_type const& v) const noexcept { return hasher{}(v.first); }
            WIZ_HIDE_FROM_ABI inline size_type operator()(key_type const& k) const noexcept { return hasher{}(k); }
        };

        struct WIZ_HIDDEN key_or_value_equal {
            WIZ_HIDE_FROM_ABI inline size_type operator()(value_type const& a, value_type const& b) const noexcept { return key_equal{}(a.first, b.first); }
            WIZ_HIDE_FROM_ABI inline size_type operator()(key_type const& a, key_type const& b) const noexcept { return key_equal{}(a, b); }
            WIZ_HIDE_FROM_ABI inline size_type operator()(key_type const& a, value_type const& b) const noexcept { return key_equal{}(a, b.first); }
        };

        struct WIZ_HIDDEN convertible_to_value {
            WIZ_HIDE_FROM_ABI operator mapped_type() const { return mapped_type{}; }
        };

        struct convertible_to_iterator {
            s8* dib;
            storage_type* value;

            operator iterator() {
                iterator it{dib, value};
                it.skip_empty_or_deleted();
                return it;
            }
            operator const_iterator() {
                iterator it{dib, value};
                it.skip_empty_or_deleted();
                return it;
            }
        };

        class WIZ_HIDDEN _iterator {
            friend class raw_flat_hash_map;

            s8* _dib;
            union {
                storage_type* _value;
            };

        public:
            WIZ_HIDE_FROM_ABI constexpr _iterator() noexcept
            : _dib{nullptr} {}

            WIZ_HIDE_FROM_ABI constexpr reference operator*() const noexcept {
                assert(_is_full());
                return policy::indirection(_value);
            }
            WIZ_HIDE_FROM_ABI constexpr pointer operator->() const noexcept { return policy::addressof(_value); }

            WIZ_HIDE_FROM_ABI constexpr iterator& operator++() noexcept {
                assert(_is_full());
                ++_dib;
                ++_value;
                skip_empty_or_deleted();
                return *this;
            }

            WIZ_HIDE_FROM_ABI constexpr iterator operator++(int) noexcept {
                auto tmp = *this;
                ++*this;
                return tmp;
            }

            WIZ_HIDE_FROM_ABI constexpr void skip_empty_or_deleted() noexcept {
                while (details::is_empty_or_deleted(*_dib)) {
                    ++_dib;
                    ++_value;
                    __builtin_prefetch(_dib + 1ul);
                }
                if (WIZ_UNLIKELY(details::is_end(*_dib))) {
                    _dib = nullptr;
                }
            }

            WIZ_HIDE_FROM_ABI friend constexpr bool operator==(_iterator const& a, _iterator const& b) noexcept {
                assert(a._is_valid() || b._is_valid());
                return a._dib == b._dib;
            }
            WIZ_HIDE_FROM_ABI friend constexpr bool operator!=(const iterator& a, const iterator& b) noexcept { return !operator==(a, b); }

        private:
            WIZ_HIDE_FROM_ABI explicit constexpr _iterator(s8* dib, storage_type* value) noexcept
            : _dib(dib)
            , _value{value} {
                __builtin_assume(dib != nullptr);
            }
            WIZ_HIDE_FROM_ABI constexpr bool _is_full() const noexcept { return _dib != nullptr && details::is_full(*_dib); }
            WIZ_HIDE_FROM_ABI constexpr bool _is_valid() const noexcept { return _dib == nullptr || details::is_full(*_dib); }
        };

        class WIZ_HIDDEN _const_iterator {
            friend class raw_flat_hash_map;

            _iterator _inner;

        public:
            WIZ_HIDE_FROM_ABI constexpr _const_iterator() noexcept = default;
            WIZ_HIDE_FROM_ABI constexpr _const_iterator(_iterator i) noexcept
            : _inner(wiz::move(i)) {}
            WIZ_HIDE_FROM_ABI constexpr _const_iterator(_const_iterator const&) noexcept = default;
            WIZ_HIDE_FROM_ABI constexpr _const_iterator(_const_iterator&&) noexcept = default;
            WIZ_HIDE_FROM_ABI constexpr _const_iterator& operator=(_const_iterator const&) noexcept = default;
            WIZ_HIDE_FROM_ABI constexpr _const_iterator& operator=(_const_iterator&&) noexcept = default;

            WIZ_HIDE_FROM_ABI constexpr reference operator*() const { return *_inner; }
            WIZ_HIDE_FROM_ABI constexpr pointer operator->() const { return _inner.operator->(); }

            WIZ_HIDE_FROM_ABI constexpr _const_iterator& operator++() {
                ++_inner;
                return *this;
            }
            WIZ_HIDE_FROM_ABI constexpr _const_iterator operator++(int) { return _inner++; }

            WIZ_HIDE_FROM_ABI friend constexpr bool operator==(_const_iterator const& a, _const_iterator const& b) noexcept { return a._inner == b._inner; }
            WIZ_HIDE_FROM_ABI friend constexpr bool operator!=(_const_iterator const& a, _const_iterator const& b) noexcept { return !(a == b); }
        };

    private:
        alignas(META_ALIGN) s8* _metas;
        storage_type* _values;
        size_type _capacity_minus_one;
        size_type _size;

    public:
        raw_flat_hash_map() noexcept
        : _metas{details::last_meta<VALUE_PER_BUCKET, META_ALIGN>()}
        , _capacity_minus_one{0ul}
        , _size{0ul} {}
        explicit raw_flat_hash_map(size_type new_cap)
        : raw_flat_hash_map() {
            rehash(new_cap);
        }
        template <typename InputIt>
        raw_flat_hash_map(InputIt first, InputIt last, size_type new_cap = 0ul)
        : raw_flat_hash_map(new_cap == 0ul ? _recommended_capacity(static_cast<usize>(last - first)) : new_cap) {
            insert(first, last);
        }
        raw_flat_hash_map(std::initializer_list<value_type> il, size_type new_cap = 0)
        : raw_flat_hash_map(new_cap == 0ul ? _recommended_capacity(il.size()) : new_cap) {
            insert(il.begin(), il.end());
        }
        raw_flat_hash_map(raw_flat_hash_map const& other)
        : raw_flat_hash_map(_recommended_capacity(other._size)) {
            insert(other.begin(), other.end());
        }
        raw_flat_hash_map(raw_flat_hash_map&& other) noexcept
        : _metas{other._metas}
        , _values{other._values}
        , _capacity_minus_one{other._capacity_minus_one}
        , _size{other._size} {
            other._metas = details::last_meta<VALUE_PER_BUCKET, META_ALIGN>();
            other._capacity_minus_one = 0ul;
            other._size = 0ul;
        }
        ~raw_flat_hash_map() { _destroy(); }

        raw_flat_hash_map& operator=(raw_flat_hash_map const& other) {
            if (WIZ_UNLIKELY(this == addressof(other))) {
                return *this;
            }
            clear();
            rehash(_recommended_capacity(other._size));
            insert(other.begin(), other.end());
            return *this;
        }
        raw_flat_hash_map& operator=(raw_flat_hash_map&& other) noexcept {
            if (WIZ_UNLIKELY(this == addressof(other))) {
                return *this;
            }
            _destroy();
            _metas = other._metas;
            _values = other._values;
            _capacity_minus_one = other._capacity_minus_one;
            _size = other._size;
            other._metas = details::last_meta<VALUE_PER_BUCKET, META_ALIGN>();
            other._capacity_minus_one = 0ul;
            other._size = 0ul;
            return *this;
        }
        raw_flat_hash_map& operator=(std::initializer_list<value_type> ilist) {
            clear();
            rehash(_recommended_capacity(ilist._size));
            insert(ilist.begin(), ilist.end());
            return *this;
        }

        // iterators

        iterator begin() noexcept {
            iterator it{_iterator_at(0)};
            it.skip_empty_or_deleted();
            return it;
        }
        iterator end() noexcept { return {}; }
        const_iterator begin() const noexcept { return const_cast<raw_flat_hash_map*>(this)->begin(); }
        const_iterator end() const noexcept { return {}; }
        const_iterator cbegin() const noexcept { return begin(); }
        const_iterator cend() const noexcept { return end(); }

        // capacity

        [[nodiscard]] bool empty() const noexcept { return _size == 0; }
        size_type size() const noexcept { return _size; }
        size_type max_size() const noexcept { return ~0ul / ((SIZEOF_VALUE + 1ul) * VALUE_PER_BUCKET); }

        // lookup

        iterator find(key_type const& key) {
            usize const hash{hasher{}(key)};
            s8 const h2{hash & H2_MASK};
            for (usize index{_index_for_hash(hash)}; !details::is_empty_or_end(*(_metas + index)); ++index) {
                if (*(_metas + index) == h2 && policy::equal(key, _values + index)) {
                    return _iterator_at(index);
                }
                __builtin_prefetch(_metas + index + 1ul);
            }
            return end();
        }
        const_iterator find(key_type const& key) const { return const_cast<raw_flat_hash_map*>(this)->find(key); }
        size_type count(key_type const& key) const { return find(key) == end() ? 0 : 1; }
        mapped_type& operator[](key_type const& key) { return _emplace(key, convertible_to_value{}).first->second; }
        mapped_type& operator[](key_type&& key) { return _emplace(wiz::move(key), convertible_to_value{}).first->second; }
        mapped_type& at(key_type const& key) {
            iterator found = find(key);
            assert(found != end()); // Argument passed to at() was not in the map
            return found->second;
        }
        mapped_type const& at(key_type const& key) const {
            const_iterator found = find(key);
            assert(found != end()); // Argument passed to at() was not in the map
            return found->second;
        }
        bool contains(key_type const& key) const { return find(key) == end() ? false : true; }
        wiz::pair<iterator, iterator> equal_range(key_type const& key) {
            iterator found = find(key);
            if (found == end()) {
                return {found, found};
            } else {
                return {found, ++found};
            }
        }
        wiz::pair<const_iterator, const_iterator> equal_range(key_type const& key) const {
            const_iterator found = find(key);
            if (found == end()) {
                return {found, found};
            } else {
                return {found, ++found};
            }
        }

        // modifiers

        void clear() {
            for (usize index{0}, last{(_capacity_minus_one + 1ul) << BIT_OFFSET}; index < last; ++index) {
                if (details::is_full(*(_metas + index))) {
                    _destroy_at(index);
                }
            }
            _size = 0ul;
        }

        pair<iterator, bool> insert(value_type const& value) { return _emplace(value); }
        pair<iterator, bool> insert(value_type&& value) { return _emplace(wiz::move(value)); }
        iterator insert(const_iterator, value_type const& value) { return _emplace(value).first; }
        iterator insert(const_iterator, value_type&& value) { return _emplace(wiz::move(value)).first; }
        template <typename It>
        void insert(It begin, It end) {
            for (; begin != end; ++begin) {
                _emplace(*begin);
            }
        }
        void insert(std::initializer_list<value_type> il) { insert(il.begin(), il.end()); }

        template <typename M>
        pair<iterator, bool> insert_or_assign(key_type const& key, M&& m) {
            pair<iterator, bool> emplace_result = _emplace(key, wiz::forward<M>(m));
            if (!emplace_result.second) {
                emplace_result.first->second = wiz::forward<M>(m);
            }
            return emplace_result;
        }
        template <typename M>
        pair<iterator, bool> insert_or_assign(key_type&& key, M&& m) {
            pair<iterator, bool> emplace_result = _emplace(wiz::move(key), wiz::forward<M>(m));
            if (!emplace_result.second) {
                emplace_result.first->second = wiz::forward<M>(m);
            }
            return emplace_result;
        }
        template <typename M>
        iterator insert_or_assign(const_iterator, key_type const& key, M&& m) {
            return insert_or_assign(key, wiz::forward<M>(m)).first;
        }
        template <typename M>
        iterator insert_or_assign(const_iterator, key_type&& key, M&& m) {
            return insert_or_assign(wiz::move(key), wiz::forward<M>(m)).first;
        }

        template <typename... Args>
        pair<iterator, bool> emplace(Args&&... args) {
            return _emplace(wiz::forward<Args>(args)...);
        }
        template <typename... Args>
        iterator emplace_hint(const_iterator, Args&&... args) {
            return _emplace(wiz::forward<Args>(args)...).first;
        }

        template <typename... Args>
        pair<iterator, bool> try_emplace(key_type const& k, Args&&... args) {
            return _emplace(k, wiz::forward<Args>(args)...);
        }
        template <typename... Args>
        pair<iterator, bool> try_emplace(key_type&& k, Args&&... args) {
            return _emplace(wiz::move(k), wiz::forward<Args>(args)...);
        }
        template <typename... Args>
        iterator try_emplace(const_iterator, key_type const& k, Args&&... args) {
            return _emplace(wiz::move(k), wiz::forward<Args>(args)...);
        }
        template <class... Args>
        iterator try_emplace(const_iterator, key_type&& k, Args&&... args) {
            return _emplace(wiz::move(k), wiz::forward<Args>(args)...);
        }

        convertible_to_iterator erase(const_iterator pos) {
            assert(pos != end());
            usize current{static_cast<usize>(pos._inner._dib - _metas)};
            _destroy_at(current);
            --_size;
            return convertible_to_iterator{pos._inner._dib, pos._inner._value};
        }
        convertible_to_iterator erase(const_iterator first, const_iterator last) {
            if (WIZ_UNLIKELY(first == last)) {
                return convertible_to_iterator{first._inner._dib, first._inner._value};
            }
            usize ind_end{last._inner._dib == nullptr ? (_capacity_minus_one + 1ul) << BIT_OFFSET : static_cast<usize>(last._inner._dib - _metas)};
            usize current{first._inner._dib == nullptr ? (_capacity_minus_one + 1ul) << BIT_OFFSET : static_cast<usize>(first._inner._dib - _metas)};
            assert(current <= ind_end);
            for (; current != ind_end; ++current) {
                if (details::is_full(*(_metas + current))) {
                    _destroy_at(current);
                    --_size;
                }
            }
            return convertible_to_iterator{_metas + current, _values + current};
        }
        size_type erase(key_type const& key) {
            iterator found = find(key);
            if (found == end()) {
                return 0ul;
            } else {
                erase(found);
                return 1ul;
            }
        }

        void swap(raw_flat_hash_map& other) {
            wiz::swap(_metas, other._metas);
            wiz::swap(_values, other._values);
            wiz::swap(_capacity_minus_one, other._capacity_minus_one);
            wiz::swap(_size, other._size);
        }

        // bucket interface

        constexpr size_type bucket_count() const {
            if (WIZ_LIKELY(_is_init())) {
                return _capacity_minus_one + 1ul;
            } else {
                return 0ul;
            }
        }

        size_type max_bucket_count() const { return (~0ul - (VALUE_PER_BUCKET << 1ul)) / SIZEOF_VALUE; }

        float load_factor() const {
            if (WIZ_LIKELY(_is_init())) {
                return static_cast<float>(_size) / (_capacity_minus_one + 1ul);
            } else {
                return 0.f;
            }
        }

        // hash policy

        void reserve(size_type new_cap) { rehash(new_cap); }
        void rehash(size_type new_cap) {
            if (new_cap > bucket_count()) {
                _grow(details::bit::ceil2(new_cap));
            }
        }

        friend bool operator==(raw_flat_hash_map const& lhs, raw_flat_hash_map const& rhs) {
            if (lhs.size() != rhs.size()) {
                return false;
            }
            for (auto const& vlhs : lhs) {
                auto found = rhs.find(vlhs.first);
                if (found == rhs.end()) {
                    return false;
                } else if (vlhs.second != found->second) {
                    return false;
                }
            }
            return true;
        }
        friend bool operator!=(const raw_flat_hash_map& lhs, const raw_flat_hash_map& rhs) { return !(lhs == rhs); }

    private:
        WIZ_HIDE_FROM_ABI constexpr iterator _iterator_at(usize index) const noexcept { return iterator{_metas + index, _values + index}; }

        WIZ_HIDE_FROM_ABI inline void _destroy_at(usize index) const noexcept {
            assert(details::is_full(*(_metas + index)));
            *(_metas + index) = details::flag::DELETED;
            policy::destruct_in_place(_values + index);
        }

        template <typename Key, typename... Args>
        WIZ_HIDE_FROM_ABI inline void _placement_new_at(usize index, s8 h2, Key&& key, Args&&... args) const noexcept {
            policy::construct_in_place(_values + index, wiz::forward<Key>(key), wiz::forward<Args>(args)...);
            *(_metas + index) = h2;
        }

        WIZ_HIDE_FROM_ABI constexpr usize _recommended_capacity(usize size) const noexcept { return (size + MASK) >> BIT_OFFSET; }

        WIZ_HIDE_FROM_ABI constexpr usize _index_for_hash(usize hash) const noexcept {
            usize const bucket_num{(hash >> BIT_OFFSET) & _capacity_minus_one};
            return (bucket_num << BIT_OFFSET) + (hash & MASK);
            //            usize const bucket_num{hash & _capacity_minus_one};
            //            return (bucket_num << BIT_OFFSET) + static_cast<usize>((static_cast<u64>(hash) * 11400714819323198485ull) >> 60ull);
        }

        WIZ_HIDE_FROM_ABI inline bool _is_init() const noexcept { return _metas != details::last_meta<VALUE_PER_BUCKET, META_ALIGN>(); }

        template <typename Key, typename... Args>
        WIZ_HIDE_FROM_ABI WIZ_NOINLINE pair<iterator, bool> _emplace(Key&& key, Args&&... args) {
            usize const hash{policy::hash(key)};
            s8 const h2{hash & H2_MASK};
            usize index{_index_for_hash(hash)};
            s8 distance{0};
            for (; !details::is_empty_or_end(*(_metas + index)); ++distance, ++index) {
                if (*(_metas + index) == h2 && policy::equal(key, _values + index)) {
                    return {_iterator_at(index), false};
                }
                __builtin_prefetch(_metas + index + 1ul);
            }

            return _emplace_new(distance, h2, index, wiz::forward<Key>(key), wiz::forward<Args>(args)...);
        }

        template <typename Key, typename... Args>
        WIZ_HIDE_FROM_ABI pair<iterator, bool> _emplace_new(s8 distance_from_desired, s8 h2, usize index, Key&& key, Args&&... args) noexcept {
            if (WIZ_LIKELY(details::is_empty_or_deleted(*(_metas + index)))) {
                _placement_new_at(index, h2, wiz::forward<Key>(key), wiz::forward<Args>(args)...);
                ++_size;
                return {_iterator_at(index), true};
            } else if (details::is_end(*(_metas + index)) || distance_from_desired == (VALUE_PER_BUCKET << 1ul)) {
                if (WIZ_LIKELY(_is_init())) {
                    _grow((_capacity_minus_one + 1ul) << 1ul);
                } else {
                    _grow(1ul);
                }
                return _emplace(wiz::forward<Key>(key), wiz::forward<Args>(args)...);
            }
            ++distance_from_desired;
            ++index;
            while (true) {
                if (WIZ_LIKELY(details::is_empty_or_deleted(*(_metas + index)))) {
                    _placement_new_at(index, h2, wiz::forward<Key>(key), wiz::forward<Args>(args)...);
                    ++_size;
                    return {_iterator_at(index), true};
                } else if (details::is_end(*(_metas + index)) || distance_from_desired == (VALUE_PER_BUCKET << 1ul)) {
                    if (WIZ_LIKELY(_is_init())) {
                        _grow((_capacity_minus_one + 1ul) << 1ul);
                    } else {
                        _grow(1ul);
                    }
                    return _emplace(wiz::forward<Key>(key), wiz::forward<Args>(args)...);
                }
                __builtin_prefetch(_metas + index + 1ul);
                ++distance_from_desired;
                ++index;
            }
            return {end(), false};
        }

        WIZ_HIDE_FROM_ABI inline void _grow(size_type new_cap) {
            assert(new_cap > 0ul || (new_cap & (new_cap - 1ul)) != 0ul);
            size_type new_cap_minus_one{new_cap - 1ul};
            new_cap = (new_cap << BIT_OFFSET);
            s8* metas = static_cast<s8*>(details::memalign((SIZEOF_VALUE + 1ul) * new_cap + VALUE_PER_BUCKET, META_ALIGN));
            storage_type* values{reinterpret_cast<storage_type*>(metas + new_cap + VALUE_PER_BUCKET)};
            details::fill(metas, metas + new_cap, details::flag::EMPTY);
            details::fill(metas + new_cap, metas + new_cap + VALUE_PER_BUCKET, details::flag::END);
            bool const is_init{_is_init()};
            wiz::swap(_metas, metas);
            wiz::swap(_values, values);
            wiz::swap(_capacity_minus_one, new_cap_minus_one);
            _size = 0ul;
            if (WIZ_LIKELY(is_init)) {
                for (usize index{0ul}, last{(new_cap_minus_one + 1ul) << BIT_OFFSET}; index < last; ++index) {
                    if (details::is_full(*(metas + index))) {
                        _emplace(wiz::move(*(values + index)));

                        *(metas + index) = details::flag::DELETED;
                        if constexpr (!policy::is_node) {
                            policy::destruct_in_place(values + index);
                        }
                    }
                    __builtin_prefetch(metas + index + 1ul);
                }
                ::free(metas);
            }
        }

        WIZ_HIDE_FROM_ABI inline void _destroy() {
            if (WIZ_LIKELY(_is_init())) {
                for (usize index{0}, last{(_capacity_minus_one + 1ul) << BIT_OFFSET}; index < last; ++index) {
                    if (details::is_full(*(_metas + index))) {
                        _destroy_at(index);
                    }
                    __builtin_prefetch(_metas + index + 1ul);
                }
                ::free(_metas);
            }
        }
    };

    template <typename K, typename T, typename Hash = wiz::hash<K>, typename KeyEqual = wiz::equal_to<K>>
    using node_hash_map = raw_flat_hash_map<K, T, Hash, KeyEqual, details::node_policy<K, T, Hash, KeyEqual>>;

    template <typename K, typename T, typename Hash = wiz::hash<K>, typename KeyEqual = wiz::equal_to<K>>
    using flat_hash_map = raw_flat_hash_map<K, T, Hash, KeyEqual, details::policy<K, T, Hash, KeyEqual>>;

} // namespace wiz::linear_probing
