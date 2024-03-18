#pragma once

#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>

#define COLOR_WHITE Color(255, 255, 255)
#define COLOR_BLACK Color(0, 0, 0)
#define COLOR_RED Color(255, 0, 0)
#define COLOR_GREEN Color(0, 255, 0)
#define COLOR_BLUE Color(0, 0, 255)

typedef glm::ivec2 iVec2;
typedef glm::vec2 Vector2;
typedef glm::vec3 Vector3;
typedef glm::vec4 Vector4;
typedef glm::mat4 Matrix4;
typedef glm::mat3 Matrix3;

typedef Vector4 Vector;
typedef Vector3 Color;
typedef Vector3 Point;

enum Topology : char {
	eLine = 0,
	eLineList,
	eTriangle,
	eTopology_Max
};

enum KeyBoard : char {
	eKey_A = 97,
	eKey_B = 98,
	eKey_C = 99,
	eKey_W = 119,
	eKey_D = 100,
	eKey_S = 115,
	eKey_ESC = 27,
	eKeyBoard_Max
};
