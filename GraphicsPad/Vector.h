#pragma once

struct  Vector2
{
	float x, y;
	Vector2 operator+(Vector2 other) { return { x + other.x, y + other.y }; }
	Vector2 operator*(Vector2 other) { return { x * other.x, y * other.y }; }
	Vector2 operator*(float val) { return { x * val, y * val }; }
};

struct Vector3
{
	float x, y, z;
};

struct Vertex {
	Vector2 position;
	Vector3 color;
};