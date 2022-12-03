#include "vector2d.h"
#include <math.h>

/*
    Creates a Vector2D object

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia
*/
Vector2D::Vector2D() {
    x = 0;
    y = 0;
}

/*
    Creates a Vector2D object

    Parameters:
        a: the x component of the vector
        b: the y component of the vector

    Return value: none

    Authors: Aryan Panpalia
*/
Vector2D::Vector2D(float a, float b) {
    x = a;
    y = b;
}

/*
    Returns a Vector2D object equaling the sum of this Vector2D and the one passed as an argument

    Parameters:
        other: the other Vector2D object to add

    Return value: the sum of this and other

    Authors: Aryan Panpalia
*/
Vector2D Vector2D::add(Vector2D other) {
    return Vector2D(x + other.x, y + other.y);
}

/*
    Returns a Vector2D object equaling the difference of this Vector2D and the one passed as an argument

    Parameters:
        other: the other Vector2D object to subtract

    Return value: the difference of this and other

    Authors: Aryan Panpalia
*/
Vector2D Vector2D::sub(Vector2D other) {
    return Vector2D(x - other.x, y - other.y);
}

/*
    Returns a Vector2D object equaling the product of this Vector2D and number passed as an argument

    Parameters:
        other: the number to multiply with this Vector2D

    Return value: the product of this and other

    Authors: Aryan Panpalia
*/
Vector2D Vector2D::mult(float other) {
    return Vector2D(x * other, y * other);
}

/*
    Returns a Vector2D object equaling the quotient of this Vector2D and number passed as an argument

    Parameters:
        other: the number to divide this Vector2D by

    Return value: the quotient of this and other

    Authors: Aryan Panpalia
*/
Vector2D Vector2D::div(float other) {
    return Vector2D(x / other, y / other);
}

/*
    Returns the magnitude of the vector

    Parameters: none
    Return value: magnitude of the vector

    Authors: Aryan Panpalia
*/
float Vector2D::magnitude() {
    return sqrt(pow(x, 2) + pow(y, 2));
}

/*
    Returns the norm of the vector

    Parameters: none
    Return value: the norm of the vector

    Authors: Aryan Panpalia
*/
Vector2D Vector2D::norm() {
    return div(magnitude());
}

/*
    Sets the x and y components to those passed as arguments

    Parameters:
        a: the new x component
        b: the new y component

    Return value: none

    Authors: Aryan Panpalia
*/
void Vector2D::set(float a, float b) {
    x = a;
    y = b;
}

/*
    Resets the x and y components to 0

    Parameters: none
    Return value: none

    Authors: Aryan Panpalia
*/
void Vector2D::reset() {
    x = 0;
    y = 0;
}
