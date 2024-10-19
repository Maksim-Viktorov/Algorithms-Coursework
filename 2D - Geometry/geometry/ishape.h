#ifndef ISHAPE
#define ISHAPE

#include <string>

namespace geometry {

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
}
#endif // ISHAPE
