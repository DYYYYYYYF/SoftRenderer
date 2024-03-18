#include "Renderer.hpp"
#include "VoronoiDiagram.h"

// Callback
void BeginRender();
void EndRender();


// Entrance
int main(int argv, char* args[]) {
	Renderer renderer;
	renderer.Init();
	renderer.SetClearColor(Color{ 0, 0, 0 });

	// Registe Callback
	Renderer::RegisterBeginRenderCallBack(BeginRender);
	Renderer::RegisterEndRenderCallBack(EndRender);
	
	VoronoiDiagram::GenerateVoronoi(&renderer, 3);

	int pressedKey = -1;
	while (pressedKey = cv::waitKey(0)) {
		if (pressedKey == KeyBoard::eKey_ESC) break;

		// Change sites
		if (pressedKey == KeyBoard::eKey_C) {
			renderer.ClearObjects();
			VoronoiDiagram::GenerateVoronoi(&renderer, 3);
		}
		

		renderer.Render();
	}

	return 0;
}

#ifdef _DEBUG
void BeginRender() {

	std::cout << "Begin Render\n";
}

void EndRender() {
	std::cout << "End Render\n";
}

#else	// Release

void BeginRender() {

}

void EndRender() {

}

#endif

// ------------------------------------------------------------------------------------------------------
void TestColorChannel() {

	float* buffer = (float*)malloc(sizeof(float) * 600 * 600 * 3);
	if (buffer != nullptr) {
		for (int i = 0; i < 600; ++i) {
			for (int j = 0; j < 600 * 3; j += 3) {
				buffer[i * 600 * 3 + j + 0] = 1.05f;
				buffer[i * 600 * 3 + j + 1] = 0.0f;
				buffer[i * 600 * 3 + j + 2] = 0.0f;
			}
		}
		cv::Mat a = cv::Mat(cv::Size(600, 600), CV_32FC3, buffer);
		cv::cvtColor(a, a, cv::COLOR_BGR2RGB);

		for (int i = 0; i < 600; ++i) {
			for (int j = 0; j < 600 * 3; j += 3) {
				if (j < 300 * 3) {
					buffer[i * 600 * 3 + j + 0] = 1.05f;
					buffer[i * 600 * 3 + j + 1] = 0.0f;
					buffer[i * 600 * 3 + j + 2] = 0.0f;
				}
				else {
					buffer[i * 600 * 3 + j + 0] = 0.0f;
					buffer[i * 600 * 3 + j + 1] = 0.0f;
					buffer[i * 600 * 3 + j + 2] = 1.0f;
				}
			}
		}

		cv::cvtColor(a, a, cv::COLOR_BGR2RGB);
		cv::imshow("TestFloat", a);
		cv::waitKey(0);

		free(buffer);
	}

}
