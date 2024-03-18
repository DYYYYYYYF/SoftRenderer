#pragma once

#include "Foundation.h"
#include "Renderer.hpp"

class VoronoiDiagram {
public:
	struct Edge {
		Edge() {}
		Edge(size_t vv1, size_t vv2);

	public:
		bool GetLine(float& k, float& b);
		void Draw();
		void DrawBisector(float x1, float x2);

	private:
		size_t v1, v2;
		float k = 0.0f, b = 0.0f;
		bool exist_k = false;
	};

	struct Triangle {
		
		enum DrawType {
			eDrawTriangle = 0 | 1 << 0,
			eDrawCircle = 0 | 1 << 1
		};

		Triangle() {}
		Triangle(size_t vv1, size_t vv2, size_t vv3);

		void DrawDebug(int type) {
			if (type & DrawType::eDrawTriangle) DrawTriangle();
			if (type & DrawType::eDrawCircle) DrawCircle();
		}

	private:
		void GenerateOutCircle();
		void DrawTriangle();
		void DrawCircle();

	private:
		// Site Index
		size_t v1, v2, v3;
		
		// Out-Circle
		float r;
		Point center;
		std::array<Edge, 3> edges;
	};

public:
	static void GenerateVoronoi(Renderer* renderer, size_t size = 10);

private:
	static void GenerateAABB();
	static void DrawAABB();
	static void Triangulation();

private:
	static Renderer* renderer;
	static std::array<Vector, 4> aabb;
	static std::vector<Vector> sites;
	static std::vector<Triangle> triangles;
};