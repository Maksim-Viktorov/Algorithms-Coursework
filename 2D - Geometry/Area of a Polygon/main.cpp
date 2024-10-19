#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
  int64_t x = 0;
  int64_t y = 0;
  Point(int64_t x1, int64_t y1) : x(x1), y(y1) {
  }
  Point() = default;
};

bool operator==(const Point& p1, const Point& p2) {
  return (p1.x == p2.x) && (p1.y == p2.y);
}

struct Vector {
  int64_t x = 0;
  int64_t y = 0;
  Vector() = default;
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

int64_t ScalarProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

int64_t VectorProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.y - v2.x * v1.y;
}

struct Polygon {
  std::vector<Point> points;
  explicit Polygon(const std::vector<Point>& vect) : points(vect) {
  }
  Polygon(const Polygon& other) = default;
  Polygon(Polygon&& other) = default;
  Polygon& operator=(const Polygon& other) = default;
  Polygon& operator=(Polygon&& other) = default;
  ~Polygon() = default;
  int64_t AreaX2() const {
    int64_t area = 0;
    for (size_t i = 1; i < points.size() - 1; ++i) {
      Vector a(points[0], points[i]);
      Vector b(points[0], points[i + 1]);
      area += VectorProduct(a, b);
    }
    return std::abs(area);
  }
};

int main() {
  size_t n;
  std::cin >> n;
  int64_t x, y;
  std::vector<Point> points;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> x >> y;
    points.emplace_back(Point(x, y));
  }
  Polygon polygon(points);
  int64_t area = polygon.AreaX2();
  if (area % 2 == 0) {
    std::cout << area / 2 << ".0" << '\n';
  } else {
    std::cout << area / 2 << ".5" << '\n';
  }
  return 0;
}
