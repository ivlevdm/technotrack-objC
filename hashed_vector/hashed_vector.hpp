#ifndef OBJC_HASHED_VECTOR_HPP_
#define OBJC_HASHED_VECTOR_HPP_

#include <vector>
#include <list>
#include <functional>

template <typename T, int32_t (*Hash)(T&)>
class hashed_vector {
public:
    class iterator<T> {

    };

    explicit hashed_vector() {};
    explicit hashed_vector(size_t n);
    hashed_vector(size_t n, const T& val);
    template <class InputIterator> hashed_vector(InputIterator first, InputIterator last);
    hashed_vector(const hashed_vector& hv);
    hashed_vector(hashed_vector&& hv);
    hashed_vector(std::initializer_list<T> il);

    ~hashed_vector() {};

    hashed_vector& operator=(const hashed_vector& hv);
    hashed_vector& operator=(hashed_vector&& hv);
    hashed_vector& operator=(std::initializer_list<T> hv);



    friend void swap(hashed_vector<T, Hash> &one, hashed_vector<T, Hash> &two);

private:
    static const uint32_t HASH_MOD = 113;

    void insert_hash(T& val, uint32_t pos);

    std::vector<T> vector_;
    std::list<std::pair<T&,uint32_t>> hash_table_[HASH_MOD];
};


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T,Hash>::hashed_vector(size_t n) : vector_(n) {}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T,Hash>::hashed_vector(size_t n, const T& val) : vector_(n, val) {}


template <typename T, int32_t (*Hash)(T&), InputIterator>
hashed_vector<T,Hash>::hashed_vector(InputIterator first, InputIterator last) : vector_(first, last) {
    for (uint32_t i = 0; i < vector_.size(); ++i) {
        insert_hash(vector_[i], i);
    }
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T,Hash>::hashed_vector(const hashed_vector& hv) : vector_(hv.vector_) {
    for (uint32_t i = 0; i < HASH_MOD; ++i) {
        hash_table_[i](hv.hash_table_[i]);
    }
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T,Hash>::hashed_vector(hashed_vector&& hv) : vector_(hv.vector_) {
    for (uint32_t i = 0; i < HASH_MOD; ++i) {
        hash_table_[i](hv.hash_table_[i]);
        hv.hash_table_[i] = nullptr;
    }
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T,Hash>::hashed_vector(std::initializer_list<T> il) : vector_(il) {
    for (uint32_t i = 0; i < vector_.size(); ++i) {
        insert_hash(vector_[i], i);
    }
}


void swap(hashed_vector<T, Hash> &one, hashed_vector<T, Hash> &two) {
    std::swap(one.vector_, two.vector_);
    std::swap(one.hash_table_, two.hash_table_);
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T,Hash>::insert_hash(T &val, uint32_t pos) {
    uint32_t index = Hash(val) % HASH_MOD;
    hash_table_[index].push_back(std::pair(val, pos));
}
#endif //OBJC_HASHED_VECTOR_HPP_