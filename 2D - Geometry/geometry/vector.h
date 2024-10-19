#ifndef VECTOR
#define VECTOR

#include "ishape.h"

namespace geometry {

struct Point;

struct Vector {
  int64_t x = 0;
  int64_t y = 0;
  Vector() = default;
  Vector(int64_t x1, int64_t y1, int64_t x2, int64_t y2);
  Vector(int64_t x1, int64_t y1);
  Vector(const Point& p1, const Point& p2);
  Vector& operator+=(const Vector& other);
  Vector& operator-=(const Vector& other);
  Vector& operator*=(int64_t val);
  Vector& operator/=(int64_t val);
  double Len() const;
};

Vector operator+(const Vector& v1, const Vector& v2);
Vector operator-(const Vector& v1, const Vector& v2);
Vector operator*(const Vector& v1, int64_t val);
Vector operator/(const Vector& v1, int64_t val);
bool operator==(const Vector& v1, const Vector& v2);
bool VectorLenCmp(const Vector& v1, const Vector& v2);
int64_t ScalarProduct(const Vector& v1, const Vector& v2);
int64_t VectorProduct(const Vector& v1, const Vector& v2);
}
#endif  // VECTOR
