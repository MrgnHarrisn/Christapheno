#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Sector {
public:
    std::vector<sf::Vector2f> vertices;   // List of vertices that define the polygon of the sector
    float floorHeight = 0;                    // Height of the floor
    float ceilingHeight = 0;                  // Height of the ceiling
    sf::Color floorColor;                 // Color or texture ID for the floor
    sf::Color ceilingColor;               // Color or texture ID for the ceiling
    sf::Color wallColor;                  // Color or texture ID for the walls
    bool initialized = false;
    // Add more properties as needed, such as special flags, light levels, etc.

    /// <summary>
    /// Constructor for a Sector
    /// </summary>
    Sector() : floorHeight(0.0f), ceilingHeight(4.0f), // Default heights
        floorColor(sf::Color::Green), ceilingColor(sf::Color::White),
        wallColor(sf::Color::Red) {
        floorHeight = 0;
        ceilingHeight = 1;
        initialized = false;
    }

    /// <summary>
    /// Gets the sector as a sf::ConvexShape
    /// </summary>
    /// <returns>output sf::ConvexShape</returns>
    sf::ConvexShape get_shape(bool is_selected) {

        sf::ConvexShape shape;
        shape.setPointCount(vertices.size());
        for (size_t i = 0; i < vertices.size(); ++i) {
            shape.setPoint(i, vertices[i]);
        }
        // Adjust color brightness based on selection status
        sf::Color color = is_selected ? floorColor : sf::Color(floorColor.r, floorColor.g, floorColor.b, 100);
        shape.setFillColor(color);
        return shape;
    }
};

