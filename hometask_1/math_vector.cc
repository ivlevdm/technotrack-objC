#include "math_vector.hpp"

#include <stdarg.h>
#include <iterator>
#include <stdexcept>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>

MathVector::MathVector(): MathVector(0) {}


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

    for (unsigned int i = 0; i < size_; ++i) {
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


MathVector MathVector::sub(const MathVector& other) const {
    return add(other.opposite());
}


double MathVector::scalar_mul(const MathVector& other) const {
    if (size_ != other.size_) {
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
    return values_[dim];
}


MathVector MathVector::operator-(const MathVector& other) const {
    return sub(other);
}


MathVector MathVector::operator+(const MathVector& other) const {
    return add(other);
}


double MathVector::operator*(const MathVector& other) const {
    return scalar_mul(other);
}


MathVector operator*(double coef, const MathVector& vect) {
    MathVector res = MathVector(vect.size_);
    for (unsigned int i = 0; i < vect.size_; ++i) {
        res[i] = vect.values_[i] * coef;
    }
    return res;
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


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


std::istream& operator>>(std::istream& is, MathVector& vect) {
    char c;
    std::string input;
    is >> c;
    if (c != '(') {
        throw std::runtime_error("Incorrect format");
    }

    is >> c;
    while (c != ')') {
        input += c;
        is >> c;
    }

    std::vector<std::string> str_values = split(input, ',');
    double* buf = new double[str_values.size()];
    if (vect.size_ != 0) {
        delete[] vect.values_;
    }
    vect.values_ = buf;
    vect.size_ = str_values.size();
    for (unsigned int  i = 0; i < vect.size_; ++i) {
        vect.values_[i] = std::stod(str_values[i]);
    }

    return is;
}
