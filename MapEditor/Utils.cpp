#include "Utils.h"

#include <cmath>

void Utils::draw_vertex_markers(sf::RenderWindow& window, const std::vector<sf::Vector2f>& vertices)
{
    float markerSize = 10.0f;  // Size of the rectangle marker
    for (const sf::Vector2f& vertex : vertices) {
        sf::RectangleShape rect(sf::Vector2f(markerSize, markerSize));
        rect.setOrigin(markerSize / 2, markerSize / 2); // Set origin to the center of the rectangle
        rect.setPosition(vertex);
        rect.setFillColor(sf::Color::Red); // Color the marker red for visibility
        window.draw(rect);
    }
}

void Utils::draw_shape(sf::RenderWindow& window, const std::vector<sf::Vector2f>& vertices)
{
    if (vertices.size() < 2) return; // Not enough points to form a line

    // Draw the shape being currently created
    thor::ConcaveShape tempShape;
    tempShape.setPointCount(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        tempShape.setPoint(i, vertices[i]);
    }
    tempShape.setFillColor(sf::Color(100, 100, 250, 255)); // Semi-transparent
    window.draw(tempShape);
}

void Utils::draw_grid(sf::RenderWindow& window, const Camera& camera)
{
    float gridSize = 50.0f; // Grid spacing
    sf::Vector2f size = window.getView().getSize();
    sf::Vector2f center = window.getView().getCenter();

    // Start positions for grid lines based on the camera view
    float startX = center.x - size.x / 2;
    float startY = center.y - size.y / 2;
    float endX = center.x + size.x / 2;
    float endY = center.y + size.y / 2;

    // Adjust the start and end to be on the grid
    startX = std::floor(startX / gridSize) * gridSize;
    startY = std::floor(startY / gridSize) * gridSize;

    sf::Vertex line[2];
    line[0].color = sf::Color::White;
    line[1].color = sf::Color::White;

    // Draw vertical grid lines
    for (float x = startX; x <= endX; x += gridSize) {
        line[0].position = sf::Vector2f(x, startY);
        line[1].position = sf::Vector2f(x, endY);
        window.draw(line, 2, sf::Lines);
    }

    // Draw horizontal grid lines
    for (float y = startY; y <= endY; y += gridSize) {
        line[0].position = sf::Vector2f(startX, y);
        line[1].position = sf::Vector2f(endX, y);
        window.draw(line, 2, sf::Lines);
    }
}

sf::Vector2f Utils::snap_to_grid(const sf::Vector2f& point, float gridSize)
{
    float x = round(point.x / gridSize) * gridSize;
    float y = round(point.y / gridSize) * gridSize;
    return sf::Vector2f(x, y);
}

sf::Color Utils::float_3_color(float color[3])
{
    return sf::Color(
        static_cast<unsigned char>(color[0] * 255),
        static_cast<unsigned char>(color[1] * 255),
        static_cast<unsigned char>(color[2] * 255),
        255  // Alpha value set to 255 for full opacity
    );
}

void Utils::color_float_3(sf::Color color, float output[3])
{
    output[0] = color.r / 255.0f;
    output[1] = color.g / 255.0f;
    output[2] = color.b / 255.0f;
}


bool Utils::is_overlapping_vec(sf::Vector2f target, const std::vector<sf::Vector2f>& others, int index)
{

    if (others.size() == 0) { return true; }

    for (size_t i = 0; i < others.size(); ++i) {
        if (i == index) continue;

        if (target == others[i]) {
            return false;
        }

    }

    return true;

}
