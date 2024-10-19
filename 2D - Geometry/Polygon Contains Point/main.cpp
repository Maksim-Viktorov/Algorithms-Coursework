#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

int Sign(int64_t val) {
  if (val < 0) {
    return -1;
  }
  if (val > 0) {
    return 1;
  }
  return 0;
}

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
  int64_t x;
  int64_t y;
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

struct Ray {
  Point point;
  Vector guide;
  Ray(int64_t x1, int64_t y1, int64_t x2, int64_t y2) : point(x1, y1), guide(x2 - x1, y2 - y1) {
  }
  Ray(const Point& p, const Vector& v) : point(p), guide(v) {
  }
  bool Contains(const Point& c) const {
    Vector pc(point.x, point.y, c.x, c.y);
    return (ScalarProduct(guide, pc) >= 0 && VectorProduct(guide, pc) == 0);
  }
};

struct Line {
  int64_t a = 0;
  int64_t b = 0;
  int64_t c = 0;
  Line(int64_t x1, int64_t y1, int64_t x2, int64_t y2) : a(-(y2 - y1)), b(x2 - x1), c(-(a * x1 + b * y1)) {
  }
  Line(const Point& p2, const Point& p1) : a(-(p2.y - p1.y)), b(p2.x - p1.x), c(-(a * p1.x + b * p1.y)) {
  }
  explicit Line(const Ray& ray) {
    int64_t x1 = ray.point.x;
    int64_t y1 = ray.point.y;
    int64_t x2 = ray.point.x + ray.guide.x;
    int64_t y2 = ray.point.y + ray.guide.y;
    a = -(y2 - y1);
    b = x2 - x1;
    c = -(a * x1 + b * y1);
  }
  Vector GuideVector() const {
    return Vector(b, -a);
  }
  bool Contains(const Point& point) const {
    return (a * point.x + b * point.y + c == 0);
  }
};

struct Segment {
  Point a;
  Point b;
  Segment(int64_t x1, int64_t y1, int64_t x2, int64_t y2) : a(x1, y1), b(x2, y2) {
  }
  Segment(const Point& p1, const Point& p2) : a(p1.x, p1.y), b(p2.x, p2.y) {
  }
  bool Contains(const Point& c) const {
    Vector ac(a.x, a.y, c.x, c.y);
    Vector cb(c.x, c.y, b.x, b.y);
    return (ScalarProduct(ac, cb) >= 0 && VectorProduct(ac, cb) == 0);
  }
};

struct Polygon {
  std::vector<Point> points;
  explicit Polygon(const std::vector<Point>& vect) {
    points = vect;
  }
  Polygon(const Polygon& other) {
    points = other.points;
  }
  Polygon(Polygon&& other) noexcept {
    points = std::move(other.points);
  }
  Polygon& operator=(Polygon& other) noexcept {
    points = std::move(other.points);
    return *this;
  }
  Polygon& operator=(Polygon&& other) noexcept {
    points = std::move(other.points);
    return *this;
  }
  ~Polygon() = default;
  bool ContainsPoint(const Point& point) const;
};

bool LineIntersectsSegment(const Line& l, const Segment& s) {
  int64_t u = l.a * s.a.x + l.b * s.a.y + l.c;
  int64_t v = l.a * s.b.x + l.b * s.b.y + l.c;
  return (u <= 0 && v >= 0) || (u >= 0 && v <= 0);
}

bool RayIntersectsSegment(const Ray& ray, const Segment& segm) {
  if (ray.Contains(segm.a) || ray.Contains(segm.b)) {
    return true;
  }
  if (!LineIntersectsSegment(Line(ray), segm)) {
    return false;
  }
  Vector vector1 = Vector(segm.a, ray.point);
  Vector vector2 = Vector(segm.a, segm.b);
  int64_t vp_of_v1_v2 = VectorProduct(vector1, vector2);
  int64_t vp_of_guide_v2 = VectorProduct(ray.guide, vector2);
  return ((vp_of_v1_v2 >= 0 && vp_of_guide_v2 < 0) || (vp_of_v1_v2 <= 0 && vp_of_guide_v2 > 0));
}

Point LowerPoint(const Point& p1, const Point& p2) {
  return (p1.y <= p2.y ? p1 : p2);
}

bool Polygon::ContainsPoint(const Point& point) const {
  Ray ray(point, Vector(1, 0));
  size_t count = 0;
  for (size_t i = 0; i < points.size(); ++i) {
    Segment s(points[i], points[(i + 1) % points.size()]);
    if (s.Contains(point)) {
      return true;
    }
    if (RayIntersectsSegment(ray, s)) {
      if (!ray.Contains(LowerPoint(s.a, s.b))) {
        ++count;
      }
    }
  }
  return (count % 2 == 1);
}

/*bool Polygon::ContainsPoint(const Point& point) const {
  Ray ray(point, Vector(2000000001, 1));
  size_t count = 0;
  for (size_t i = 0; i < points.size(); ++i) {
    Segment s(points[i], points[(i + 1) % points.size()]);
    if (s.Contains(point)) {
      return true;
    }
    if (RayIntersectsSegment(ray, s)) {
      ++count;
    }
  }
  return (count % 2 == 1);
}*/

int main() {
  int64_t n;
  std::cin >> n;
  int64_t x, y;
  std::cin >> x >> y;
  Point o(x, y);
  std::vector<Point> points;
  for (int64_t i = 0; i < n; ++i) {
    std::cin >> x >> y;
    points.emplace_back(Point(x, y));
  }
  Polygon polygon(points);
  std::cout << (polygon.ContainsPoint(o) ? "YES" : "NO") << '\n';
  return 0;
}
