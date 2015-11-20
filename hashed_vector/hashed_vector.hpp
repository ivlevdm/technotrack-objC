#ifndef OBJC_HASHED_VECTOR_HPP_
#define OBJC_HASHED_VECTOR_HPP_

#include <vector>
#include <list>
#include <functional>
#include <initializer_list>
#include <algorithm>


template  <typename T, int32_t (*Hash)(T&)> class hashed_vector;

template <typename T,int32_t (*Hash)(T&)>
void swap(hashed_vector<T, Hash>& one, hashed_vector<T, Hash>& two);


template <typename T, int32_t (*Hash)(T&)>
class hashed_vector {
public:
    class iterator
    {
    public:
        typedef iterator self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::random_access_iterator_tag  iterator_category;

        iterator(pointer ptr) : ptr_(ptr) { }
        self_type operator++() { self_type i = *this; ptr_++; return i; }
        self_type operator++(int junk) { ptr_++; return *this; }
        reference operator*() { return *ptr_; }
        pointer operator->() { return ptr_; }
        bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
    private:
        pointer ptr_;
    };

    class const_iterator
    {
    public:
        typedef const_iterator self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::random_access_iterator_tag iterator_category;

        const_iterator(pointer ptr) : ptr_(ptr) { }
        self_type operator++() { self_type i = *this; ptr_++; return i; }
        self_type operator++(int junk) { ptr_++; return *this; }
        const reference operator*() { return *ptr_; }
        const pointer operator->() { return ptr_; }
        bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
    private:
        pointer ptr_;
    };

    class reverse_iterator
    {
    public:
        typedef reverse_iterator self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::random_access_iterator_tag  iterator_category;

        reverse_iterator(pointer ptr) : ptr_(ptr) { }
        self_type operator++() { self_type i = *this; ptr_--; return i; }
        self_type operator++(int junk) { ptr_--; return *this; }
        reference operator*() { return *ptr_; }
        pointer operator->() { return ptr_; }
        bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
    private:
        pointer ptr_;
    };

    class const_reverse_iterator
    {
    public:
        typedef const_reverse_iterator self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::random_access_iterator_tag iterator_category;

        const_reverse_iterator(pointer ptr) : ptr_(ptr) { }
        self_type operator++() { self_type i = *this; ptr_--; return i; }
        self_type operator++(int junk) { ptr_--; return *this; }
        const reference operator*() { return *ptr_; }
        const pointer operator->() { return ptr_; }
        bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
    private:
        pointer ptr_;
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

    void insert_hash_elem(T& val, uint32_t pos);
    void remove_hash_elem(T& val, uint32_t pos);
    void update_hash_table();
    void no_update() { is_always_update = false; }
    void always_update() { is_always_update = true; }

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
    T* data();
    const T* data() const;

    iterator begin();
    const_iterator begin() const;

    template <class InputIterator> void assign(InputIterator first, InputIterator last);
    void assign(size_t n, const T& val);
    void assign(std::initializer_list<T> il);

    void push_back(const T& val);
    void push_back(T&& val);
    void pop_back();

    void clear();

    friend void swap<>(hashed_vector<T, Hash> &one, hashed_vector<T, Hash> &two);

private:
    static const uint32_t HASH_MOD = 113;

    typedef std::list<std::pair<T&,uint32_t>> hash_elem;
    typedef std::pair<uint32_t, uint32_t> change;

    std::vector<T> vector_;
    std::vector<hash_elem> hash_table_;
    bool is_always_update = true;
};


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::hashed_vector(size_t n) : vector_(n), hash_table_(HASH_MOD) {
    update_hash_table();
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::hashed_vector(size_t n, const T& val) : vector_(n, val), hash_table_(HASH_MOD) {
    update_hash_table();
}


template <typename T, int32_t (*Hash)(T&)>
template <class InputIterator>
hashed_vector<T, Hash>::hashed_vector(InputIterator first, InputIterator last) : vector_(first, last) {
    update_hash_table();
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::hashed_vector(const hashed_vector& hv) : vector_(hv.vector_), hash_table_(hv.hash_table_) {
    update_hash_table();
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::hashed_vector(hashed_vector&& hv) : vector_(std::move(hv.vector_)),
    hash_table_(std::move(hv.hash_table_)) {}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::hashed_vector(std::initializer_list<T> il) : vector_(il) {
    update_hash_table();
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>::~hashed_vector() {}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>& hashed_vector<T, Hash>::operator=(hashed_vector hv) {
    swap(this, hv);
    return *this;
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>& hashed_vector<T, Hash>::operator=(hashed_vector&& hv) {
    vector_ = std::move(hv.vector_);
    update_hash_table();
    return *this;
}


template <typename T, int32_t (*Hash)(T&)>
hashed_vector<T, Hash>& hashed_vector<T, Hash>::operator=(std::initializer_list<T> il) {
    vector_ = il;
    update_hash_table();
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
bool hashed_vector<T, Hash>::empty() const {
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
    return vector_[n];
}


template <typename T, int32_t (*Hash)(T&)>
const T& hashed_vector<T, Hash>::operator[](size_t n) const {
    return vector_[n];
}


template <typename T, int32_t (*Hash)(T&)>
T& hashed_vector<T, Hash>::at(size_t n) {
    return vector_.at(n);
}


template <typename T, int32_t (*Hash)(T&)>
const T& hashed_vector<T, Hash>::at(size_t n) const {
    return vector_.at(n);
}


template <typename T, int32_t (*Hash)(T&)>
T& hashed_vector<T, Hash>::front() {
    return vector_.front();
}


template <typename T, int32_t (*Hash)(T&)>
const T& hashed_vector<T, Hash>::front() const {
    return vector_.front();
}


template <typename T, int32_t (*Hash)(T&)>
T& hashed_vector<T, Hash>::back() {
    return vector_.back();
}


template <typename T, int32_t (*Hash)(T&)>
const T& hashed_vector<T, Hash>::back() const {
    return vector_.back();
}


template <typename T, int32_t (*Hash)(T&)>
T* hashed_vector<T, Hash>::data() {
    return vector_.data();
}


template <typename T, int32_t (*Hash)(T&)>
const T* hashed_vector<T, Hash>::data() const {
    return vector_.data();
}


template <typename T, int32_t (*Hash)(T&)>
class hashed_vector<T, Hash>::iterator hashed_vector<T, Hash>::begin() {
    return iterator(vector_.data());
}

template <typename T, int32_t (*Hash)(T&)>
class hashed_vector<T, Hash>::const_iterator hashed_vector<T, Hash>::begin() const {
    return const_iterator(vector_.data());
};



template <typename T, int32_t (*Hash)(T&)>
template <class InputIterator>
void hashed_vector<T, Hash>::assign(InputIterator first, InputIterator last) {
    vector_.assign(first, last);
    update_hash_table();
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::assign(size_t n, const T& val) {
    vector_.assign(n, val);
    update_hash_table();
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::assign(std::initializer_list<T> il) {
    vector_.assign(il);
    update_hash_table();
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::push_back(const T& val) {
    vector_.push_back(val);
    insert_hash_elem(vector_.back(), vector_.size() - 1);
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::push_back(T&& val) {
    vector_.push_back(val);
    insert_hash_elem(vector_.back(), vector_.size() - 1);
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::pop_back() {
    if (!vector_.empty()) {
        remove_hash_elem(vector_.back(), vector_.size() - 1);
    }
    vector_.pop_back();
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::clear() {
    vector_.clear();
    update_hash_table();
}


template <typename T, int32_t (*Hash)(T&)>
void swap(hashed_vector<T, Hash> &one, hashed_vector<T, Hash> &two) {
    std::swap(one.vector_, two.vector_);
    std::swap(one.hash_table_, two.hash_table_);
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::insert_hash_elem(T &val, uint32_t pos) {
    uint32_t index = Hash(val) % HASH_MOD;
    hash_table_[index].push_back(std::pair<T&, size_t>(val, pos));
}


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::remove_hash_elem(T &val, uint32_t pos) {
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


template <typename T, int32_t (*Hash)(T&)>
void hashed_vector<T, Hash>::update_hash_table() {
    size_t i = 0;
    std::for_each(hash_table_, hash_table_ + HASH_MOD,
                  [](hash_elem &elem){ elem.clear(); });
    std::for_each(vector_.begin(), vector_.end(),
                  [&i](T &val) { insert_hash_elem(val, i); ++i;});
}
#endif //OBJC_HASHED_VECTOR_HPP_
