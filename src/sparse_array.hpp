#pragma once

#include <vector>
#include <optional>
#include <iostream>

template<typename Type, typename Alloc = std::allocator<Type>>
class sparse_array {
    public:
        using value_type = std::optional<Type>;
        using reference_type = value_type&;
        using const_reference_type = value_type const &;
        using container_t = std::vector<value_type, Alloc>;
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

    private:
        container_t _data;

    public:
        sparse_array() : _data(container_t()) {};
        ~sparse_array() {};
        sparse_array(sparse_array const &other) : _data(other._data) {}; //copy constructor
        sparse_array(sparse_array && other) noexcept : _data(std::move(other._data)) {}; //move constructor
        sparse_array &operator =(sparse_array const &other) { //copy assignment operator
            _data = other._data;
            return *this;
        };
        sparse_array &operator =(sparse_array &&other) noexcept { // move assignment operator
            _data = std::move(other._data);
            return *this;
        };

        reference_type operator [](std::size_t idx) {
            return _data.at(idx);
        };

        const_reference_type operator [] (std::size_t idx) const {
            return _data.at(idx);
        };

        iterator begin () { return _data.begin(); };
        const_iterator begin () const { return _data.begin(); };
        const_iterator cbegin () const { return _data.cbegin(); };
        iterator end () { return _data.end(); };
        const_iterator end () const { return _data.end(); };
        const_iterator cend () const { return _data.cend(); };
        size_type size () const { return _data.size(); };
        void resize (size_type len) { _data.resize(len, std::nullopt); };

        reference_type insert_at(size_type pos, Type const &val) {
            if (pos > _data.size())
                _data.resize(pos + 1, std::nullopt);
            _data[pos] = std::optional(val);
            return _data[pos];
        }

        reference_type insert_at(size_type pos, Type &&val) {
            if (pos > _data.size())
                _data.resize(pos + 1, std::nullopt);
            _data[pos] = std::optional(val);
            return _data[pos];
        }

        template <class ... Params>
        reference_type emplace_at(size_type pos, Params &&... args) {
            insert_at(pos, std::allocator_traits<Alloc>::construct(args...));
        }

        void erase(size_type pos) {
            _data[pos] = std::nullopt;
        }

        reference_type insert(Type &&val) {
            for (auto& element : _data) {
                if (!element) {
                    element = std::optional(val);
                    return element;
                }
            }
            _data.push_back(std::optional(val));
            return _data.back();
        }

        std::optional<size_type> get_index(value_type const &elem) const {
            value_type const *ptr = std::addressof(elem);

            for (size_type i = 0; i != _data.size(); i++) {
                if (ptr == std::addressof(_data[i])) {
                    return i;
                }
            }
            return std::nullopt;
        }
};
