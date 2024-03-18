#include "Renderer.hpp"

std::function<void()> Renderer::BeiginRenderCallback;
std::function<void()> Renderer::EndRenderCallback;

Renderer::Renderer() : Width(1200), Height(600){
	ClearColorValue = Color(0, 0, 0);
}

void Renderer::RegisterBeginRenderCallBack(std::function<void()> func) {
	BeiginRenderCallback = std::move(func);
}

void Renderer::RegisterEndRenderCallBack(std::function<void()> func) {
	EndRenderCallback = std::move(func);
}

bool Renderer::Init() {
	RenderTarget = new Buffer(Width, Height);
	Screen = cv::Mat(cv::Size(Width, Height), CV_8UC3, RenderTarget->Data());
	ClearColor();

	return true;
}

void Renderer::Render() {
	BeiginRenderCallback();

	unsigned char* Pixels = RenderTarget->Data();
	if (Pixels == nullptr) return;
	ClearColor();

	Matrix4 prespective = Matrix4(1);
	prespective = {
		0.2f / (float)Width, 0, 0, 0,
		0, -0.2f / (float)Height, 0, 0,
		0, 0, 100.1f / -99.9f, (-0.2f * 100.0f) / -99.9f,
		0, 0, 1, 0
	};
	prespective[1][1] *= -1;

	for (Mesh& mesh : SceneObjects) {
		const std::vector<Vertex>& vertices = mesh.GetVertices();
		Topology topology = mesh.GetTopologyMode();
		Matrix4 model = mesh.GetMatrixModel();

		VertexShader(vertices, prespective * model, topology);
	}
	EndRenderCallback();

	cv::imshow("RenderTarget", Screen);
}

void Renderer::ClearColor() {
	auto frameBuffer = RenderTarget->Data();

	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width * 3; j += 3) {
			frameBuffer[i * Width * 3 + j + 0] = (unsigned char)ClearColorValue.r;
			frameBuffer[i * Width * 3 + j + 1] = (unsigned char)ClearColorValue.g;
			frameBuffer[i * Width * 3 + j + 2] = (unsigned char)ClearColorValue.b;
		}
	}
}

void Renderer::ClearObjects() {
	SceneObjects.resize(0);
}

void Renderer::VertexShader(std::vector<Vertex> vertices, Matrix4 mvp, Topology topo) {
	if (vertices.empty()) return;
	
	std::vector<Vector> vectors = std::vector<Vector>(vertices.size());
	for (size_t index = 0; index < vertices.size(); ++index) {
		vectors[index] = mvp * vertices[index].position;
	}

	std::vector<std::vector<Vector>>  primitives = Assembly(vectors, topo);
	if (topo == Topology::eLine) {
		for (std::vector<Vector>& primitive : primitives) {
			RasterizationLine(primitive.data());
		}
	}
	else if (topo == Topology::eTriangle) {
		for (std::vector<Vector>& primitive : primitives) {
			Rasterization(primitive.data());
		}
	}
}

std::vector<std::vector<Vector>>  Renderer::Assembly(std::vector<Vector> vector_list, Topology topo) {
	std::vector<std::vector<Vector>> primitives;
	if (topo == Topology::eLine) {
		for (int i = 0; i < vector_list.size() - 1; i+=1) {
			std::vector<Vector> primitive;
			primitive.push_back(vector_list[i]);
			primitive.push_back(vector_list[i + 1]);
			primitives.push_back(primitive);
		}

		std::vector<Vector> primitive;
		primitive.push_back(vector_list[vector_list.size() - 1]);
		primitive.push_back(vector_list[0]);
		primitives.push_back(primitive);
	}
	else if (topo == Topology::eLineList) {
		std::vector<std::vector<Vector>> primitives;
		for (int i = 0; i < vector_list.size() - 1; i += 2) {
			std::vector<Vector> primitive;
			primitive.push_back(vector_list[i]);
			primitive.push_back(vector_list[i + 1]);
			primitives.push_back(primitive);
		}

		std::vector<Vector> primitive;
		primitive.push_back(vector_list[vector_list.size() - 1]);
		primitive.push_back(vector_list[0]);
		primitives.push_back(primitive);
	}
	else if (topo == Topology::eTriangle) {
		for (int i = 0; i < vector_list.size() - 2; i += 3) {
			std::vector<Vector> primitive;
			primitive.push_back(vector_list[i]);
			primitive.push_back(vector_list[i + 1]);
			primitive.push_back(vector_list[i + 2]);
			primitives.push_back(primitive);
		}
	}

	return primitives;
}

void Renderer::Rasterization(Vector* vector_list) {
	Vector p1 = vector_list[0];
	Vector p2 = vector_list[1];
	Vector p3 = vector_list[2];

	p1.x *= 2.0f * (float)Width;
	p1.y *= 2.0f * (float)Height;
	p2.x *= 2.0f * (float)Width;
	p2.y *= 2.0f * (float)Height;
	p3.x *= 2.0f * (float)Width;
	p3.y *= 2.0f * (float)Height;

#ifdef _DEBUG
	std::cout << p2.x << " " << p2.y << " " << p2.z << "\n";
#endif
 
	if (p1.y > p2.y) std::swap(p1, p2);
	if (p1.y > p3.y) std::swap(p1, p3);
	if (p2.y > p3.y) std::swap(p2, p3);

	float totalHeight = p3.y - p1.y;
	Vector2 offsetHalfScreen(Width / 2.0f, Height / 2.0f);

	// Bottom triangle
	for (float h = p1.y; h < p2.y; h++) {
		float segmentHeight = p2.y - p1.y + 1;
		float alpha = (float)(h - p1.y) / totalHeight;
		float beta = (float)(h - p1.y) / segmentHeight;

		Vector vv1 = (p3 - p1) * alpha;
		Vector A = p1 + vv1;

		Vector vv2 = (p2 - p1) * beta;
		Vector B = p1 + vv2;

		if (A.x > B.x) std::swap(A, B);
		for (float j = A.x; j <= B.x; j++) {
			Color col = Color((j - A.x) / (B.x - A.x) * 255.0f);
			FragmentShader(Vector2(j, h) + offsetHalfScreen, COLOR_WHITE);
		}
	}
	
	// Upper triangle
	for (float h = p2.y; h < p3.y; h++) {
		float segmentHeight = p3.y - p2.y + 1;
		float alpha = (float)(h - p1.y) / totalHeight;
		float beta = (float)(h - p2.y) / segmentHeight;

		Vector vv1 = (p3 - p1) * alpha;
		Vector A = p1 + vv1;

		Vector vv2 = (p3 - p2) * beta;
		Vector B = p2 + vv2;

		if (A.x > B.x) std::swap(A, B);
		for (float j = A.x; j <= B.x; j++) {
			Color col = Color((j - A.x) / (B.x - A.x) * 255.0f);
			FragmentShader(Vector2(j, h) + offsetHalfScreen, COLOR_WHITE);
		}
	}
}

void Renderer::RasterizationLine(Vector* vector_list) {
	bool steep = false;

	Vector pos1 = vector_list[0];
	Vector pos2 = vector_list[1];
	Color color = Color(255, 255, 255);

	pos1.x *= 2.0f * (float)Width;
	pos1.y *= 2.0f * (float)Height;
	pos2.x *= 2.0f * (float)Width;
	pos2.y *= 2.0f * (float)Height;

	if (std::abs(pos1.x - pos2.x) < std::abs(pos1.y - pos2.y)) {
		std::swap(pos1.x, pos1.y);
		std::swap(pos2.x, pos2.y);
		steep = true;
	}

	if (pos1.x > pos2.x) {
		std::swap(pos1.x, pos2.x);
		std::swap(pos1.y, pos2.y);
	}

	float dx = pos2.x - pos1.x;
	float dy = pos2.y - pos1.y;
	float derror = std::abs(dy / dx);
	float error = 0;
	float y = pos1.y;

	Vector2 offsetHalfScreen(Width / 2.0f, Height / 2.0f);
	for (int x = (int)pos1.x; x < pos2.x; x++) {
		// Position
		float k = (x - pos1.x) / (pos2.x - pos1.x);
		y = pos1.y * (1.0f - k) + pos2.y * k;

		if (steep) {
			FragmentShader(Vector2(y, x) + offsetHalfScreen, color);
		}
		else {
			FragmentShader(Vector2(x, y) + offsetHalfScreen, color);
		}

		error += derror;
		if (error > .5f) {
			y += (pos2.y > pos1.y ? 1 : -1);
			error -= 1;
		}
	}
}

void Renderer::FragmentShader(Vector2 pos, Color col) {
	Shader(iVec2(pos), col);
}

void Renderer::Shader(iVec2 pos, Color col) {
	auto frame = RenderTarget->Data();
	int size = Width * Height * 3;

	if (frame != nullptr) {
		if (pos.x < 0 || pos.y < 0
			|| pos.x > Width - 1 || pos.y > Height - 1)
			return;

		int index = (Height - 1 - pos.y) * Width * 3 + pos.x * 3;
		for (int i = 0; i < 3; ++i) {
			if ((index + i) > size || index < 0) continue;
			frame[index + i] = (unsigned char)col[i];

		}
	}
}

Mesh Renderer::GenerateCircle(Vector center, float r, bool filled) {
	Mesh circle;
	std::vector<Vertex>& vertices = circle.GetVertices();

	int side = r > 50.f ? 360 : 36;
	float radius = 2.0f * 3.1415926f / side;
	for (int i = 0; i <= side; i+=2) {
		float current_radius = i * radius;
		float next_radius = (i + 1) * radius;

		float cx = cos(current_radius) * r;
		float cy = sin(current_radius) * r;
		float nx = cos(next_radius) * r;
		float ny = sin(next_radius) * r;

		Vector c_position = center + Vector{ cx, cy, 0, 1 };
		Vector n_position = center + Vector{ nx, ny, 0, 1 };

		vertices.push_back(c_position);
		vertices.push_back(n_position);

		if (filled) vertices.push_back(center);
	}

	if (!filled) circle.SetTopologyMode(Topology::eLine);
	return circle;
}

Mesh Renderer::GenerateLine(Vector v1, Vector v2){
	Mesh line;
	std::vector<Vertex>& vertices = line.GetVertices();

	vertices.push_back(v1);
	vertices.push_back(v2);

	line.SetTopologyMode(Topology::eLine);

	return line;
}

Mesh Renderer::GenerateTriangle(Vector v1, Vector v2, Vector v3){
	Mesh triangle;
	std::vector<Vertex>& vertices = triangle.GetVertices();

	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	return triangle;
}
