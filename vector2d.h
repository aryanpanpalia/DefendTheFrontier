#pragma once

class Vector2D {
    public:
        float x, y;

        Vector2D();
        Vector2D(float a, float b);
        Vector2D add(Vector2D other);
        Vector2D sub(Vector2D other);
        Vector2D mult(float other);
        Vector2D div(float other);
        float magnitude();
        Vector2D norm();
        void set(float a, float b);
        void reset();
};
