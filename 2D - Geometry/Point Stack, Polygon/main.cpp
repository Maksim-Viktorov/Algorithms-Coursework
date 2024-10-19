#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

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
  size_t angles_num = 0;
  Point* points;
  Polygon(Point* data, size_t size) {
    angles_num = size;
    points = new Point[size];
    for (size_t i = 0; i < size; ++i) {
      points[i] = data[i];
    }
  }
  Polygon(Polygon&& other) noexcept {
    angles_num = other.angles_num;
    points = other.points;
    other.points = nullptr;
  }
  ~Polygon() {
    delete[] points;
  }
  Polygon& operator=(Polygon&& other) noexcept {
    angles_num = other.angles_num;
    delete[] points;
    points = other.points;
    other.points = nullptr;
    return *this;
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

Polygon ConvexHull(Point* points, size_t n) {
  int64_t x, y, x_min = 10000000000LL, y_min = 10000000000LL;
  size_t idx;
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
  std::sort(points, points + n, [first](const Point& p1, const Point& p2) {
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
  Polygon polygon(points, n);
  delete[] points;
  return polygon;
}

int main() {
  size_t n;
  int64_t x, y;
  std::cin >> n;
  auto points = new Point[n];
  for (size_t i = 0; i < n; ++i) {
    std::cin >> x >> y;
    points[i] = Point(x, y);
  }
  Polygon polygon = ConvexHull(points, n);
  n = polygon.angles_num;
  std::cout << n << '\n';
  for (size_t i = 0; i < n; ++i) {
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
