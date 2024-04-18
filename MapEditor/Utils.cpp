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

void Utils::draw_grid(sf::RenderWindow& window, const Camera& camera, double grid_size) {
    sf::Vector2f center = camera.getCenter();
    sf::Vector2f size = camera.getSize();
    double left = center.x - size.x / 2;
    double right = center.x + size.x / 2;
    double top = center.y - size.y / 2;
    double bottom = center.y + size.y / 2;

    // Adjust the grid start positions to align with the dynamically calculated grid size
    left = floor(left / grid_size) * grid_size;
    top = floor(top / grid_size) * grid_size;

    sf::Color col(255, 255, 255, 100);

    sf::Vertex line[2];
    line[0].color = col;
    line[1].color = col;

    // Draw vertical grid lines
    for (double x = left; x <= right; x += grid_size) {
        line[0].position = sf::Vector2f(x, top);
        line[1].position = sf::Vector2f(x, bottom);
        window.draw(line, 2, sf::Lines);
    }

    // Draw horizontal grid lines
    for (double y = top; y <= bottom; y += grid_size) {
        line[0].position = sf::Vector2f(left, y);
        line[1].position = sf::Vector2f(right, y);
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

double Utils::get_dynamic_grid_size(double zoomLevel) {
    const int aValues[3] = { 1, 2, 5 };

    // Find the largest power of 10 that is less than or equal to the zoomLevel
    double b = std::floor(std::log10(zoomLevel));

    // Calculate the base spacing by raising 10 to the power of b
    double baseSpacing = std::pow(10, b);

    double spacing = 0;
    // Find the largest 'a' value such that 'a' times 'baseSpacing' is still less than or equal to zoomLevel
    for (int i = 0; i < 3; ++i) {
        double testSpacing = aValues[i] * baseSpacing;
        if (testSpacing <= zoomLevel) {
            spacing = testSpacing;
        }
        else {
            break; // If testSpacing exceeds zoomLevel, the previous spacing was the correct one
        }
    }

    std::cout << "Zoom: " << spacing << "\n";
    return spacing;
}

void Utils::save(std::vector<Sector>& sectors) {
    nfdchar_t* path = nullptr;
    nfdresult_t result = NFD_SaveDialog("rcf", NULL, &path);

    if (result == NFD_OKAY) {
        std::string fpath(path);
        fpath += ".rcf";  // Append extension

        std::ofstream file(fpath);  // Should use fpath, not path

        if (!file.is_open()) {
            printf("Failed to open file\n");
            free(path);  // Make sure to free path before returning
            return;
        }

        /* Store Sector ID which will be the index it is stored */
        for (size_t id = 0; id < sectors.size(); ++id) {
            file << sectors[id].to_str(id != (sectors.size()-1));
        }
        file.close();

        free(path);  // Free the path memory
    }
    else if (result == NFD_CANCEL) {
        printf("User pressed cancel.\n");
        free(path);  // Still need to free path if allocated
    }
    else {  // Handle errors properly
        std::cout << "Error: " << NFD_GetError() << std::endl;
        if (path) {
            free(path);  // Ensure to free path if it's not nullptr
        }
    }
}

void Utils::goto_sectors(std::vector<Sector>& sectors, Camera& camera, size_t sector)
{
    if (sectors.empty() || sector >= sectors.size() || !sectors[sector].initialized) {
        printf("No sectors found!\n");
        camera.setCenter(0, 0);
        return;
    }

    camera.setCenter(sectors[sector].vertices[0]);

}

void Utils::load(std::vector<Sector>& sectors)
{
    

    nfdchar_t* path;

    nfdresult_t result = NFD_OpenDialog("rcf", NULL, &path);

    switch (result) {
    case NFD_ERROR:
        printf("Error: %s\n", NFD_GetError());
        return;
    case NFD_CANCEL:
        printf("User pressed Cancel");
        return;
    case NFD_OKAY:
        puts(path);
        break;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        printf("Error reading file\n");
        return;
    }

    /* Clear the sectors */
    sectors.clear();

    size_t int_temp;
    int r, g, b;
    std::stringstream data;
    data << file.rdbuf();
    std::string line;
    while (data >> r) {
        Sector sector;
        sector.initialized = true;
        sector.ID = r;
        dbg("Sector ID:");
        dbg(sector.ID);
        data >> sector.floor_height >> sector.ceiling_height;
        /* Wall colors */
        data >> r >> g >> b;
        sector.floor_color = sf::Color(r, g, b);
        data >> r >> g >> b;
        sector.ceiling_color = sf::Color(r, g, b);
        data >> r >> g >> b;
        sector.wall_color = sf::Color(r, g, b);
        data >> int_temp;

        /* Walls */
        for (size_t i = 0; i < int_temp; ++i) {
            sf::Vector2f vec;
            data >> vec.x >> vec.y;
            sector.vertices.push_back(vec);
        }
        /* Add sector */
        sectors.push_back(sector);
    }

    for (Sector& s : sectors) {
        dbg(s.to_str());
    }

    file.close();
    free(path);

}


