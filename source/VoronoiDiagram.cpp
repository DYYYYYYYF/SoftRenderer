#include "VoronoiDiagram.h"

Renderer* VoronoiDiagram::renderer = nullptr;
std::array<Vector, 4> VoronoiDiagram::aabb;
std::vector<Vector> VoronoiDiagram::sites = std::vector<Vector>(0);
std::vector<VoronoiDiagram::Triangle> VoronoiDiagram::triangles = std::vector<VoronoiDiagram::Triangle>(0);

void VoronoiDiagram::GenerateVoronoi(Renderer* r, size_t size) {
	renderer = r;
	
	sites.resize(0);
	triangles.resize(0);
	
	sites = RandomVector(size);
	for (auto vec : sites) renderer->DrawCircle(vec, 5, false);

	std::sort(sites.begin(), sites.end(), [](Vector a, Vector b) {
		return a.x > b.x;
		});

	Triangulation();

	for (VoronoiDiagram::Triangle triangle : triangles) {
		triangle.DrawDebug(Triangle::DrawType::eDrawTriangle |
			Triangle::DrawType::eDrawCircle);
	}
}


void VoronoiDiagram::Triangulation() {
	GenerateAABB();

	// Init super triangle
	size_t aabb_index = sites.size() - 4;
	Triangle tri0(aabb_index, aabb_index + 1, aabb_index + 2);
	Triangle tri1(aabb_index, aabb_index + 2, aabb_index + 3);

	//triangles.push_back(tri0);
	//triangles.push_back(tri1);

	// Insert site and generate delunay triangle
	for (int i = 0; i < sites.size(); i++) {
		
	}
	Triangle triangle(0, 1, 2);
	triangles.push_back(triangle);
	//DrawAABB();
}

void VoronoiDiagram::DrawAABB() {
	renderer->DrawLine(aabb[0], aabb[1]);
	renderer->DrawLine(aabb[1], aabb[2]);
	renderer->DrawLine(aabb[2], aabb[3]);
	renderer->DrawLine(aabb[0], aabb[3]);
}

void VoronoiDiagram::GenerateAABB() {
	float xMin = 65536, yMin = 65536;
	float xMax = -65536, yMax = -65536;

	for (auto p : sites) {
		if (p.x < xMin) xMin = p.x;
		if (p.y < yMin) yMin = p.y;

		if (p.x > xMax) xMax = p.x;
		if (p.y > yMax) yMax = p.y;
	}

	aabb[0] = Vector{ xMin, yMin, 0, 1 };
	aabb[1] = Vector{ xMin, yMax, 0, 1 };
	aabb[2] = Vector{ xMax, yMax, 0, 1 };
	aabb[3] = Vector{ xMax, yMin, 0, 1 };

	sites.push_back(aabb[0]);
	sites.push_back(aabb[1]);
	sites.push_back(aabb[2]);
	sites.push_back(aabb[3]);
}

// Struct
VoronoiDiagram::Edge::Edge(size_t vv1, size_t vv2) {
	this->v1 = vv1;
	this->v2 = vv2;

	Point p1 = Point{ sites[v1].x, sites[v1].y, 1 };
	Point p2 = Point{ sites[v2].x, sites[v2].y, 1 };

	exist_k = Math::ClaculatePerpendicularBisector(p1, p2, k, b);
}

void VoronoiDiagram::Edge::Draw() {
	renderer->DrawLine(Vector{ sites[v1].x, sites[v1].y, 0, 1 },
		Vector{ sites[v2].x, sites[v2].y, 0, 1 });
}

void VoronoiDiagram::Edge::DrawBisector(float x1, float x2) {

	if (!exist_k) {
		renderer->DrawLine(Vector{ b, 600, 0, 1 },
			Vector{ b,-600, 0, 1 });
		return;
	}

	float y1 = x1 * k + b;
	float y2 = x2 * k + b;

	renderer->DrawLine(Vector{ x1, y1, 0, 1 },
		Vector{ x2, y2, 0, 1 });
}

bool VoronoiDiagram::Edge::GetLine(float& k, float& b) {
	k = this->k;
	b = this->b;

	return this->exist_k;
}

VoronoiDiagram::Triangle::Triangle(size_t vv1, size_t vv2, size_t vv3) {
	v1 = vv1;
	v2 = vv2;
	v3 = vv3;

	edges[0] = Edge(vv1, vv2);
	edges[1] = Edge(vv2, vv3);
	edges[2] = Edge(vv1, vv3);

	GenerateOutCircle();
}

void VoronoiDiagram::Triangle::GenerateOutCircle() {
	float k1, k2;
	float b1, b2;
	bool is_valued;

	// check k's valid
	is_valued = edges[0].GetLine(k1, b1);
	if (!is_valued) edges[2].GetLine(k1, b1);

	is_valued = edges[1].GetLine(k2, b2);
	if (!is_valued) edges[2].GetLine(k1, b1);

	// triange do have two intersect lines
	center = Math::LineIntersection(k1, b1, k2, b2);
	r = Math::Length(Vector{ center , 1 } - sites[v1]);
}

void VoronoiDiagram::Triangle::DrawTriangle() {
	renderer->DrawLine(sites[v1], sites[v2]);
	renderer->DrawLine(sites[v1], sites[v3]);
	renderer->DrawLine(sites[v2], sites[v3]);
}

void VoronoiDiagram::Triangle::DrawCircle() {
	edges[0].DrawBisector(-1200.f, 1200.f);
	edges[1].DrawBisector(-1200.f, 1200.f);
	edges[2].DrawBisector(-1200.f, 1200.f);

	renderer->DrawCircle(Vector{ center, 1 }, 5);	// Center
	renderer->DrawCircle(Vector{ center, 1 }, r, false);	// Circle
}