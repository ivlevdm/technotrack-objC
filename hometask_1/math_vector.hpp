#ifndef OBJC_HOMETASK_1_MATH_VECTOR_HPP_
#define OBJC_HOMETASK_1_MATH_VECTOR_HPP_

#include <ostream>

class MathVector {
public:
    MathVector();
    MathVector(unsigned int size);
    MathVector(int argn, ...);
    MathVector(const MathVector& copy);
    ~MathVector();

    void set_at(unsigned int dim, double val);
    double get_at(unsigned int dim) const;
    unsigned int get_size() const;
    MathVector opposite() const;
    MathVector add(const MathVector& other) const;
    MathVector sub(const MathVector& other) const;
    double scalar_mul(const MathVector& other) const;
    double length() const;

    MathVector operator-() const;
    MathVector& operator=(MathVector copy);
    double& operator[](unsigned int dim);

    MathVector operator+(const MathVector& other) const;
    MathVector operator-(const MathVector& other) const;
    double operator*(const MathVector& other) const;
    friend MathVector operator*(double coef, const MathVector& vect);

    friend std::ostream& operator<<(std::ostream& os, const MathVector& vect);
    friend std::istream& operator>>(std::istream& is, MathVector& vect);

private:
    unsigned int size_;
    double *values_;
};

#endif // OBJC_HOMETASK_1_MATH_VECTOR_HPP_
