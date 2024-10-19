#include <cmath>
#include <iomanip>
#include <iostream>

struct Point {
  int x = 0;
  int y = 0;
  Point(int x1, int y1) : x(x1), y(y1) {
  }
};

struct Vector {
  int x;
  int y;
  Vector(int x1, int y1, int x2, int y2) : x(x2 - x1), y(y2 - y1) {
  }
  Vector(int x1, int y1) : x(x1), y(y1) {
  }
  Vector(const Point& p1, const Point& p2) : x(p2.x - p1.x), y(p2.y - p1.y) {
  }
  double Len() const {
    return std::sqrt(x * x + y * y);
  }
};

int ScalarProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

int VectorProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.y - v2.x * v1.y;
}

struct Line {
  int a = 0;
  int b = 0;
  int c = 0;
  Line(int x1, int y1, int x2, int y2) : a(-(y2 - y1)), b(x2 - x1), c(-(a * x1 + b * y1)) {
  }
  Line(const Point& p2, const Point& p1) : a(-(p2.y - p1.y)), b(p2.x - p1.x), c(-(a * p1.x + b * p1.y)) {
  }
  Vector GuideVector() const {
    return Vector(b, -a);
  }
  double Distance(const Point& p) const {
    return std::fabs(double(a * p.x + b * p.y + c) / std::sqrt(a * a + b * b));
  }
};

struct Ray {
  Point point;
  Vector guide;
  Ray(int x1, int y1, int x2, int y2) : point(x1, y1), guide(x2 - x1, y2 - y1) {
  }
  bool Contains(const Point& c) const {
    Vector pc(point.x, point.y, c.x, c.y);
    return (ScalarProduct(guide, pc) >= 0 && VectorProduct(guide, pc) == 0);
  }
  double Distance(const Point& p) const {
    if (ScalarProduct(guide, Vector(point, p)) > 0) {
      return std::fabs(VectorProduct(guide, Vector(point, p)) / guide.Len());
    }
    return Vector(point, p).Len();
  }
};

struct Segment {
  Point a;
  Point b;
  Segment(int x1, int y1, int x2, int y2) : a(x1, y1), b(x2, y2) {
  }
  bool Contains(const Point& c) const {
    Vector ac(a.x, a.y, c.x, c.y);
    Vector cb(c.x, c.y, b.x, b.y);
    return (ScalarProduct(ac, cb) >= 0 && VectorProduct(ac, cb) == 0);
  }
  double Distance(const Point& p) const {
    if (ScalarProduct(Vector(a, b), Vector(a, p)) <= 0) {
      return Vector(a, p).Len();
    }
    if (ScalarProduct(Vector(b, a), Vector(b, p)) <= 0) {
      return Vector(b, p).Len();
    }
    return Line(a, b).Distance(p);
  }
};

int main() {
  int x1, y1, x2, y2;
  std::cin >> x1 >> y1;
  Point c(x1, y1);
  std::cin >> x1 >> y1 >> x2 >> y2;
  Line line(x1, y1, x2, y2);
  Ray ray(x1, y1, x2, y2);
  Segment segm(x1, y1, x2, y2);
  std::cout << std::fixed << std::setprecision(6);
  std::cout << line.Distance(c) << '\n';
  std::cout << ray.Distance(c) << '\n';
  std::cout << segm.Distance(c) << '\n';
  return 0;
}
