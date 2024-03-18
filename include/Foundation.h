#pragma once

#include <random>
#include <Defines.h>

float RandomFloat(float min, float max);
std::vector<Vector> RandomVector(size_t size);

class Math {
public:
	static bool ClaculatePerpendicularBisector(Point p1, Point p2, float& k, float& b);
	static float Clamp(float a, float min, float max);

	// Geometry
	static Point LineIntersection(float k1, float b1, float k2, float b2);

	// Linear Algebra
	static float Length(Vector v);
};

struct Vertex {
	Vertex() {}
	Vertex(Vector pos) { position = pos; }

	Vector4 position = Vector4(0, 0, 0, 1);
	Vector3 normal;
	Color color = COLOR_WHITE;
};

struct Mesh {
public:
	struct Transform {
		Vector3 translate = Vector3(0, 0, 0);
		Vector3 scale = Vector3(1, 1, 1);
		Vector3 rotate = Vector3(0, 0, 0);

		friend std::ostream& operator<<(std::ostream& os, Transform t) {
			return os << t.translate.x << " " << t.translate.y << " " << t.translate.x << "\n"
				<< t.scale.x << " " << t.scale.y << " " << t.scale.x << "\n"
				<< t.rotate.x << " " << t.rotate.y << " " << t.rotate.x << "\n";
		}
	};

public:
	std::vector<Vertex>& GetVertices() { return vertices; }

	void SetTopologyMode(Topology t) { topology = t; }
	Topology GetTopologyMode() const { return topology; }

	void SetTranslate(Vector3 t) { transform.translate += t; }
	Vector3 GetTranslate() const { return transform.translate; }

	void SetScale(Vector3 s) { transform.scale = s; }
	void SetScale(float s) { transform.scale = Vector3(s, s, s); }
	Vector3 GetScale() const { return transform.scale; }

	void SetRotate(Vector3 r) { transform.rotate += r; }
	Vector3 GetRotate() const { return transform.rotate; }

	void SetTransform(Transform t) { transform = t; }
	Transform GetTransform() const { return transform; }

	Matrix4 GetMatrixModel() const {
		return Matrix4{
			transform.scale.x, 0, 0, transform.translate.x,
			0, transform.scale.y, 0, transform.translate.y,
			0, 0, transform.scale.z, transform.translate.z,
			0, 0, 0, 1
		};
	}

protected:
	std::vector<Vertex> vertices;
	Topology topology = Topology::eTriangle;
	Transform transform;
};
