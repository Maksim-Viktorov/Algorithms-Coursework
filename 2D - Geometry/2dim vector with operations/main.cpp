#include <cmath>
#include <iomanip>
#include <iostream>


struct Point {
  int x = 0;
  int y = 0;
  Point(int x1, int y1) : x(x1), y(y1) {
  }
};

struct Vector {
  int x;
  int y;
  Vector(int x1, int y1, int x2, int y2) : x(x2 - x1), y(y2 - y1) {
  }
  Vector(int x1, int y1) : x(x1), y(y1) {
  }
  double Len() const {
    return std::sqrt(x * x + y * y);
  }
};

Vector operator+(const Vector& v1, const Vector& v2) {
  return Vector(v1.x + v2.x, v1.y + v2.y);
}

int ScalarProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

int VectorProduct(const Vector& v1, const Vector& v2) {
  return v1.x * v2.y - v2.x * v1.y;
}

int main() {
  /*int x1, x2, y1, y2;
  std::cin >> x1 >> y1 >> x2 >> y2;
  Vector a(x1, y1, x2, y2);
  std::cin >> x1 >> y1 >> x2 >> y2;
  Vector b(x1, y1, x2, y2);
  std::cout << std::fixed << std::setprecision(6);
  std::cout << a.Len() << ' ' << b.Len() << '\n';
  std::cout << double((a + b).x) << ' ' << double((a + b).y) << '\n';
  std::cout << double(ScalarProduct(a, b)) << ' ' << double(VectorProduct(a, b)) << '\n';
  std::cout << std::fabs(VectorProduct(a, b) / 2.0);*/
  return 0;
}
