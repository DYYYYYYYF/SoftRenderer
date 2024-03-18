#pragma once
#include "Buffer.hpp"

class Renderer {
public:
	Renderer();
	virtual ~Renderer() {
		if (RenderTarget != nullptr) delete(RenderTarget);
	}

	bool Init();
	void Render();

// Member function
public:
	void SetWidth(int width) { Width = width; }
	int GetWidth() const { return Width; }

	void SetHeight(int height) { Height = height; }
	int GetHeight() const { return Height; }

	Color GetClearColor() const {
		return ClearColorValue;
	}
	void SetClearColor(Color col) {
		ClearColorValue = col;
	}

	void AddMesh(Mesh mesh) {
		SceneObjects.push_back(mesh);
	}

	void DrawLine(Vector v1 = Vector(-100.0f, 50.0f, 10.0f, 1.0f),
		Vector v2 = Vector(250.0f, 50.0f, 20.0f, 1.0f)) {
		SceneObjects.push_back(GenerateLine(v1, v2));
	}

	void DrawTriangle(Vector v1 = Vector(-100.0f, 0.0f, 1.0f, 1.0f),
		Vector v2 = Vector(250.0f, 200.0f, 2.0f, 1.0f),
		Vector v3 = Vector(300.0f, -50.0f, 4.0f, 1.0f)) {
		SceneObjects.push_back(GenerateTriangle(v1, v2, v3));
	}

	void DrawCircle(Vector center = Vector(-100.0f, 50.0f, 10.0f, 1.0f), 
		float r = 50.0f, bool filled = true) {
		SceneObjects.push_back(GenerateCircle(center, r, filled));
	}
	
	void ClearColor();
	void ClearObjects();

private:
	// Pipeline
	void VertexShader(std::vector<Vertex> vertices, Matrix4 mvp, Topology topo = Topology::eTriangle);
	void Rasterization(Vector* vector_list);
	void RasterizationLine(Vector* vector_list);
	void FragmentShader(Vector2 pos, Color col);
	void Shader(iVec2 pos, Color col);
	
	std::vector<std::vector<Vector>> Assembly(std::vector<Vector> vector_list, Topology topo);

public:
	static void RegisterBeginRenderCallBack(std::function<void()> func);
	static void RegisterEndRenderCallBack(std::function<void()> func);

	static Mesh GenerateCircle(Vector center = Vector(-100.0f, 50.0f, 10.0f, 1.0f),
		float r = 50.0f, bool filled = true);

	static Mesh GenerateLine(
		Vector v1 = Vector(-100.0f, 50.0f, 10.0f, 1.0f),
		Vector v2 = Vector(250.0f, 50.0f, 20.0f, 1.0f));

	static Mesh GenerateTriangle(
		Vector v1 = Vector(-100.0f, 0.0f, 1.0f, 1.0f),
		Vector v2 = Vector(250.0f, 200.0f, 2.0f, 1.0f),
		Vector v3 = Vector(300.0f, -50.0f, 4.0f, 1.0f));

private:
	cv::Mat Screen;
	int Width, Height;
	Buffer *RenderTarget;
	Color ClearColorValue;

	std::vector<Mesh> SceneObjects;

	static std::function<void()> BeiginRenderCallback;
	static std::function<void()> EndRenderCallback;
};