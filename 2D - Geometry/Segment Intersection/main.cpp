#include <cmath>
#include <iomanip>
#include <iostream>

struct Point {
  int64_t x = 0;
  int64_t y = 0;
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
  int64_t a = 0;
  int64_t b = 0;
  int64_t c = 0;
  Line(int64_t x1, int64_t y1, int64_t x2, int64_t y2) : a(-(y2 - y1)), b(x2 - x1), c(-(a * x1 + b * y1)) {
  }
  Line(Point p2, Point p1) : a(-(p2.y - p1.y)), b(p2.x - p1.x), c(-(a * p1.x + b * p1.y)) {
  }
  Vector GuideVector() const {
    return Vector(b, -a);
  }
  bool Contains(const Point& p) const {
    return (a * p.x + b * p.y + c == 0);
  }
  double Distance(const Point& p) const {
    return std::fabs(double(a * p.x + b * p.y + c) / std::sqrt(a * a + b * b));
  }
};

struct Segment {
  Point a;
  Point b;
  Segment(int64_t x1, int64_t y1, int64_t x2, int64_t y2) : a(x1, y1), b(x2, y2) {
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

bool LineIntersectsSegment(const Line& l, const Segment& s) {
  return (!(l.Contains(s.a) && l.Contains(s.b)) &&
          ((l.a * s.a.x + l.b * s.a.y + l.c) * (l.a * s.b.x + l.b * s.b.y + l.c) <= 0));
}

bool SegmentIntersection(const Segment& s1, const Segment& s2) {
  Line l1(s1.a, s1.b);
  Line l2(s2.a, s2.b);
  if (LineIntersectsSegment(l1, s2) && LineIntersectsSegment(l2, s1)) {
    return true;
  }
  return s1.Contains(s2.a) || s1.Contains(s2.b) || s2.Contains(s1.a);
}

int main() {
  int64_t x1, y1, x2, y2;
  std::cin >> x1 >> y1 >> x2 >> y2;
  Segment segm1(x1, y1, x2, y2);
  std::cin >> x1 >> y1 >> x2 >> y2;
  Segment segm2(x1, y1, x2, y2);
  std::cout << std::fixed << std::setprecision(6);
  std::cout << (SegmentIntersection(segm1, segm2) ? "YES" : "NO") << '\n';
  return 0;
}
