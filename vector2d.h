#pragma once

/*
    Vector2D class

    Contains variables that stores:
        x: the x component of the vector
        y: the y component of the vector

    Contains methods that:
        Vector2D(): creates a Vector2D object
        Vector2D(float, float): creates a Vector2D object based on arguments
        add(Vector2D): returns a Vector2D object equaling the sum of this Vector2D and the one passed as an argument
        sub(Vector2D): returns a Vector2D object equaling the difference of this Vector2D and the one passed as an argument
        mult(float): returns a Vector2D object equaling the product of this Vector2D and number passed as an argument
        div(float): returns a Vector2D object equaling the quotient of this Vector2D and number passed as an argument
        magnitude(): returns the magnitude of the vector
        norm(): returns the norm of the vector
        set(float, float): sets the x and y components to those passed as arguments
        reset(): resets the x and y components to 0
*/
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
