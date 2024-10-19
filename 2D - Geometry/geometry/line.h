#ifndef LINE
#define LINE

#include "ishape.h"
#include "point.h"
#include "line.h"

namespace geometry {

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
}
#endif  // LINE
