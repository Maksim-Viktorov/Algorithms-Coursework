#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

struct Vector;
struct Point;
struct Segment;

struct IShape {
  virtual IShape& Move(const Vector&) = 0;
  virtual bool ContainsPoint(const Point&) const = 0;
  virtual bool CrossesSegment(const Segment&) const = 0;
  virtual IShape* Clone() const = 0;
  virtual std::string ToString() const = 0;
  virtual ~IShape() = default;
};

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

struct Segment;
bool operator==(const Point& p1, const Point& p2);

struct Point : IShape {
  int64_t x = 0;
  int64_t y = 0;
  Point() = default;
  Point(int64_t x1, int64_t y1);
  Point(const Point& p);
  Point& Move(const Vector& v) override;
  bool ContainsPoint(const Point& p) const override;
  bool CrossesSegment(const Segment& s) const override;
  IShape* Clone() const override;
  std::string ToString() const override;
};

Vector operator-(const Point& p1, const Point& p2);
bool operator==(const Point& p1, const Point& p2);

struct Segment : IShape {
  Point a;
  Point b;
  Segment(int64_t x1, int64_t y1, int64_t x2, int64_t y2);
  Segment(const Point& p1, const Point& p2);
  Segment& Move(const Vector& v) override;
  bool ContainsPoint(const Point& p) const override;
  bool CrossesSegment(const Segment& s) const override;
  IShape* Clone() const override;
  std::string ToString() const override;
};

struct Ray : IShape {
  Point point;
  Vector guide;
  Ray(int x1, int y1, int x2, int y2);
  Ray(const Point& p1, const Point& p2);
  Ray(const Point& p, const Vector& v);
  Ray& Move(const Vector& v) override;
  bool ContainsPoint(const Point& c) const override;
  bool CrossesSegment(const Segment& s) const override;
  IShape* Clone() const override;
  std::string ToString() const override;
  double Distance(const Point& p) const;
};

struct Line : IShape {
  int64_t a = 0;
  int64_t b = 0;
  int64_t c = 0;
  Point point1;
  Point point2;
  Line(const Point& p1, const Point& p2);
  Line(int64_t a0, int64_t b0, int64_t c0);
  Line& Move(const Vector& v) override;
  Vector GuideVector() const;
  bool ContainsPoint(const Point& p) const override;
  bool CrossesSegment(const Segment& s) const override;
  IShape* Clone() const override;
  std::string ToString() const override;
  Line PerpendecularLine(const Point& p) const;
  double Distance(const Point& p) const;
};

struct Polygon : IShape {
  std::vector<Point> points;
  explicit Polygon(const std::vector<Point>& vect);
  Polygon(const Polygon& other);
  Polygon(Polygon&& other) noexcept;
  Polygon& operator=(Polygon&& other) noexcept;
  Polygon& Move(const Vector& v) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossesSegment(const Segment& s) const override;
  bool StrictlyContainsPoint(const Point& point) const;
  bool PointOnTheBorder(const Point& point) const;
  IShape* Clone() const override;
  std::string ToString() const override;
};

Vector::Vector(int64_t x1, int64_t y1, int64_t x2, int64_t y2) : x(x2 - x1), y(y2 - y1) {
}

Vector::Vector(int64_t x1, int64_t y1) : x(x1), y(y1) {
}

Vector::Vector(const Point& p1, const Point& p2) : x(p2.x - p1.x), y(p2.y - p1.y) {
}

Vector& Vector::operator+=(const Vector& other) {
  x += other.x;
  y += other.y;
  return *this;
}

Vector& Vector::operator-=(const Vector& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

Vector& Vector::operator*=(int64_t val) {
  x *= val;
  y *= val;
  return *this;
}

Vector& Vector::operator/=(int64_t val) {
  x /= val;
  y /= val;
  return *this;
}

double Vector::Len() const {
  return std::sqrt(x * x + y * y);
}

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

Point::Point(int64_t x1, int64_t y1) : x(x1), y(y1) {
}

Point::Point(const Point& p) : x(p.x), y(p.y) {
}

Point& Point::Move(const Vector& v) {
  x += v.x;
  y += v.y;
  return *this;
}

bool Point::ContainsPoint(const Point& p) const {
  return p == *this;
}

bool Point::CrossesSegment(const Segment& s) const {
  return s.ContainsPoint(*this);
}

IShape* Point::Clone() const {
  return new Point(*this);
}

std::string Point::ToString() const {
  return std::string("Point(") + std::to_string(x) + ", " + std::to_string(y) + ")";
}

Vector operator-(const Point& p1, const Point& p2) {
  return Vector(p2, p1);
}

bool operator==(const Point& p1, const Point& p2) {
  return (p1.x == p2.x) && (p1.y == p2.y);
}

Segment::Segment(int64_t x1, int64_t y1, int64_t x2, int64_t y2) : a(x1, y1), b(x2, y2) {
}

Segment::Segment(const Point& p1, const Point& p2) : a(p1), b(p2) {
}

Segment& Segment::Move(const Vector& v) {
  a.Move(v);
  b.Move(v);
  return *this;
}

bool Segment::ContainsPoint(const Point& p) const {
  Vector ap(a, p);
  Vector pb(p, b);
  return (ScalarProduct(ap, pb) >= 0 && VectorProduct(ap, pb) == 0);
}

bool Segment::CrossesSegment(const Segment& s) const {
  Line l1(a, b);
  Line l2(s.a, s.b);
  if (l1.ContainsPoint(s.a) && l1.ContainsPoint(s.b)) {
    return ContainsPoint(s.a) || ContainsPoint(s.b) || s.ContainsPoint(a);
  }
  return l1.CrossesSegment(s) && l2.CrossesSegment(*this);
}

IShape* Segment::Clone() const {
  return new Segment(*this);
}

std::string Segment::ToString() const {
  return "Segment(" + a.ToString() + ", " + b.ToString() + ")";
}

Ray::Ray(int x1, int y1, int x2, int y2) : point(x1, y1), guide(x2 - x1, y2 - y1) {
}

Ray::Ray(const Point& p1, const Point& p2) : point(p1), guide(p1, p2) {
}

Ray::Ray(const Point& p, const Vector& v) : point(p), guide(v) {
}

Ray& Ray::Move(const Vector& v) {
  point.Move(v);
  return *this;
}

bool Ray::ContainsPoint(const Point& c) const {
  Vector pc(point, c);
  return (ScalarProduct(guide, pc) >= 0 && VectorProduct(guide, pc) == 0);
}

bool Ray::CrossesSegment(const Segment& s) const {
  if (ContainsPoint(s.a) || ContainsPoint(s.b)) {
    return true;
  }
  if (!Line(point, Point(point).Move(guide)).CrossesSegment(s)) {
    return false;
  }
  Vector vector1 = Vector(s.a, point);
  Vector vector2 = Vector(s.a, s.b);
  int64_t vp_of_v1_v2 = VectorProduct(vector1, vector2);
  int64_t vp_of_guide_v2 = VectorProduct(guide, vector2);
  return ((vp_of_v1_v2 >= 0 && vp_of_guide_v2 < 0) || (vp_of_v1_v2 <= 0 && vp_of_guide_v2 > 0));
}

IShape* Ray::Clone() const {
  return new Ray(*this);
}

std::string Ray::ToString() const {
  return std::string("Ray(") + point.ToString() + ", " + "Vector(" + std::to_string(guide.x) + ", " +
         std::to_string(guide.y) + "))";
}

double Ray::Distance(const Point& p) const {
  if (ScalarProduct(guide, Vector(point, p)) > 0) {
    return std::fabs(VectorProduct(guide, Vector(point, p)) / guide.Len());
  }
  return Vector(point, p).Len();
}

Line::Line(const Point& p1, const Point& p2) {
  point1 = p1;
  point2 = p2;
  a = -(p2.y - p1.y);
  b = p2.x - p1.x;
  c = -(a * p1.x + b * p1.y);
}

Line::Line(int64_t a0, int64_t b0, int64_t c0) : a(a0), b(b0), c(c0) {
}

Line& Line::Move(const Vector& v) {
  *this = Line(point1.Move(v), point2.Move(v));
  return *this;
}

Vector Line::GuideVector() const {
  return Vector(b, -a);
}

bool Line::ContainsPoint(const Point& p) const {
  return (a * p.x + b * p.y + c == 0);
}

bool Line::CrossesSegment(const Segment& s) const {
  int64_t u = a * s.a.x + b * s.a.y + c;
  int64_t v = a * s.b.x + b * s.b.y + c;
  return (u <= 0 && v >= 0) || (u >= 0 && v <= 0);
}

IShape* Line::Clone() const {
  return new Line(*this);
}

std::string Line::ToString() const {
  return std::string("Line(") + std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + ")";
}

Line Line::PerpendecularLine(const Point& p) const {
  return Line(-b, a, b * p.x - a * p.y);
}

double Line::Distance(const Point& p) const {
  return std::fabs(double(a * p.x + b * p.y + c) / std::sqrt(a * a + b * b));
}

Polygon::Polygon(const std::vector<Point>& vect) {
  points = vect;
}

Polygon::Polygon(const Polygon& other) {
  points = other.points;
}

Polygon::Polygon(Polygon&& other) noexcept {
  points = std::move(other.points);
}

Polygon& Polygon::operator=(Polygon&& other) noexcept {
  points = std::move(other.points);
  return *this;
}

Polygon& Polygon::Move(const Vector& v) {
  for (size_t i = 0; i < points.size(); ++i) {
    points[i].Move(v);
  }
  return *this;
}

Point LowerPoint(const Point& p1, const Point& p2) {
  return (p1.y <= p2.y ? p1 : p2);
}

bool Polygon::ContainsPoint(const Point& point) const {
  Ray ray(point, Vector(1, 0));
  size_t count = 0;
  for (size_t i = 0; i < points.size(); ++i) {
    Segment s(points[i], points[(i + 1) % points.size()]);
    if (s.ContainsPoint(point)) {
      return true;
    }
    if (ray.CrossesSegment(s)) {
      if (!ray.ContainsPoint(LowerPoint(s.a, s.b))) {
        ++count;
      }
    }
  }
  return (count % 2 == 1);
}

bool Polygon::PointOnTheBorder(const Point& point) const {
  for (size_t i = 0; i < points.size(); ++i) {
    if (Segment(points[i], points[(i + 1) % points.size()]).ContainsPoint(point)) {
      return true;
    }
  }
  return false;
}

bool Polygon::CrossesSegment(const Segment& s) const {
  for (size_t i = 1; i < points.size() + 1; ++i) {
    if (Segment(points[i - 1], points[i % points.size()]).CrossesSegment(s)) {
      return true;
    }
  }
  return false;
}

IShape* Polygon::Clone() const {
  return new Polygon(*this);
}

std::string Polygon::ToString() const {
  std::string str = "Polygon(";
  for (size_t i = 0; i < points.size(); ++i) {
    str += points[i].ToString();
    if (i != points.size() - 1) {
      str += ", ";
    }
  }
  return (str + ")");
}

class PointStack {
  struct Node {
    Point val;
    Node* prev = nullptr;
  };
  Node* head_ = nullptr;

 public:
  PointStack() = default;
  ~PointStack() {
    while (head_) {
      Pop();
    }
  }
  void Push(const Point& point) {
    auto new_head = new Node;
    new_head->val = point;
    new_head->prev = head_;
    head_ = new_head;
  }
  Point Pop() {
    Node* prev_head = head_;
    Point point = head_->val;
    head_ = head_->prev;
    delete prev_head;
    return point;
  }
  const Point& First() {
    return head_->val;
  }
  const Point& Second() {
    return head_->prev->val;
  }
  bool ContainsSecond() {
    return head_->prev != nullptr;
  }
  bool Empty() {
    return head_ == nullptr;
  }
};

Polygon ConvexHull(std::vector<Point> points) {
  int64_t x, y, x_min = points[0].x, y_min = points[0].y;
  size_t idx = 0, n = points.size();
  for (size_t i = 0; i < n; ++i) {
    x = points[i].x;
    y = points[i].y;
    if (y < y_min) {
      idx = i;
      y_min = y;
      x_min = x;
    } else if (y == y_min && x < x_min) {
      idx = i;
      x_min = x;
    }
  }
  Point first(x_min, y_min);
  points[idx] = points[--n];
  std::sort(points.data(), points.data() + n, [first](const Point& p1, const Point& p2) {
    Vector v1(first, p1);
    Vector v2(first, p2);
    return ((VectorProduct(v1, v2) > 0) || ((VectorProduct(v1, v2) == 0) && VectorLenCmp(v1, v2)));
  });
  PointStack stack;
  stack.Push(first);
  for (idx = 0; idx < n; ++idx) {
    while (stack.ContainsSecond() &&
           VectorProduct(Vector(stack.Second(), stack.First()), Vector(stack.First(), points[idx])) <= 0) {
      stack.Pop();
    }
    stack.Push(points[idx]);
  }
  x_min = 10000000000LL, y_min = 10000000000LL;
  for (n = 0; !stack.Empty(); ++n) {
    if ((stack.First().x < x_min) || ((stack.First().x == x_min) && (stack.First().y < y_min))) {
      idx = n;
      x_min = stack.First().x;
      y_min = stack.First().y;
    }
    points[n] = stack.Pop();
  }
  for (size_t i = idx; i > 0;) {
    stack.Push(points[--i]);
  }
  for (size_t i = n; i > idx;) {
    stack.Push(points[--i]);
  }
  for (size_t i = 0; i < n; ++i) {
    points[i] = stack.Pop();
  }
  while (points.size() > n) {
    points.pop_back();
  }
  Polygon polygon(points);
  return polygon;
}

std::vector<int> Infection(std::vector<Point>& social, std::vector<Point>& scientific) {
  size_t n = scientific.size();
  std::vector<int> ans;
  for (size_t i = 0; i < n; ++i) {
    ans.emplace_back(-1);
  }
  size_t zone = 0;
  Polygon polygon(ConvexHull(social));
  for (size_t i = 0; i < social.size(); ++i) {
    if (polygon.PointOnTheBorder(social[i])) {
      std::swap(social[i--], social[social.size() - 1]);
      social.pop_back();
    }
  }
  while (social.size() > 2) {
    polygon = ConvexHull(social);
    if (polygon.points.size() < 3) {
      break;
    }
    for (size_t i = 0; i < scientific.size(); ++i) {
      if (ans[i] == -1 && !polygon.ContainsPoint(scientific[i])) {
        ans[i] = zone;
      }
    }
    for (size_t i = 0; i < social.size(); ++i) {
      if (polygon.PointOnTheBorder(social[i])) {
        std::swap(social[i--], social[social.size() - 1]);
        social.pop_back();
      }
    }
    ++zone;
  }
  for (size_t i = 0; i < n; ++i) {
    if (ans[i] == -1) {
      ans[i] = zone;
    }
  }
  return ans;
}

int main() {
  std::vector<Point> social;
  size_t n;
  std::cin >> n;
  for (size_t i = 0; i < n; ++i) {
    Point point;
    std::cin >> point.x >> point.y;
    social.push_back(point);
  }
  std::cin >> n;
  std::vector<Point> scientific;
  for (size_t i = 0; i < n; ++i) {
    Point point;
    std::cin >> point.x >> point.y;
    scientific.push_back(point);
  }
  std::vector<int> ans(Infection(social, scientific));
  for (size_t i = 0; i < n; ++i) {
    std::cout << ans[i] << '\n';
  }
  return 0;
}
