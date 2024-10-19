#ifndef RAY
#define RAY

#include "ishape.h"
#include "vector.h"

namespace geometry {

struct Ray : IShape {
  Point point;
  Vector guide;
  Ray(int x1, int y1, int x2, int y2);
  Ray(const Point& p1, const Point& p2);
  Ray& Move(const Vector& v) override;
  bool ContainsPoint(const Point& c) const override;
  bool CrossesSegment(const Segment& s) const override;
  IShape* Clone() const override;
  std::string ToString() const override;
  double Distance(const Point& p) const;
};
}
#endif  // RAY
