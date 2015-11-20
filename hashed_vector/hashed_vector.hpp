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

    explicit hashed_vector() {}
    explicit hashed_vector(size_t n);
    hashed_vector(size_t n, const T& val);
    template <class InputIterator> hashed_vector(InputIterator first, InputIterator last);
    hashed_vector(const hashed_vector& hv);
    hashed_vector(hashed_vector&& hv);
    hashed_vector(std::initializer_list<T> il);

    ~hashed_vector();

    hashed_vector& operator=(hashed_vector hv);
    hashed_vector& operator=(hashed_vector&& hv);
    hashed_vector& operator=(std::initializer_list<T> hv);

    size_t size() const;
    size_t max_size() const;
    void resize(size_t n);
    void resize(size_t n, const T& val);
    size_t capacity() const;
    bool empty() const;
    void reserve(size_t n);
    void shrink_to_fit();

    T& operator[](size_t n);
    const T& operator[](size_t n) const;
    T& at(size_t n);
    const T& at(size_t n) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    friend void swap(hashed_vector<T, Hash> &one, hashed_vector<T, Hash> &two);

private:
    static const uint32_t HASH_MOD = 113;

    typedef std::list<std::pair<T&,uint32_t>> hash_elem;
    typedef std::pair<uint32_t, uint32_t> change;

    void insert_hash_elem(T& val, uint32_t pos);
    void remove_hash_elem(T& val, uint32_t pos);

    std::vector<T> vector_;
    std::vector<change> changes_;
    hash_elem hash_table_[HASH_MOD];
};


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::hashed_vector(size_t n) : vector_(n) {}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::hashed_vector(size_t n, const T& val) : vector_(n, val) {}


template <typename T, int32_t (*Hash)(T&), InputIterator>
hashed_vector<T, Hash>::hashed_vector(InputIterator first, InputIterator last) :
    vector_(first, last) {
    for (uint32_t i = 0; i < vector_.size(); ++i) {
        insert_hash(vector_[i], i);
    }
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::hashed_vector(const hashed_vector& hv) : vector_(hv.vector_), changes_(hv.changes_) {
    for (uint32_t i = 0; i < HASH_MOD; ++i) {
        hash_table_[i](hv.hash_table_[i]);
    }
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::hashed_vector(hashed_vector&& hv) : vector_(std::move(hv.vector_)),
    changes_(std::move(hv.changes_)) {
    for (uint32_t i = 0; i < HASH_MOD; ++i) {
        hash_table_[i](std::move(hv.hash_table_[i]));
        hv.hash_table_[i] = nullptr;
    }
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::hashed_vector(std::initializer_list<T> il) : vector_(il) {
    for (uint32_t i = 0; i < vector_.size(); ++i) {
        insert_hash(vector_[i], i);
    }
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::~hashed_vector() {}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector& hashed_vector<T, Hash>::operator=(hashed_vector hv) {
    swap(this, hv);
    return *this;
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector& hashed_vector<T, Hash>::operator=(hashed_vector&& hv) {
    vector_ = std::move(hv.vector_);
    changes_ = std::move(hv.changes_);
    for (size_t i = 0; i < HASH_MOD; ++i) {
        hash_table_[i] = std::move(hv.hash_table_[i]);
    }
    return *this;
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector& hashed_vector<T, Hash>::operator=(std::initializer_list<T> il) {
    vector_ = il;
    std::for_each(hash_table_, hash_table_ + HASH_MOD, [](hash_elem &elem) {elem.erase(elem.cbegin(), elem.cend())});
    changes_.erase(changes_.begin(), changes_.end());
    for (uint32_t i = 0; i < vector_.size(); ++i) {
        insert_hash(vector_[i], i);
    }
    return *this;
}


template <typename T, int32_t (*Hash)(T&)>
size_t hashed_vector<T, Hash>::size() const {
    return vector_.size();
}


template <typename T, int32_t (*Hash)(T&)>
size_t hashed_vector<T, Hash>::max_size() const {
    return vector_.max_size();
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::resize(size_t n) {
    if (n < vector_.size()) {
        auto it = vector_.cbegin();
        std::advance(it + n);
        for (; it != vector_.cend(); ++it) {
            remove_hash_elem(*it, std::distance(vector_.cbegin(), it));
        }
        vector_.resize(n);
    } else if (n > vector_.size()) {
        size_t index = vector_.size();
        vector_.resize(n);
        auto it = vector_.cbegin();
        std::advance(it + index);
        for (; it != vector_.cend(); ++it) {
            insert_hash_elem(*it, std::distance(vector_.cbegin(), it));
        }
    }
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::resize(size_t n, const T &val) {
    if (n < vector_.size()) {
        auto it = vector_.cbegin();
        std::advance(it + n);
        for (; it != vector_.cend(); ++it) {
            remove_hash_elem(*it, std::distance(vector_.cbegin(), it));
        }
        vector_.resize(n);
    } else if (n > vector_.size()) {
        size_t index = vector_.size();
        vector_.resize(n, val);
        auto it = vector_.cbegin();
        std::advance(it + index);
        for (; it != vector_.cend(); ++it) {
            insert_hash_elem(*it, std::distance(vector_.cbegin(), it));
        }
    }
}


template <typename T, int32_t (*Hash)(T&)>
size_t hashed_vector<T, Hash>::capacity() const {
    return vector_.capacity();
}


template <typename T, int32_t (*Hash)(T&)>
bool hashed_vector<T, Hash>::empty() {
    return vector_.empty();
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::reserve(size_t n) {
    return vector_.reserve(n);
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::shrink_to_fit() {
    return vector_.shrink_to_fit();
}


template <typename T, int32_t (*Hash)(T&)>
T& hashed_vector<T, Hash>::operator[](size_t n) {
    changes_.push_back(change(Hash(vector_[i] % HASH_MOD, i)));
    return vector_[n];
}


template <typename T, int32_t (*Hash)(T&)>
const T& hashed_vector<T, Hash>::operator[](size_t n) const {
    return vector_[n];
}


template <typename T, int32_t (*Hash)(T&)>
T& hashed_vector<T, Hash>::at(size_t n) {
    changes_.push_back(change(Hash(vector_[i] % HASH_MOD, i)));
    return vector_.at(n);
}


template <typename T, int32_t (*Hash)(T&)>
const T& hashed_vector<T, Hash>::at(size_t n) const {
    return vector_.at(n);
}


template <typename T, int32_t (*Hash)(T&)>
T& hashed_vector<T, Hash>::front() {
    if (!vector_.empty()) {
        changes_.push_back(change(Hash(vector_[0] % HASH_MOD, 0)));
    }
    return vector_.front();
}


template <typename T, int32_t (*Hash)(T&)>
const T& hashed_vector<T, Hash>::front() const {
    return vector_.front();
}


template <typename T, int32_t (*Hash)(T&)>
T& hashed_vector<T, Hash>::back() {
    if (!vector_.empty()) {
        size_t index = vector_.size() - 1;
        changes_.push_back(change(Hash(vector_[index] % HASH_MOD, index)));
    }
    return vector_.back();
}


template <typename T, int32_t (*Hash)(T&)>
const T& hashed_vector<T, Hash>::back() const {
    return vector_.back();
}


void swap(hashed_vector<T, Hash> &one, hashed_vector<T, Hash> &two) {
    std::swap(one.changes_, two.changes_);
    std::swap(one.vector_, two.vector_);
    std::swap(one.hash_table_, two.hash_table_);
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T,Hash>::insert_hash_elem(T &val, uint32_t pos) {
    uint32_t index = Hash(val) % HASH_MOD;
    hash_table_[index].push_back(std::pair(val, pos));
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T,Hash>::remove_hash_elem(T &val, uint32_t pos) {
    uint32_t index = Hash(val) % HASH_MOD;
    hash_elem &elem = hash_table_[index];
    for (auto it = elem.begin(); it != elem.end(); ++it ) {
        if ((*it).second == pos) {
            assert((*it).first == val);
            elem.erase(it);
            break;
        }
    }
}


#endif //OBJC_HASHED_VECTOR_HPP_
