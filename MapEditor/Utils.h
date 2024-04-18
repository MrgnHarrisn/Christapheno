#pragma once

#include <SFML/Graphics.hpp>
#include <Thor/Shapes.hpp>

#include <nfd.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Camera.h"
#include "Sector.h"

#define dbg(X) std::cout << X << std::endl;

class Utils
{
public:
	/// <summary>
	/// Draws rectangles at each vertex
	/// </summary>
	/// <param name="window">sf::RenderTarget</param>
	/// <param name="vertices">sf::Vector2f[]</param>
	static void draw_vertex_markers(sf::RenderWindow& window, const std::vector<sf::Vector2f>& vertices);
	
	/// <summary>
	/// Draws a shape to a RenderTarget
	/// </summary>
	/// <param name="window">sf::RenderTarget</param>
	/// <param name="vertices">sf::Vector2f[]</param>
	static void draw_shape(sf::RenderWindow& window, const std::vector<sf::Vector2f>& vertices);
	
	/// <summary>
	/// Draws a grid relative to camera
	/// </summary>
	/// <param name="window">sf::RenderTarget</param>
	/// <param name="camera">Camera</param>
	static void draw_grid(sf::RenderWindow& window, const Camera& camera, double grid_spacing);
	
	static sf::Vector2f snap_to_grid(const sf::Vector2f& point, float gridSize);

	/// <summary>
	/// Converts an sf::Color to a float[3]
	/// </summary>
	/// <param name="color">sf::Color</param>
	/// <returns>float[3]</returns>
	static sf::Color float_3_color(float color[3]);
	
	/// <summary>
	/// sets contents of an sf::Color to a float[3]
	/// </summary>
	/// <param name="color">sf::Color</param>
	/// <param name="output">float[3]</param>
	static void color_float_3(sf::Color color, float output[3]);

	/// <summary>
	/// Checks for overlapping vectors
	/// </summary>
	/// <param name="target">position to check</param>
	/// <param name="others">positions to check against</param>
	/// <param name="index">optional index of vector to ignore</param>
	/// <returns>found occupid spot</returns>
	static bool is_overlapping_vec(sf::Vector2f target, const std::vector<sf::Vector2f>& others, int index = -1);

	static double get_dynamic_grid_size(double zoomLevel);

	static void save(std::vector<Sector>& sectors);

	static void goto_sectors(std::vector<Sector>& sectors, Camera& camera, size_t sector);

	static void load(std::vector<Sector>& sectors);

};

