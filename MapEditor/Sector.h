#pragma once
#include <vector>
#include "Utils.h"
#include <Thor/Shapes.hpp>

class Sector {
public:
    std::vector<sf::Vector2f> vertices;   // List of vertices that define the polygon of the sector
    float floor_height = 0;                    // Height of the floor
    float ceiling_height = 0;                  // Height of the ceiling
    sf::Color floor_color;                 // Color or texture ID for the floor
    sf::Color ceiling_color;               // Color or texture ID for the ceiling
    sf::Color wall_color;                  // Color or texture ID for the walls
    bool initialized = false;
    // Add more properties as needed, such as special flags, light levels, etc.

    /// <summary>
    /// Constructor for a Sector
    /// </summary>
    Sector() : floor_height(0.0f), ceiling_height(4.0f), // Default heights
        floor_color(sf::Color::Green), ceiling_color(sf::Color::White),
        wall_color(sf::Color::Red) {
        initialized = false;
    }

    /// <summary>
    /// Gets the sector as a sf::ConvexShape
    /// </summary>
    /// <returns>output sf::ConvexShape</returns>
    thor::ConcaveShape get_shape(bool is_selected) {

        thor::ConcaveShape shape;

        //sf::ConvexShape shape;
        shape.setOutlineColor(wall_color);
        shape.setOutlineThickness(1.5);
        shape.setPointCount(vertices.size());
        for (size_t i = 0; i < vertices.size(); ++i) {
            shape.setPoint(i, vertices[i]);
        }
        // Adjust color brightness based on selection status
        sf::Color color = is_selected ? floor_color : sf::Color(floor_color.r, floor_color.g, floor_color.b, 100);
        shape.setFillColor(color);
        return shape;

    }
};

