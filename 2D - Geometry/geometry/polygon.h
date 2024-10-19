#ifndef POLYGON
#define POLYGON

#include "ishape.h"

namespace geometry {

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
}
#endif  // POLYGON
