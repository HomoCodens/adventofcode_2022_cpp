#include <functional>
#include <iostream>

struct Vector2D {
    int x{0};
    int y{0};
};

struct PointHasher {
    size_t operator()(const Vector2D& x) const {
        auto hasher = std::hash<int>{};

        auto h1 = hasher(x.x);
        auto h2 = hasher(x.y);

        if(h1 != h2) {
            return h1 ^ h2;
        }
        return h1;
    }
};

bool operator==(const Vector2D& a, const Vector2D& b);
Vector2D operator+(const Vector2D& a, const Vector2D& b);
Vector2D operator-(const Vector2D& a, const Vector2D& b);
Vector2D operator*(const int& magnitude, const Vector2D& v);
std::ostream& operator<<(std::ostream& os, const Vector2D& v);
uint manhattanDist(const Vector2D& a, const Vector2D& b);