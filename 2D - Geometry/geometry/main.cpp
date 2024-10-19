/*#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace geometry {

struct Point;

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
  Vector& operator+=(const Vector& other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Vector& operator-=(const Vector& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  Vector& operator*=(int64_t val) {
    x *= val;
    y *= val;
    return *this;
  }
  Vector& operator/=(int64_t val) {
    x /= val;
    y /= val;
    return *this;
  }
  double Len() const {
    return std::sqrt(x * x + y * y);
  }
};

Vector operator+(const Vector& v1, const Vector& v2) {
  return Vector(v1.x + v2.x, v1.y + v2.y);
}

Vector operator-(const Vector& v1, const Vector& v2) {
  return Vector(v1.x - v2.x, v1.y - v2.y);
}

Vector operator*(const Vector& v1, int64_t val) {
  return Vector(v1.x * val, v1.y * val);
}

Vector operator/(const Vector& v1, int64_t val) {
  return Vector(v1.x / val, v1.y / val);
}

bool operator==(const Vector& v1, const Vector& v2) {
  return (v1.x == v2.x && v1.y == v2.y);
}

bool VectorLenCmp(const Vector& v1, const Vector& v2) {
  return (v1.x * v1.x + v1.y * v1.y < v2.x * v2.x + v2.y * v2.y);
}

int64_t ScalarProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

int64_t VectorProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.y - v2.x * v1.y;
}

struct Segment;

struct IShape {
  virtual IShape& Move(const Vector&) = 0;
  virtual bool ContainsPoint(const Point&) = 0;
  virtual bool CrossesSegment(const Segment&) = 0;
  virtual IShape* Clone() = 0;
  virtual std::string ToString() = 0;
  virtual ~IShape() {
  }
};

bool operator==(const Point& p1, const Point& p2);

struct Point : IShape {
  int64_t x = 0;
  int64_t y = 0;
  Point(int64_t x1, int64_t y1) : x(x1), y(y1) {
  }
  Point(const Point& p)  x(point.x), y(point.y) {
  }
  Point() = default;
  Point& Move(const Vector& v) {
    x += v.x;
    y += v.y;
    return *this;
  }
  bool ContainsPoint(const Point& p) {
    return p == *this;
  }
//  bool CrossesSegment(const Segment& s) {
//    return s.CointainsPoint(*this);
//  }
  IShape* Clone() {
    return new Point(*this);
  }
  std::string ToString() {
    return "Point(" + string(x) + ", " + string(y) + ")\n";
  }
};

Vector operator-(const Point& p1, const Point& p2) {
  return Vector(p1, p2);
}

bool operator==(const Point& p1, const Point& p2) {
  return (p1.x == p2.x) && (p1.y == p2.y);
}

struct Segment : IShape {
  Point a;
  Point b;
  Segment(int64_t x1, int64_t y1, int64_t x2, int64_t y2) : a(x1, y1), b(x2, y2) {
  }
  Segment(const Point& p1, const Point& p2) : a(p1), b(p2) {
  }
  Segment& Move(const Vector& v) {
    a.x += v.x;
    a.y += v.y;
    b.x += v.x;
    b.y += v.y;
    return *this;
  }
  bool ContainsPoint(const Point& p) const {
    Vector ap(a, p);
    Vector pb(p, b);
    return (ScalarProduct(ap, pb) >= 0 && VectorProduct(ap, pb) == 0);
  }
  bool CrossesSegment(const Segment& s) {
    Line l1(a, b);
    Line l2(s.a, s.b);
    if (l1.CrossesSegment(s) && l2.CrossesSegment(*this) {
      return true;
    }
    return ContainsPoint(s.a) || ContainsPoint(s.b) || s.ContainsPoint(a);
  }
  IShape* Clone() {
    return new Segment(*this);
  }
  std::string ToString() {
    return "Segment(Point(" + string(a.x) + ", " + string(a.y) + "), " + "Point(" + string(b.x) + ", " + string(b.y) + "))\n";
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

struct Line : IShape {
  int64_t a = 0;
  int64_t b = 0;
  int64_t c = 0;
  Point point1(0, 0);
  Point point2(0, 0);
  Line(const Point& p1, const Point& p2) {
    point1 = p1;
    point2 = p2;
    a = -(p2.y - p1.y);
    b = p2.x - p1.x;
    c = -(a * p1.x + b * p1.y);
    int64_t gcd = std::gcd(a, std::gcd(b, c));
    if (gcd != 1) {
      a /= gcd;
      b /= gcd;
      c /= gcd;
    }
    if ((a < 0 && b < 0) || (a < 0 && c < 0) ||(b < 0 && c < 0)) {
      a *= -1;
      b *= -1;
      c *= -1;
    }
  }
  Line(int64_t a_, b_, c_) : a(a_), b(b_), c(c_) {
  }
  Line& Move(const Vector& v) {
    *this = Line(Point(point1.x + v.x, point1.y + v.y), Point(point2.x + v.x, point2.y + v.y));
    return *this;
  Vector GuideVector() const {
    return Vector(b, -a);
  }
  bool ContainsPoint(const Point& p) const {
    return (a * p.x + b * p.y + c == 0);
  }
  bool CrossesSegment(const Segment& s) {
    return (!(ContainsPoint(s.a) && ContainsPoint(s.b)) &&
            ((a * s.a.x + b * s.a.y + c) * (a * s.b.x + b * s.b.y + c) <= 0));
  }
  IShape* Clone() {
    return new Line(*this);
  }
  std::string ToString() {
    return "Line(" + string(a) + ", " + string(b) + ", " + string(c) ")\n";
  }
  Line PerpendecularLine(const Point& p) {
    return Line(a, -b, -a * p.x + b * p.y);
  }
  double Distance(const Point& p) const {
    return std::fabs(double(a * p.x + b * p.y + c) / std::sqrt(a * a + b * b));
  }
};

struct Ray  : IShape {
  Point point;
  Vector guide;
  Ray(int x1, int y1, int x2, int y2) : point(x1, y1), guide(x2 - x1, y2 - y1) {
  }
  Ray(const Point& p1, const Point& p2) {
    point = p1;
    guide = Vector(p2.x - p1.x, p2.y - p1.y);
  }
  Ray& Move(const Vector& v) {
    *this = Ray(Point(point.x + v.x, point.y + v.y), Point(point.x + v.x + guide.x, point.y + v.y + guide.y));
    return *this;
  bool ContainsPoint(const Point& c) const {
    Vector pc(point.x, point.y, c.x, c.y);
    return (ScalarProduct(guide, pc) >= 0 && VectorProduct(guide, pc) == 0);
  }
  bool CrossesSegment(const Segment& s) {
    if ((ContainsPoint(s.a) && ContainsPoint(s.b)) || !Line(point, Point(point.x + guide.x, point.y + guide.y)).CrossesSegment(s)) {
      return false;
    }
    Vector vector1 = Vector(s.a, point);
    Vector vector2 = Vector(s.a, s.b);
    int64_t vp_of_v1_v2 = VectorProduct(vector1, vector2);
    int64_t vp_of_guide_v2 = VectorProduct(guide, vector2);
    return ((vp_of_v1_v2 > 0 && vp_of_guide_v2 < 0) || (vp_of_v1_v2 < 0 && vp_of_guide_v2 > 0));
  }
  IShape* Clone() {
    auto ray = ;
    return new Ray(*this);
  }
  std::string ToString() {
    return "Ray(Point(" + string(point.x) + ", " + string(point.y) + "), " + "Vector(" + string(guide.x) + ", " + string(guide.y) + "))\n";
  }
  double Distance(const Point& p) const {
    if (ScalarProduct(guide, Vector(point, p)) > 0) {
      return std::fabs(VectorProduct(guide, Vector(point, p)) / guide.Len());
    }
    return Vector(point, p).Len();
  }
};

struct Polygon {
  std::vector<Point> points;
  Polygon(const std::vector<Points>& vect) {
    points = vect;
  }
  Polygon(const Polygon& other) {
    points = other.points;
  }
  Polygon(Polygon&& other) noexcept {
    points = std::move(other.points);
  }
  ~Polygon() {
  }
  Polygon& operator=(Polygon&& other) noexcept {
    points = std::move(other.points);
    return *this;
  }
  Polygon& Move(const Vector& v) {
    for (size_t i = 0; i < points.size()) {
      points[i].x += v.x;
      points[i].y += v.y;
    }
    return *this;
  }
  bool ContainsPoint(const Point& point) {
    int64_t inf = 21474836;
    Ray ray(point, Point(point.x + 1, inf));
    int64_t size = points.size();
    int64_t count = 0;
    for (int64_t i = 0; i < points.size(); ++i) {
      if (points[i] == point) {
        return true;
      }
      if (ray.CrossesSegment(Segment(points[i], points[(i + 1) % points.size()]))) {
        ++count;
      }
    }
    return (count % 2 == 1);
  }
  bool CrossesSegment(const Segment& s) {
    for (size_t i = 1; i < points.size() + 1) {
      if (Segment(points[i - 1], points[i % points.size()]).CrossesSegment(s)) {
        return true;
      }
    }
    return false;
  }
  IShape* Clone() {
    return new Polygon(*this);
  }
  std::string ToString() {
    string str = "Polygon(";
    for (size_t i = 0; i < point.size(); ++i) {
      str += "Point(" + string(point.x) + ", " + string(point.y) + ")";
      if (i != point.size() - 1) {
        str += ", ";
      }
    }
    return str + ")\n";
  }
  int64_t AreaX2() const {
    int64_t area = 0;
    for (size_t i = 1; i < angles_num - 1; ++i) {
      Vector a(points[0], points[i]);
      Vector b(points[0], points[i + 1]);
      area += VectorProduct(a, b);
    }
    return std::abs(area);
  }
};

struct Circle() : IShape {
  Point point;
  int64_t radius;
  Circle(const Point& p, int64_t r) : point(p), radius(r) {
  }
  Circle& Move(const Vector& v) {
    point = Point(point.x + v.x, point.y + v.y);
    return *this;
  }
  bool ContainsPoint(const Point& p) {
    return ((p - point).Len() <= radius);
  }
  bool CrossesSegment(const Segment& s) {
    Line line(s.a, s.b);
    if (line.Distance(point) <= r) {
      if ((!VectorLenCmp(radius, point - s.a) && !VectorLenCmp(point - s.b, raduis)) ||
          (!VectorLenCmp(radius, point - s.b) && !VectorLenCmp(point - s.a, raduis))) {
        return true;
      }
      return (VectorLenCmp(radius, point - s.a) && VectorLenCmp(radius, point - s.b) &&
              line.PerpendecularLine(point).CrossesSegment(s));
    }
    return false;
  }
  IShape* Clone() {
    return new Circle(*this);
  }
  std::string ToString() {
    return "Circle(Point(" + string(point.x) + ", " + string(point.y) + "), " + string(radius) ")\n";
  }
};

}
*/
#include <iostream>
#include <vector>
#include <memory>

#include "vector.h"
#include "point.h"
#include "segment.h"
#include "ray.h"
#include "line.h"
#include "circle.h"
#include "polygon.h"

#include "vector.h"   // check include guards
#include "point.h"    // check include guards
#include "segment.h"  // check include guards
#include "ray.h"      // check include guards
#include "line.h"     // check include guards
#include "circle.h"   // check include guards
#include "polygon.h"  // check include guards

template <class SmartPtrT>
void Delete(const SmartPtrT&) {
}

template <class T>
void Delete(T* ptr) {
  delete ptr;
}

void CheckFunctions(const geometry::IShape* shape_ptr, const geometry::Point& point_a, const geometry::Point& point_b) {
  std::cout << "Given shape " << (shape_ptr->ContainsPoint(point_a) ? "contains" : "does not contain") << " point A\n";

  const auto segment_ab = geometry::Segment(point_a, point_b);
  std::cout << "Given shape " << (shape_ptr->CrossesSegment(segment_ab) ? "crosses" : "does not cross")
            << " segment AB\n";

  const auto vector_ab = point_b - point_a;
  const auto cloned_shape_ptr = shape_ptr->Clone();  // may return either raw or smart pointer
  std::cout << cloned_shape_ptr->Move(vector_ab).ToString();

  Delete(cloned_shape_ptr);  // raw pointer compatibility
}

int main() {
  std::string command;
  std::cin >> command;

  auto shape_ptr = std::unique_ptr<geometry::IShape>();

  int x = 0;
  int y = 0;
  int a = 0;
  int b = 0;

  if (command == "point") {
    std::cin >> x >> y;
    shape_ptr = std::make_unique<geometry::Point>(x, y);
  } else if (command == "segment") {
    std::cin >> x >> y >> a >> b;
    shape_ptr = std::make_unique<geometry::Segment>(geometry::Point(x, y), geometry::Point(a, b));
  } else if (command == "ray") {
    std::cin >> x >> y >> a >> b;
    shape_ptr = std::make_unique<geometry::Ray>(geometry::Point(x, y), geometry::Point(a, b));
  } else if (command == "line") {
    std::cin >> x >> y >> a >> b;
    shape_ptr = std::make_unique<geometry::Line>(geometry::Point(x, y), geometry::Point(a, b));
  } else if (command == "polygon") {
    size_t n_points = 0;
    std::cin >> n_points;
    auto points = std::vector<geometry::Point>();
    points.reserve(n_points);
    for (size_t i = 0; i < n_points; ++i) {
      std::cin >> x >> y;
      points.emplace_back(x, y);
    }
    shape_ptr = std::make_unique<geometry::Polygon>(std::move(points));
  } else if (command == "circle") {
    std::cin >> x >> y;
    const auto center = geometry::Point(x, y);
    uint32_t radius = 0;
    std::cin >> radius;
    shape_ptr = std::make_unique<geometry::Circle>(center, radius);
  } else {
    std::cerr << "Undefined command" << std::endl;
    return 1;
  }

  std::cin >> x >> y;
  const auto point_a = geometry::Point(x, y);
  std::cin >> x >> y;
  const auto point_b = geometry::Point(x, y);

  CheckFunctions(shape_ptr.get(), point_a, point_b);
  return 0;
}
