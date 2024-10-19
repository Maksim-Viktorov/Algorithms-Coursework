#include <algorithm>
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

bool Cmp(const Point& p1, const Point& p2) {
  return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
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
  Vector& operator*=(int64_t val) {
    x *= val;
    y *= val;
    return *this;
  }
  double Len() const {
    return std::sqrt(x * x + y * y);
  }
};

bool VectorLenCmp(const Vector& v1, const Vector& v2) {
  return (v1.x * v1.x + v1.y * v1.y < v2.x * v2.x + v2.y * v2.y);
}

int64_t ScalarProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

int64_t VectorProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.y - v2.x * v1.y;
}

struct Line {
  int64_t a = 0;
  int64_t b = 0;
  int64_t c = 0;
  Line(int64_t x1, int64_t y1, int64_t x2, int64_t y2) : a(-(y2 - y1)), b(x2 - x1), c(-(a * x1 + b * y1)) {
  }
  Line(const Point& p2, const Point& p1) : a(-(p2.y - p1.y)), b(p2.x - p1.x), c(-(a * p1.x + b * p1.y)) {
  }
  Vector GuideVector() const {
    return Vector(b, -a);
  }
  bool Contains(const Point& point) const {
    return (a * point.x + b * point.y + c == 0);
  }
};

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

Polygon JarvisConvexHull(std::vector<Point> points) {
  size_t idx = 0;
  Point first(*std::min_element(points.data(), points.data() + points.size(), Cmp));
  Point last = first;
  std::vector<Point> hull;
  hull.emplace_back(last);
  idx = 1;
  Vector checked(last.x, last.y, last.x, last.y - 1);
  while (true) {
    for (size_t i = 0; i < points.size(); ++i) {
      Vector current(last, points[i]);
      if (VectorProduct(checked, current) > 0) {
        checked = current;
        idx = i;
      } else if (VectorProduct(checked, current) == 0 &&
                 ((ScalarProduct(checked, current) < 0 || VectorLenCmp(checked, current)))) {
        checked = current;
        idx = i;
      }
    }
    if (first == points[idx]) {
      break;
    }
    checked *= -1;
    last = points[idx];
    hull.emplace_back(last);
  }
  Polygon polygon(hull);
  return polygon;
}

int main() {
  size_t n;
  int64_t x, y;
  std::cin >> n;
  std::vector<Point> points;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> x >> y;
    points.emplace_back(Point(x, y));
  }
  Polygon polygon = JarvisConvexHull(points);
  std::cout << polygon.points.size() << '\n';
  for (size_t i = 0; i < polygon.points.size(); ++i) {
    std::cout << polygon.points[i].x << ' ' << polygon.points[i].y << '\n';
  }
  int64_t area = polygon.AreaX2();
  if (area % 2 == 0) {
    std::cout << area / 2 << ".0" << '\n';
  } else {
    std::cout << area / 2 << ".5" << '\n';
  }
  return 0;
}
