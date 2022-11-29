#include "vector2d.h"
#include <math.h>

Vector2D::Vector2D() {
    x = 0;
    y = 0;
}

Vector2D::Vector2D(float a, float b) {
    x = a;
    y = b;
}

Vector2D Vector2D::add(Vector2D other) {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::sub(Vector2D other) {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::mult(float other) {
    return Vector2D(x * other, y * other);
}

Vector2D Vector2D::div(float other) {
    return Vector2D(x / other, y / other);
}

float Vector2D::magnitude() {
    return sqrt(pow(x, 2) + pow(y, 2));
}

void Vector2D::set(float a, float b) {
    x = a;
    y = b;
}

void Vector2D::reset() {
    x = 0;
    y = 0;
}
