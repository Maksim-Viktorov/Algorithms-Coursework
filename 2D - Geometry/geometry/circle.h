#ifndef CIRCLE
#define CIRCLE

#include "ishape.h"
#include "point.h"

namespace geometry {

struct Circle : IShape {
  Point point;
  int64_t radius;
  Circle(const Point& p, int64_t r);
  Circle& Move(const Vector& v) override;
  bool ContainsPoint(const Point& p) const override;
  bool CrossesSegment(const Segment& s) const override;
  IShape* Clone() const override;
  std::string ToString() const override;
};
}
#endif  // CIRCLE
