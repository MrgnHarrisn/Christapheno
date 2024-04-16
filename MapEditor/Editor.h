#pragma once

#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Sector.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "Mode.h"
#include <vector>
#include "Utils.h"

class Editor
{
public:
	Editor(int width, int height);
	void start();
private:

	// Editor Functions
	void update();
	bool is_open();
	void close();

	// imgui functions
	void render_imgui();
	void setup_imgui_style();
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

