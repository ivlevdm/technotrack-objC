#include "math_vector.hpp"

#include <stdarg.h>
#include <iterator>
#include <stdexcept>
#include <cmath>

MathVector::MathVector(unsigned int size): size_(size), values_(new double[size]) {}


MathVector::MathVector(int argn, ...): size_(argn), values_(new double[argn]) { 
    va_list values;
    va_start(values, argn);
    
    for (unsigned int i = 0; i < size_; ++i) {
        values_[i] = va_arg(values, double);
    }
    va_end(values);
}


MathVector::MathVector(const MathVector& copy): size_(copy.size_), 
    values_(new double[size_]) {
    
    std::copy(copy.values_, copy.values_ + copy.size_, values_); 
} 


MathVector::~MathVector() {
    delete [] values_;
}


void MathVector::set_at(unsigned int dim, double val) {
    if (dim >= size_) {
        throw std::out_of_range("There is not such dimenision\n");
    }
    values_[dim] = val;
}


double MathVector::get_at(unsigned int dim) const {
    if (dim >= size_) {
        throw std::out_of_range("There is not such dimenision\n");
    }
    return values_[dim];
 }


unsigned int MathVector::get_size() const {
    return size_;
}


MathVector MathVector::opposite() const {
    MathVector opposite_vect = *this;

    for ( unsigned int i = 0; i < size_; ++i) {
        opposite_vect[i] *= -1;
    }
    return opposite_vect;
}


MathVector MathVector::add(const MathVector& other) const {
    if ( size_ != other.size_) {
        throw std::length_error("Vectors have different dimenisions");
    }
    
    MathVector sum = MathVector(size_);


    for (unsigned int i = 0; i < size_; ++i) {
        sum[i] = values_[i] + other.values_[i];
    }
    return sum;
}


double MathVector::scalar_mul(const MathVector& other) const {
    if ( size_ != other.size_) {
        throw std::length_error("Vectors have different dimenisions");
    }
    
    double res = 0.0;
    
    for (unsigned int i = 0; i < size_; ++i) {
        res += values_[i] * other.values_[i];
    }
    return res;
}


double MathVector::length() const {
    return sqrt(scalar_mul(*this));
}


MathVector MathVector::operator-() const {
    return opposite();
}


MathVector& MathVector::operator=(MathVector copy) {
    std::swap(size_, copy.size_);
    std::swap(values_, copy.values_);
    return *this;
} 


double& MathVector::operator[](unsigned int dim) {
    if (dim >= size_) {
        throw std::out_of_range("There is not such dimenision\n");
    }
    
    return values_[dim];
}


MathVector MathVector::operator+(const MathVector& other) const {
    return add(other);
}


double MathVector::operator*(const MathVector& other) const {
    return scalar_mul(other);
}



std::ostream& operator<<(std::ostream& os, const MathVector& vect) {
    if (vect.size_ == 0) {
        return os;
    }
    os << '(';
    os << vect.values_[0];
    for (unsigned int i = 1; i < vect.size_; ++i) {
        os << ", " << vect.values_[i];
    }
    os << ')';
    return os;
}

