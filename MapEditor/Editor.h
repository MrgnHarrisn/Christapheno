#pragma once

#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Sector.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "Mode.h"
#include <vector>
#include <cmath>
#include "Utils.h"

class Editor
{
public:

	/// <summary>
	/// Constructor for editor with width and height
	/// </summary>
	/// <param name="width">int</param>
	/// <param name="height">int</param>
	Editor(int width, int height);

	/// <summary>
	/// starts the editor
	/// </summary>
	void start();
private:

	// toggles displaying a BSP version
	bool display_bsp = false;

	// Editor Functions
	void update();

	/// <summary>
	/// checks if the window is open
	/// </summary>
	/// <returns></returns>
	bool is_open();

	/// <summary>
	/// Closes the window and imgui
	/// </summary>
	void close();

	/// <summary>
	/// Runs the imgui windows
	/// </summary>
	void render_imgui();

	/// <summary>
	/// Decorates and customizes the imgui UI
	/// </summary>
	void setup_imgui_style();

	/// <summary>
	/// runs all event handlers
	/// </summary>
	void handle_events();
	
	Camera* m_camera;

	/* Delta time stuff */
	sf::Time m_time;
	float m_dt = 0;

	sf::Clock m_clock;
	sf::RenderWindow* m_window;

	// Level editing variables

	float grid_size = 50.0;

	Mode m_mode;
	std::vector<Sector> m_sectors;
	std::vector<sf::Vector2f> m_vertices;
	int m_current_sector = 0;

	int m_selected_vertex = -1;
	bool m_vertex_is_selected = false;

	float m_floor_color[3] = {};
	float m_ceiling_color[3] = {};
	float m_wall_color[3] = {};

};

