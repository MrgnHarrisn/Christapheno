#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Shapes.hpp>
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

	static void render_concave(sf::RenderWindow& window, std::vector<sf::Vector2f> &vertices);

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

	static std::vector<sf::Vector2f> concave_to_convex(const std::vector<sf::Vector2f>& vertices);

	static float cross_product(sf::Vector2f a, sf::Vector2f b);

	static sf::Vector2f get_item(std::vector<sf::Vector2f>& vertices, size_t index);

	static float get_polygon_area(std::vector<sf::Vector2f>& vertices);

	static bool is_point_in_triangle(const sf::Vector2f& pt, const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3);

};

