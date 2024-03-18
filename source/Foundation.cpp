#include "Foundation.h"

float RandomFloat(float min, float max) {
	std::random_device m_seed;
	std::mt19937 m_mt19937 = std::mt19937(m_seed());
	std::uniform_real_distribution<float> distribution(min, max);

	return static_cast<float>(distribution(m_mt19937));
}

std::vector<Vector> RandomVector(size_t size) {
	float half_width = 600.f;
	float half_height = 400.f;

	std::vector<Vector> vex(size);
	for (auto& v : vex) {
		v = Vector{ RandomFloat(-half_width, half_width), RandomFloat(-half_height, half_height), 0, 1 };
	}

	return vex;
}

float Math::Clamp(float a, float min, float max){
	if (min > max) {
		float t = min;
		min = max;
		max = t;
	}

	if (a < min) return min;
	if (a > max) return max;
	return a;
}

bool Math::ClaculatePerpendicularBisector(Point p1, Point p2, float& k, float& b){ 
	Point mid = (p1 + p2) / 2.0f;

#ifndef _DEBUG
	std::cout << "Math::ClaculatePerpendicularBisector: " 
		<< p1.x << " " << p2.x << " " << mid.x << "\n";
	std::cout << "Math::ClaculatePerpendicularBisector: "
		<< p1.y << " " << p2.y << " " << mid.y << "\n\n";
#endif

	if (p1.x == p2.x) {
		k = 0.0f; b = mid.y;
		return true;
	}
	if (p1.y == p2.y) {
		k = 0.0f; b = mid.x;
		return false;
	}

	k = (p1.y - p2.y) / (p1.x - p2.x);
	k = -1.0f / k;
	b = mid.y - k * mid.x;

	return true;
}

Point Math::LineIntersection(float k1, float b1, float k2, float b2) {
	float x, y;
	x = (b2 - b1) / (k1 - k2);
	y = x * k1 + b1;
	return Vector{ x, y, 0, 1 };
}

float Math::Length(Vector v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}