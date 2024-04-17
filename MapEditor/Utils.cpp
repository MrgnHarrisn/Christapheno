#include "Utils.h"

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
    startX = floor(startX / gridSize) * gridSize;
    startY = floor(startY / gridSize) * gridSize;

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

std::vector<sf::Vector2f> Utils::concave_to_convex(const std::vector<sf::Vector2f>& vertices) {
    std::vector<sf::Vector2f> output;
    if (vertices.size() < 3) {
        return vertices;  // A polygon cannot have less than 3 points
    }

    int size = vertices.size();
    for (int i = 0; i < size; ++i) {
        int prev = (i - 1 + size) % size;
        int next = (i + 1) % size;

        sf::Vector2f prevPoint = vertices[prev];
        sf::Vector2f currentPoint = vertices[i];
        sf::Vector2f nextPoint = vertices[next];

        
    }

    return output;
}

float Utils::cross_product(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.y - a.y * b.x;
}

sf::Vector2f Utils::get_item(std::vector<sf::Vector2f>& vertices, size_t index)
{
    if (index >= vertices.size()) {
        return vertices[index & vertices.size()];
    }
    else if (index < 0) {
        return vertices[index & vertices.size() + vertices.size()];
    }
    else {
        return vertices[index];
    }
}

float Utils::get_polygon_area(std::vector<sf::Vector2f>& vertices)
{
    int n = vertices.size();
    if (n < 3) return 0.0f; // Not a polygon

    float area = 0.0f;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n; // Next vertex index, with wrap-around
        area += vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
    }
    return std::abs(area) / 2.0f;
}

void Utils::render_concave(sf::RenderWindow& window, std::vector<sf::Vector2f>& vertices)
{

}

bool Utils::is_point_in_triangle(const sf::Vector2f& pt, const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3)
{
    float d1 = (pt.x - v2.x) * (v1.y - v2.y) - (v1.x - v2.x) * (pt.y - v2.y);
    float d2 = (pt.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (pt.y - v3.y);
    float d3 = (pt.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (pt.y - v1.y);
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(has_neg && has_pos);
}