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

static bool showGrid1 = true;
static bool showGrid2 = true;
static int rows1 = 10, columns1 = 10;
static int rows2 = 10, columns2 = 10;
static ImColor color1 = IM_COL32(255, 0, 0, 255); // Red color
static ImColor color2 = IM_COL32(0, 0, 255, 255); // Blue color
static ImVec2 origin1 = ImVec2(0, 0);
static ImVec2 origin2 = ImVec2(0, 0);
static bool dotted1 = false;
static bool dotted2 = false;

struct GridParams {
	bool showGrid = true;
	int rows = 10, columns = 10;
	ImColor color = IM_COL32(255, 0, 0, 255);
	ImVec2 origin = ImVec2(0, 0);
	bool dotted = false;
	std::map<std::pair<int, int>, ImColor> coloredCells; // stores (row, column) => color mapping
};

GridParams loadedParams1 = {
	.showGrid = showGrid1,
	.rows = rows1,
	.columns = columns1,
	.color = color1,
	.origin = origin1,
	.dotted = dotted1
};

GridParams loadedParams2 = {
	.showGrid = showGrid2,
	.rows = rows2,
	.columns = columns2,
	.color = color2,
	.origin = origin2,
	.dotted = dotted2
};

void SaveGridParams(const GridParams& params, const char* filename) {
	std::ofstream file(filename, std::ios::binary);
	if (file.is_open()) {
		file.write((char*)&params, sizeof(GridParams));
		file.close();
	}
}
bool LoadGridParams(GridParams& params, const char* filename) {
	std::ifstream file(filename, std::ios::binary);
	if (file.is_open()) {
		file.read((char*)&params, sizeof(GridParams));
		file.close();
		return true;
	}
	return false;
}

void DrawGrid(GridParams& params);
void GridWidget(GridParams& params);

void GridControlWidget()
{
	if (ImGui::CollapsingHeader("Grid 1 Controls"))
		GridWidget(loadedParams1);
	if (ImGui::CollapsingHeader("Grid 2 Controls"))
		GridWidget(loadedParams2);

	if (showGrid1)
	{
		DrawGrid(loadedParams1);
	}
	if (showGrid2)
	{
		DrawGrid(loadedParams2);
	}
}

void GridWidget(GridParams & params) {
	ImGui::Checkbox("Show Grid", &params.showGrid);
	ImGui::InputInt("Grid Rows", &params.rows);
	ImGui::InputInt("Grid Columns", &params.columns);
	ImGui::ColorEdit3("Grid Color", (float*)&params.color);
	ImGui::InputFloat2("Grid Origin", (float*)&params.origin);
	ImGui::Checkbox("Dotted Lines", &params.dotted);

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

	window = SDL_CreateWindow("SDL2 Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, SDL_WINDOW_RESIZABLE);
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

	GridParams loadedParams1;
	GridParams loadedParams2;

	if (LoadGridParams(loadedParams1, "grid1_config.bin")) {
		showGrid1 = loadedParams1.showGrid;
		rows1 = loadedParams1.rows;
		columns1 = loadedParams1.columns;
		color1 = loadedParams1.color;
		origin1 = loadedParams1.origin;
		dotted1 = loadedParams1.dotted;
	}

	if (LoadGridParams(loadedParams2, "grid2_config.bin")) {
		showGrid2 = loadedParams2.showGrid;
		rows2 = loadedParams2.rows;
		columns2 = loadedParams2.columns;
		color2 = loadedParams2.color;
		origin2 = loadedParams2.origin;
		dotted2 = loadedParams2.dotted;
	}

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

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.0f));
		ImGui::Begin("Kent's Grid Thingie");
		ImGui::Text("Using Dear ImGui. Of course.");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		GridControlWidget();
		// Advance the ImGui cursor to claim space in the window (otherwise the window will appear small and needs to be resized)
		ImGui::Dummy(ImVec2(400, 400));
		ImGui::End();
		ImGui::PopStyleColor();


		ImGui::Render();
		SDL_SetRenderDrawColor(renderer, 124, 34, 151, 17);
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);

	}
	// Cleanup

	GridParams params1 = { showGrid1, rows1, columns1, color1, origin1, dotted1 };
	GridParams params2 = { showGrid2, rows2, columns2, color2, origin2, dotted2 };
	SaveGridParams(params1, "grid1_config.bin");
	SaveGridParams(params2, "grid2_config.bin");

	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}