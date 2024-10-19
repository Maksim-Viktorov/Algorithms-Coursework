#ifndef POINT
#define POINT

#include "ishape.h"

namespace geometry {

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
}
#endif  // POINT
