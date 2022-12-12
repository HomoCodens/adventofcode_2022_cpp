#include "./vector2d.hpp"

bool operator==(const Vector2D& a, const Vector2D& b) {
    return a.x == b.x && a.y == b.y;
}

Vector2D operator+(const Vector2D& a, const Vector2D& b) {
    return Vector2D{a.x + b.x, a.y + b.y};
}

Vector2D operator-(const Vector2D& a, const Vector2D& b) {
    return Vector2D{a.x - b.x, a.y - b.y};
}

Vector2D operator*(const int& magnitude, const Vector2D& v) {
    return Vector2D{magnitude*v.x, magnitude*v.y};
}

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << "Vector2D(" << v.x << ", " << v.y << ")";

    return os;
}

uint manhattanDist(const Vector2D& a, const Vector2D& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}