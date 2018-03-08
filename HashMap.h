#pragma once

#include <list>
#include <vector>
#include <cstddef>
#include <functional>
#include <stdexcept>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
public:
    typedef typename std::list<std::pair<const KeyType, ValueType>>::iterator iterator;
    typedef typename std::list<std::pair<const KeyType, ValueType>>::const_iterator const_iterator;

    HashMap(Hash h = Hash()) : elements_size(0), hasher(h), elements(), table(1, std::make_pair(elements.end(), 0)) {};

    template<class Iterator>
    HashMap(Iterator beginIt, Iterator endIt, Hash h = Hash()) : HashMap(h) {
        size_t sz = std::distance(beginIt, endIt);
        if (sz > 0) {
            rebuild(2 * sz);
            while (beginIt != endIt) {
                insert(*beginIt);
                ++beginIt;
            }
        }
    }

    HashMap(const HashMap &b) : HashMap(b.begin(), b.end(), b.hash_function()) {}

    HashMap(HashMap &&b) {
        std::swap(elements_size, b.elements_size);
        std::swap(hasher, b.hasher);
        std::swap(elements, b.elements);
        std::swap(table, b.table);
    }

    HashMap(typename std::initializer_list<std::pair<KeyType, ValueType>> l, Hash h = Hash()) : HashMap(l.begin(), l.end(), h) {};

    HashMap &operator=(HashMap b) {
        std::swap(elements_size, b.elements_size);
        std::swap(hasher, b.hasher);
        std::swap(elements, b.elements);
        std::swap(table, b.table);
        return *this;
    }

    size_t size() const {
        return elements_size;
    }

    bool empty() const {
        return elements.empty();
    }

    Hash hash_function() const {
        return hasher;
    }

    std::pair<iterator, bool> insert(const std::pair<KeyType, ValueType> &p) {
        const KeyType &key = p.first;
        auto it = find(key);
        if (it == elements.end()) {
            if (2 * size() + 1 >= table.size()) {
                rebuild(table.size() * 2);
            }
            auto hs = hasher(key) % table.size();
            ++table[hs].second;
            ++elements_size;
            table[hs].first = elements.insert(table[hs].first, p);
            return std::make_pair(table[hs].first, 1);
        }
        return std::make_pair(it, 0);
    }

    void erase(const KeyType &key) {
        auto it = find(key);
        if (it != elements.end()) {
            auto hs = hasher(key) % table.size();
            --table[hs].second;
            --elements_size;
            if (table[hs].first == it) {
                if (table[hs].second == 0) {
                    table[hs].first = elements.end();
                } else {
                    ++table[hs].first;
                }
            }
            elements.erase(it);
        }
    }

    iterator begin() {
        return elements.begin();
    }

    iterator end() {
        return elements.end();
    }

    const_iterator begin() const {
        return elements.cbegin();
    }

    const_iterator end() const {
        return elements.cend();
    }

    iterator find(const KeyType &key) {
        auto res = elements.end();
        auto hs = hasher(key) % table.size();
        auto it = table[hs].first;
        for (size_t i = 0; i < table[hs].second; ++i, ++it) {
            if (key == it->first) {
                res = it;
                break;
            }
        }
        return res;
    }

    const_iterator find(const KeyType &key) const {
        auto res = elements.end();
        auto hs = hasher(key) % table.size();
        auto it = table[hs].first;
        for (size_t i = 0; i < table[hs].second; ++i, ++it) {
            if (key == it->first) {
                res = it;
                break;
            }
        }
        return res;
    }

    ValueType &operator[](const KeyType &key) {
        auto it = find(key);
        if (it == elements.end()) {
            it = insert(std::make_pair(key, ValueType())).first;
        }
        return it->second;
    }

    const ValueType &at(const KeyType &key) const {
        auto it = find(key);
        if (it == elements.end()) {
            throw std::out_of_range("Key not found");
        }
        return it->second;
    }

    void clear() {
        elements.clear();
        elements_size = 0;
        table.assign(1, std::make_pair(elements.end(), 0));
    } 

private:
    void rebuild(size_t sz) {
        decltype(elements) old;
        swap(old, elements);
        elements_size = 0;
        table.assign(sz, make_pair(elements.end(), 0));
        while (!old.empty()) {
            insert(old.front());
            old.pop_front();
        }
    }

    size_t elements_size;
    Hash hasher;
    std::list<std::pair<const KeyType, ValueType>> elements;
    std::vector<std::pair<typename decltype(elements)::iterator, size_t>> table;
};
