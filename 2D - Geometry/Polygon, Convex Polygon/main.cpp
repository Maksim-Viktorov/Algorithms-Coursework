#include <cmath>
#include <iomanip>
#include <iostream>

struct Point {
  int64_t x;
  int64_t y;
  Point() : x(0), y(0) {
  }
  Point(int64_t x1, int64_t y1) : x(x1), y(y1) {
  }
};

struct Vector {
  int64_t x;
  int64_t y;
  Vector() : x(0), y(0) {
  }
  Vector(int64_t x1, int64_t y1, int64_t x2, int64_t y2) : x(x2 - x1), y(y2 - y1) {
  }
  Vector(int64_t x1, int64_t y1) : x(x1), y(y1) {
  }
  Vector(const Point& p1, const Point& p2) : x(p2.x - p1.x), y(p2.y - p1.y) {
  }
  double Len() const {
    return std::sqrt(x * x + y * y);
  }
};

Vector operator*(int64_t number, const Vector& v1) {
  return Vector(v1.x * number, v1.y * number);
}
int64_t ScalarProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

int64_t VectorProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.y - v2.x * v1.y;
}

struct Polygon {
  int64_t angles_num = 0;
  Point* points;
  Polygon(Point* data, int64_t size) {
    angles_num = size;
    points = new Point[size];
    for (int64_t i = 0; i < size; ++i) {
      points[i] = data[i];
    }
  }
  ~Polygon() {
    delete[] points;
  }
};

bool ConvexPolygon(const Polygon& polygon) {
  int64_t size = polygon.angles_num;
  auto vectors_of_sides = new Vector[size];
  for (int64_t i = 0; i < size - 1; ++i) {
    vectors_of_sides[i] = Vector(polygon.points[i], polygon.points[i + 1]);
  }
  vectors_of_sides[size - 1] = Vector(polygon.points[size - 1], polygon.points[0]);
  int64_t angle_sign = 0;
  for (int64_t i = 0; i < size - 1; ++i) {
    if (angle_sign == 0 && VectorProduct(-1 * vectors_of_sides[i], vectors_of_sides[i + 1]) != 0) {
      if (VectorProduct(-1 * vectors_of_sides[i], vectors_of_sides[i + 1]) > 0) {
        angle_sign = 1;
      } else {
        angle_sign = -1;
      }
    }
    if (angle_sign * VectorProduct(-1 * vectors_of_sides[i], vectors_of_sides[i + 1]) < 0) {
      delete[] vectors_of_sides;
      return false;
    }
  }
  bool ans = !(angle_sign * VectorProduct(-1 * vectors_of_sides[size - 1], vectors_of_sides[0]) < 0);
  delete[] vectors_of_sides;
  return ans;
}

int main() {
  int64_t n;
  int64_t x, y;
  std::cin >> n;
  auto points = new Point[n];
  for (int64_t i = 0; i < n; ++i) {
    std::cin >> x >> y;
    points[i] = Point(x, y);
  }
  Polygon polygon(points, n);
  std::cout << (ConvexPolygon(polygon) ? "YES" : "NO") << '\n';
  delete[] points;
  return 0;
}
