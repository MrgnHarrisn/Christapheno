#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"

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
	static void draw_grid(sf::RenderWindow& window, const Camera& camera);
	
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
};

