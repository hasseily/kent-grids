#include <SDL2/SDL.h> 
#include <SDL2/gfx/SDL2_gfxPrimitives.h>

#include <iostream>
#include <fstream>

#include <map>
#include <utility>  // for std::pair

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_sdlrenderer.h"

#include <glm/glm.hpp>
#include <stb_image.h>

struct GridParams {
	bool showGrid = true;
	int rows = 10, columns = 10;
	ImColor color = IM_COL32(255, 0, 0, 255);
	ImVec2 origin = ImVec2(0, 0);
	bool dotted = false;
	bool showNumbers = false;
	ImColor numberColor = IM_COL32(255, 255, 255, 255);  // default to white
	float fontSize = 14.0f;
	float startingValue = 0.0f;
	float increment = 1.0f;

	std::map<std::pair<int, int>, ImColor> coloredCells; // stores (row, column) => color mapping
};

static GridParams loadedParams1;
static GridParams loadedParams2;

void SaveGridParams(const GridParams& params, const char* filename) {
	std::ofstream file(filename, std::ios::binary);
	if (file.is_open()) {
		file.write((char*)&params.showGrid, sizeof(bool));
		file.write((char*)&params.rows, sizeof(int));
		file.write((char*)&params.columns, sizeof(int));
		file.write((char*)&params.dotted, sizeof(bool));
		file.write((char*)&params.showNumbers, sizeof(bool));
		file.write((char*)&params.fontSize, sizeof(float));
		file.write((char*)&params.startingValue, sizeof(float));
		file.write((char*)&params.increment, sizeof(float));

		// Write ImVec2 values
		file.write((char*)&params.origin, sizeof(ImVec2));

		// Write ImColor values (RGBA)
		int r, g, b, a;
		r = params.color.Value.x * 255;
		g = params.color.Value.y * 255;
		b = params.color.Value.z * 255;
		a = params.color.Value.w * 255;

		file.write((char*)&r, sizeof(int));
		file.write((char*)&g, sizeof(int));
		file.write((char*)&b, sizeof(int));
		file.write((char*)&a, sizeof(int));

		r = params.numberColor.Value.x * 255;
		g = params.numberColor.Value.y * 255;
		b = params.numberColor.Value.z * 255;
		a = params.numberColor.Value.w * 255;

		file.write((char*)&r, sizeof(int));
		file.write((char*)&g, sizeof(int));
		file.write((char*)&b, sizeof(int));
		file.write((char*)&a, sizeof(int));

		// save the cell colors as well
		int mapSize = params.coloredCells.size();
		file.write((char*)&mapSize, sizeof(int));

		for (const auto& pair : params.coloredCells) {
			std::pair<int, int> rowColPair = pair.first;
			ImColor color = pair.second;

			file.write((char*)&rowColPair.first, sizeof(int));
			file.write((char*)&rowColPair.second, sizeof(int));

			int r, g, b, a;
			r = color.Value.x * 255;
			g = color.Value.y * 255;
			b = color.Value.z * 255;
			a = color.Value.w * 255;

			file.write((char*)&r, sizeof(int));
			file.write((char*)&g, sizeof(int));
			file.write((char*)&b, sizeof(int));
			file.write((char*)&a, sizeof(int));
		}

		file.close();
	}
}
bool LoadGridParams(GridParams& params, const char* filename) {
	std::ifstream file(filename, std::ios::binary);
	if (file.is_open()) {
		file.read((char*)&params.showGrid, sizeof(bool));
		file.read((char*)&params.rows, sizeof(int));
		file.read((char*)&params.columns, sizeof(int));
		file.read((char*)&params.dotted, sizeof(bool));
		file.read((char*)&params.showNumbers, sizeof(bool));
		file.read((char*)&params.fontSize, sizeof(float));
		file.read((char*)&params.startingValue, sizeof(float));
		file.read((char*)&params.increment, sizeof(float));

		// Read ImVec2 values
		file.read((char*)&params.origin, sizeof(ImVec2));

		// Read ImColor values (RGBA)
		int r, g, b, a;

		file.read((char*)&r, sizeof(int));
		file.read((char*)&g, sizeof(int));
		file.read((char*)&b, sizeof(int));
		file.read((char*)&a, sizeof(int));

		params.color = ImColor(r, g, b, a);

		file.read((char*)&r, sizeof(int));
		file.read((char*)&g, sizeof(int));
		file.read((char*)&b, sizeof(int));
		file.read((char*)&a, sizeof(int));

		params.numberColor = ImColor(r, g, b, a);

		// load the cell colors as well
		int mapSize;
		file.read((char*)&mapSize, sizeof(int));

		params.coloredCells.clear();

		for (int i = 0; i < mapSize; ++i) {
			std::pair<int, int> rowColPair;
			int r, g, b, a;

			file.read((char*)&rowColPair.first, sizeof(int));
			file.read((char*)&rowColPair.second, sizeof(int));

			file.read((char*)&r, sizeof(int));
			file.read((char*)&g, sizeof(int));
			file.read((char*)&b, sizeof(int));
			file.read((char*)&a, sizeof(int));

			ImColor color(r, g, b, a);
			params.coloredCells[rowColPair] = color;
		}
		file.close();
		return true;
	}
	return false;
}

void DrawGrid(GridParams& params);
void GridWidget(GridParams& params);

void GridControlWidget()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.0f));
	ImGui::Begin("Parameters");
	ImGui::Text("Using Dear ImGui. Of course.");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::CollapsingHeader("Grid 1 Controls"))
	{
		ImGui::PushID(111);
		GridWidget(loadedParams1);
		ImGui::PopID();
	}
	if (ImGui::CollapsingHeader("Grid 2 Controls"))
	{
		ImGui::PushID(2222);
		GridWidget(loadedParams2);
		ImGui::PopID();
	}

	ImGui::End();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.0f));
	ImGui::Begin("Kent Grid");
	if (loadedParams1.showGrid)
	{
		DrawGrid(loadedParams1);
	}
	if (loadedParams2.showGrid)
	{
		DrawGrid(loadedParams2);
	}
	// Advance the ImGui cursor to claim space in the window
	// (otherwise the window will appear small and needs to be resized)
	ImGui::Dummy(ImVec2(400, 400));
	ImGui::End();
	ImGui::PopStyleColor();
}

void GridWidget(GridParams & params) {
	ImGui::Checkbox("Show Grid", &params.showGrid);
	ImGui::InputInt("Grid Rows", &params.rows);
	ImGui::InputInt("Grid Columns", &params.columns);
	ImGui::ColorEdit3("Grid Color", (float*)&params.color);
	ImGui::InputFloat2("Grid Origin", (float*)&params.origin);
	ImGui::Checkbox("Dotted Lines", &params.dotted);
	ImGui::Checkbox("Show Numbers", &params.showNumbers);
	if (params.showNumbers) {
		ImGui::ColorEdit3("Number Color", (float*)&params.numberColor);
		ImGui::SliderFloat("Font Size", &params.fontSize, 10.0f, 32.0f);
		ImGui::InputFloat("Starting Value", &params.startingValue);
		ImGui::InputFloat("Increment", &params.increment);
	}

	static int selectedRow = 0, selectedColumn = 0;
	static ImColor selectedCellColor = IM_COL32(255, 255, 255, 255);

	if (ImGui::SliderInt("Select Row", &selectedRow, 0, params.rows - 1) ||
		ImGui::SliderInt("Select Column", &selectedColumn, 0, params.columns - 1)) {
		auto it = params.coloredCells.find({ selectedRow, selectedColumn });
		if (it != params.coloredCells.end()) {
			selectedCellColor = it->second;
		}
		else {
			selectedCellColor = IM_COL32(255, 255, 255, 255);  // default to white if not previously set
		}
	}

	if (ImGui::ColorEdit3("Cell Color", (float*)&selectedCellColor)) {
		params.coloredCells[{selectedRow, selectedColumn}] = selectedCellColor;
	}

	if (ImGui::Button("Clear Cell Color")) {
		params.coloredCells.erase({ selectedRow, selectedColumn });
	}
}

void DrawGrid(GridParams& params)
{
	// Window size
	ImVec2 winSize = ImGui::GetWindowSize();
	ImVec2 winPos = ImGui::GetCursorScreenPos();

	// Calculate padding
	float padding = 50.0f; // Adjust as needed

	// Determine the size of a square cell based on the smaller dimension of the window
	float cellSize = std::min((winSize.x - 2.0f * padding) / params.columns, (winSize.y - 2.0f * padding) / params.rows);

	// Adjust the total grid width and height based on cell size
	float gridWidth = params.columns * cellSize;
	float gridHeight = params.rows * cellSize;

	float xOffset = winPos.x + params.origin.x + padding;
	float yOffset = winPos.y + params.origin.y + padding;

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// draw the cells first
	for (const auto& cell : params.coloredCells) {
		ImVec2 topLeft = ImVec2(xOffset + cellSize * cell.first.second, yOffset + cellSize * cell.first.first);
		ImVec2 bottomRight = ImVec2(topLeft.x + cellSize, topLeft.y + cellSize);
		drawList->AddRectFilled(topLeft, bottomRight, cell.second);
	}

	// then the axes values
	if (params.showNumbers) {
		char number[16]; // buffer for number-to-string conversion

		// Above the grid
		for (int i = 0; i < params.columns; i++) {
			float value = params.startingValue + i * params.increment;
			snprintf(number, sizeof(number), "%.2f", value);  // assuming 2 decimal places
			ImVec2 pos = ImVec2(xOffset + cellSize * i, yOffset - params.fontSize);
			drawList->AddText(NULL, params.fontSize, pos, params.numberColor, number);
		}

		// To the left of the grid
		for (int i = 0; i < params.rows; i++) {
			float value = params.startingValue + i * params.increment;
			snprintf(number, sizeof(number), "%.2f", value);  // assuming 2 decimal places
			ImVec2 pos = ImVec2(xOffset - params.fontSize * 3, yOffset + cellSize * i);
			drawList->AddText(NULL, params.fontSize, pos, params.numberColor, number);
		}
	}

	// Finally the grid
	for (int i = 0; i <= params.columns; i++)
	{
		ImVec2 start = ImVec2(xOffset + i * cellSize, yOffset);
		ImVec2 end = ImVec2(xOffset + i * cellSize, yOffset + gridHeight);

		if (params.dotted)
		{
			for (int j = 0; j < params.rows; j++)
			{
				for (int k = 0; k < 10; k += 2)
				{
					ImVec2 dotStart = ImVec2(start.x, start.y + j * cellSize + cellSize * (k / 10.f));
					ImVec2 dotEnd = ImVec2(start.x, start.y + j * cellSize + cellSize * ((k + 1) / 10.f)); // 10% cell height for dot length
					drawList->AddLine(dotStart, dotEnd, params.color);
				}
			}
		}
		else
		{
			drawList->AddLine(start, end, params.color);
		}
	}

	for (int i = 0; i <= params.rows; i++)
	{
		ImVec2 start = ImVec2(xOffset, yOffset + i * cellSize);
		ImVec2 end = ImVec2(xOffset + gridWidth, yOffset + i * cellSize);

		if (params.dotted)
		{
			for (int j = 0; j < params.columns; j++)
			{
				for (int k = 0; k < 10; k += 2)
				{
					ImVec2 dotStart = ImVec2(start.x + j * cellSize + cellSize * (k / 10.f), start.y);
					ImVec2 dotEnd = ImVec2(start.x + j * cellSize + cellSize * ((k + 1) / 10.f), start.y); // 10% cell height for dot length
					drawList->AddLine(dotStart, dotEnd, params.color);
				}
			}
		}
		else
		{
			drawList->AddLine(start, end, params.color);
		}
	}
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event event;

	int resX = 800, resY = 800;

	window = SDL_CreateWindow("Kent Grid Tool", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

	// Load saved params
	// set defaults first
	loadedParams1.color = IM_COL32(255, 0, 0, 255); // Red color,
	loadedParams2.color = IM_COL32(0, 0, 255, 255);
	loadedParams2.origin = ImVec2(5, 5);
	loadedParams2.dotted = true;

	LoadGridParams(loadedParams1, "grid1_config.bin");
	LoadGridParams(loadedParams2, "grid2_config.bin");

	while (true) {
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				break;
			}

		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		GridControlWidget();

		ImGui::Render();
		SDL_SetRenderDrawColor(renderer, 124, 34, 151, 17);
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);

	}
	// Cleanup

	SaveGridParams(loadedParams1, "grid1_config.bin");
	SaveGridParams(loadedParams2, "grid2_config.bin");

	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}