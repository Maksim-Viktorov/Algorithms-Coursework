#ifndef SEGMENT
#define SEGMENT

#include "ishape.h"

namespace geometry {

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
}
#endif  // SEGMENT
