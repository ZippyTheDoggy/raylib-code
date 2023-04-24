#pragma once

#include "../include/raylib.h"

Vector3 operator+(Vector3 a, Vector3 b) {
    return Vector3 {a.x + b.x, a.y + b.y, a.z + b.z};
};
Vector3 operator-(Vector3 a, Vector3 b) {
    return Vector3 {a.x - b.x, a.y - b.y, a.z - b.z};
};
Vector3 operator*(Vector3 a, Vector3 b) {
    return Vector3 {a.x * b.x, a.y * b.y, a.z * b.z};
};
Vector3 operator*(Vector3 a, float b) {
    return a * Vector3 {b, b, b};
};
Vector3 operator/(Vector3 a, Vector3 b) {
    return Vector3 {a.x / b.x, a.y / b.y, a.z / b.z};
};
Vector3 operator/(Vector3 a, float b) {
    return a / Vector3 {b, b, b}; 
};
void operator+=(Vector3& a, Vector3 b) {
    a = a + b;
};
void operator-=(Vector3& a, Vector3 b) {
    a = a - b;
};
void operator*=(Vector3& a, Vector3 b) {
    a = a * b;
};
void operator*=(Vector3& a, float b) {
    a = a * b;
};
void operator/=(Vector3& a, Vector3 b) {
    a = a / b;
};
void operator/=(Vector3& a, float b) {
    a = a / b;
};