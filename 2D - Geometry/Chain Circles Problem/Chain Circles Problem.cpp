#include <iostream>
#include <vector>

struct int128 {
  uint64_t lo;
  int64_t hi;
};

bool operator<=(const int128& a, const int128& b) {
  int64_t hi = a.hi - b.hi - (a.lo < b.lo);
  return hi < 0 || (hi == 0 && a.lo == b.lo);
}

int128 Mul64(int64_t a, int64_t b) {
  int sign = +1;
  if (a < 0) {
    a = -a;
    sign = -sign;
  }
  if (b < 0) {
    b = -b;
    sign = -sign;
  }
  uint64_t alo = (uint32_t)a;
  uint64_t ahi = (uint32_t)(a >> 32);
  uint64_t blo = (uint32_t)b;
  uint64_t bhi = (uint32_t)(b >> 32);
  uint64_t lo = alo * blo;
  uint64_t mid = ahi * blo + bhi * alo + (lo >> 32);
  uint64_t hi = ahi * bhi + (mid >> 32);
  lo = (mid << 32) + (uint32_t)lo;
  if (sign < 0) {
    lo = ~lo + 1;
    hi = ~hi + (lo == 0);
  }
  return int128{ lo, (int64_t)hi };
}

int128 Sqr64(int64_t a) {
  return Mul64(a, a);
}

struct Point {
  int x;
  int y;
};

struct Circle {
  Point o;
  int r;
};

struct Segment {
  Point a;
  Point b;
  Segment(const Point& aa, const Point& bb) : a(aa), b(bb) {
  }
};

struct Vector {
  int x;
  int y;
  Vector(int xx, int yy) : x(xx), y(yy) {
  }
  Vector(const Point& a, const Point& b) : x(b.x - a.x), y(b.y - a.y) {
  }
  Vector operator-() const {
    return Vector(-x, -y);
  }
};

int64_t DotProduct(const Vector& u, const Vector& v) {
  return (int64_t)u.x * v.x + (int64_t)u.y * v.y;
}

int64_t CrossProduct(const Vector& u, const Vector& v) {
  return (int64_t)u.x * v.y - (int64_t)u.y * v.x;
}

bool CircleReachesSegment(const Circle& circle, const Segment& seg) {
  int64_t rr = (int64_t)circle.r * circle.r;
  Vector ao(seg.a, circle.o);
  Vector ab(seg.a, seg.b);
  if (DotProduct(ao, ab) <= 0) {
    return DotProduct(ao, ao) <= rr;
  }
  Vector bo(seg.b, circle.o);
  if (DotProduct(bo, -ab) <= 0) {
    return DotProduct(bo, bo) <= rr;
  }
  return Sqr64(CrossProduct(ab, ao)) <= Mul64(rr, DotProduct(ab, ab));
}

bool CircleReachesCircle(const Circle& circle, const Circle& other) {
  Vector oo(circle.o, other.o);
  int64_t d = circle.r + other.r;
  return DotProduct(oo, oo) <= d * d;
}

bool ChainCircles(const Segment& start, const Segment& finish, std::vector<Circle>& circles) {
  size_t n = circles.size();
  size_t m = 0;
  for (size_t i = 0; i < n; ++i) {
    if (CircleReachesSegment(circles[i], start)) {
      if (CircleReachesSegment(circles[i], finish)) {
        return true;
      }
      std::swap(circles[m++], circles[i]);
    }
  }
  size_t l = 0;
  for (size_t k = m; k > l; ) {
    for (size_t i = l; i < m; ++i) {
      for (size_t j = k; j < n; ++j) {
        if (CircleReachesCircle(circles[i], circles[j])) {
          if (CircleReachesSegment(circles[j], finish)) {
            return true;
          }
          std::swap(circles[k++], circles[j]);
        }
      }
    }
    l = m;
    m = k;
  }
  return false;
}

void SortConvexHull(std::vector<Point>& points) {
  size_t n = points.size();
  for (size_t i = 1; i < n; ++i) {
    if (points[i].x < points[0].x ||
      (points[i].x == points[0].x && points[i].y < points[0].y)) {
      std::swap(points[0], points[i]);
    }
  }
  for (size_t i = 1; i < n; ++i) {
    Vector prev(points[i - 1], points[i]);
    for (size_t j = i + 1; j < n; ++j) {
      Vector next(points[i - 1], points[j]);
      if (CrossProduct(prev, next) > 0) {
        prev = next;
        std::swap(points[i], points[j]);
      }
      else if (CrossProduct(prev, next) == 0 &&
        DotProduct(next, next) < DotProduct(prev, prev)) {
        prev = next;
        std::swap(points[i], points[j]);
      }
    }
  }
}

int main() {
  std::vector<Point> points(4);
  for (size_t i = 0; i < 4; ++i) {
    std::cin >> points[i].x >> points[i].y;
  }
  SortConvexHull(points);

  size_t n;
  std::cin >> n;
  std::vector<Circle> circles(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> circles[i].o.x >> circles[i].o.y >> circles[i].r;
  }
  if (ChainCircles(Segment(points[0], points[1]), Segment(points[2], points[3]), circles) &&
    ChainCircles(Segment(points[1], points[2]), Segment(points[3], points[0]), circles)) {
    std::cout << "NO\n";
  }
  else {
    std::cout << "YES\n";
  }
  return 0;
}