
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "Sector.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <string>

using namespace sf;

enum Mode {
    VISUAL,     // Pressing V
    CREATE,     // Pressing C
    EDIT,       // Pressing E
};

// Prototype for the grid drawing function
void draw_grid(RenderWindow& window, const Camera& camera);
void draw_temporary_shape(RenderWindow& window, const std::vector<Vector2f>& vertices);
Vector2f snap_to_grid(const Vector2f& point, float gridSize);
void draw_vertex_markers(RenderWindow& window, const std::vector<Vector2f>& vertices);
void SetupImGuiStyle();

std::vector<Sector> sectors;
int selectedVertexIndex = -1;
Vector2f offset;
bool vertexSelected = false;
int main() {
    RenderWindow window(VideoMode(900, 700), "Map Editor", Style::None | Style::Close);
    window.setFramerateLimit(60);
    
    
    ImGui::SFML::Init(window);
    SetupImGuiStyle();

    Camera camera(900, 700);
    Clock clock;
    std::vector<Vector2f> vertices;
    sectors.push_back(*(new Sector));
    int current_sector = 0;      // keep track of the sector we are adding to or editing
    // 100, 100, 250
    float default_color[3] = {(float)100/255, (float)100/255, (float)255/255};

    Mode currentMode = VISUAL;
    float gridSize = 50.0f; // Grid size for snapping

    // Main game loop
    while (window.isOpen()) {
        Event event;

        ImGui::SFML::ProcessEvent(event);

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            camera.handle_event(event);

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::C) {
                    currentMode = CREATE;
                    vertices.clear(); // Clear previous vertices
                }
                else if (event.key.code == Keyboard::V) {
                    currentMode = VISUAL;
                }
                else if (event.key.code == Keyboard::E) {
                    currentMode = EDIT;
                }
                else if (event.key.code == Keyboard::Enter && currentMode == CREATE && !vertices.empty()) {
                    if (vertices.size() > 2) {
                        Sector sector;
                        sector.vertices = vertices;
                        Color col((int)(default_color[0]*255), (int)(default_color[1]*255), (int)(default_color[2]*255));
                        sector.floorColor = (col); // Semi-transparent
                        sectors[current_sector] = sector;
                        sectors[current_sector].initialized = true; // makes it so that we know what is actually initialized
                        currentMode = VISUAL;   // stops you from drawing imediately after
                    }
                    else {
                        // They tried to make a convex shape with less than 3 points
                        printf("Invalid Shape!\n");
                        continue;
                    }
                    // clear the stuff
                    vertices.clear();
                }
                else if (event.key.code == Keyboard::Right) {
                    if (current_sector + 1 <= sectors.size() && sectors[current_sector].initialized) {
                        if (sectors.size() == current_sector + 1) {
                            sectors.push_back(*(new Sector));
                        }
                        current_sector++;
                        
                    }
                }
                else if (event.key.code == Keyboard::Left) {
                    if (current_sector - 1 >= 0) {
                        current_sector--;
                    }
                }
                else if (event.key.code == Keyboard::Q) {
                    for (int i = 0; i < sectors.size(); i++) {
                        std::cout << "Sector: " << i << " is " << (sectors[i].initialized ? "" : "not ") << "initialized.\n";
                    }
                }
                
            }

            if (currentMode == EDIT) {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mouseWorldPos = window.mapPixelToCoords(Mouse::getPosition(window));
                    for (size_t i = 0; i < sectors[current_sector].vertices.size(); ++i) {
                        FloatRect vertexRect(sectors[current_sector].vertices[i].x - 5, sectors[current_sector].vertices[i].y - 5, 10, 10);

                        if (vertexRect.contains(mouseWorldPos)) {
                            std::cout << "Vertex selected at index: " << i << std::endl;
                        }
                        else {
                            std::cout << "No vertex selected" << std::endl;
                        }

                        if (vertexRect.contains(mouseWorldPos)) {
                            selectedVertexIndex = i;
                            vertexSelected = true;
                            break;
                        }
                    }
                }
                else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right) {
                    printf("Put down vertex\n");
                    vertexSelected = false;
                    selectedVertexIndex = -1;
                }
                else if (event.type == Event::MouseMoved && vertexSelected == true) {
                    if (selectedVertexIndex != -1) {
                        sectors[current_sector].vertices[selectedVertexIndex] = window.mapPixelToCoords(Mouse::getPosition(window));
                    }
                }
            }

            if (currentMode == CREATE && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f worldPos = window.mapPixelToCoords(Mouse::getPosition(window));
                vertices.push_back(snap_to_grid(worldPos, gridSize));
            }
        }

        sf::Time time = clock.restart();
        float deltaTime = time.asSeconds();
        camera.update(deltaTime);
        window.clear();

        window.setView(camera);
        draw_grid(window, camera);

        if (currentMode == EDIT) {
            for (const auto& vertex : vertices) {
                RectangleShape vertexMarker(Vector2f(10, 10));
                vertexMarker.setPosition(vertex.x - 5, vertex.y - 5);
                vertexMarker.setFillColor(Color::Red);
                window.draw(vertexMarker);
            }
        }

        // Draw all sectors

        for (int i = 0; i < sectors.size(); i++) {
            /* Check that it isn't an empty sector */
            if (sectors[i].initialized) {
                window.draw(sectors[i].get_shape(i == current_sector));
            }
        }

        if (currentMode == EDIT && sectors[current_sector].initialized) {
            draw_vertex_markers(window, sectors[current_sector].vertices);
        }

        draw_temporary_shape(window, vertices);

        ImGui::SFML::Update(window, time);

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    // New action
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    // Open action
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    // Save action
                }
                if (ImGui::MenuItem("Save As..")) {
                    // Save As action
                }
                if (ImGui::MenuItem("Quit", "Alt+F4")) {
                    window.close();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                // Edit menu items here...
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Options")) {
                // Options menu items here...
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        
        ImGui::Begin("Edit Tools");
        std::string _mode = "Mode: ";
        if (currentMode == VISUAL) {
            _mode += "VISUAL";
        }
        else if (currentMode == EDIT) {
            _mode += "EDIT";
        }
        else {
            _mode += "CREATE";
        }
        ImGui::Text(_mode.c_str());
        ImGui::Text("Current Sector: %i", current_sector);
        if (sectors[current_sector].initialized) {
            ImGui::ColorEdit3("Floor Color", default_color);
            /* Need to make sure that the sector exists */
            ImGui::SliderFloat("Floor Height", &sectors[current_sector].floorHeight, -1, 1);
            ImGui::SliderFloat("Ceiling Height", &sectors[current_sector].ceilingHeight, -1, 1);
            sectors[current_sector].floorColor = Color((int)(default_color[0] * 255), (int)(default_color[1] * 255), (int)(default_color[2] * 255));
        }
        else {
            ImGui::Text("No Convex Shape");
        }
        ImGui::End();

        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}

/// <summary>
/// Draws a grid of lines
/// </summary>
/// <param name="window">RenderTarget</param>
/// <param name="camera">View</param>
void draw_grid(RenderWindow& window, const Camera& camera) {
    float gridSize = 50.0f; // Grid spacing
    Vector2f size = window.getView().getSize();
    Vector2f center = window.getView().getCenter();

    // Start positions for grid lines based on the camera view
    float startX = center.x - size.x / 2;
    float startY = center.y - size.y / 2;
    float endX = center.x + size.x / 2;
    float endY = center.y + size.y / 2;

    // Adjust the start and end to be on the grid
    startX = floor(startX / gridSize) * gridSize;
    startY = floor(startY / gridSize) * gridSize;

    Vertex line[2];
    line[0].color = Color::White;
    line[1].color = Color::White;

    // Draw vertical grid lines
    for (float x = startX; x <= endX; x += gridSize) {
        line[0].position = Vector2f(x, startY);
        line[1].position = Vector2f(x, endY);
        window.draw(line, 2, Lines);
    }

    // Draw horizontal grid lines
    for (float y = startY; y <= endY; y += gridSize) {
        line[0].position = Vector2f(startX, y);
        line[1].position = Vector2f(endX, y);
        window.draw(line, 2, Lines);
    }
}

/// <summary>
/// Draws a temporary shape
/// </summary>
/// <param name="window">RenderTarget</param>
/// <param name="vertices">Vector2f[]</param>
void draw_temporary_shape(RenderWindow& window, const std::vector<Vector2f>& vertices) {
    if (vertices.size() < 2) return; // Not enough points to form a line

    // Draw the shape being currently created
    ConvexShape tempShape;
    tempShape.setPointCount(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        tempShape.setPoint(i, vertices[i]);
    }
    tempShape.setFillColor(Color(100, 100, 250, 255)); // Semi-transparent
    window.draw(tempShape);
}

/// <summary>
/// Snaps a given point to the grid
/// </summary>
/// <param name="point">Vector2f</param>
/// <param name="gridSize">int</param>
/// <returns>Vector2f</returns>
Vector2f snap_to_grid(const Vector2f& point, float gridSize) {
    float x = round(point.x / gridSize) * gridSize;
    float y = round(point.y / gridSize) * gridSize;
    return Vector2f(x, y);
}

/// <summary>
/// draw a rectangle at each vertex of the sector
/// </summary>
/// <param name="window">RenderTarget</param>
/// <param name="vertices">Vector2f[]</param>
void draw_vertex_markers(RenderWindow& window, const std::vector<Vector2f>& vertices) {
    float markerSize = 10.0f;  // Size of the rectangle marker
    for (const Vector2f& vertex : vertices) {
        RectangleShape rect(Vector2f(markerSize, markerSize));
        rect.setOrigin(markerSize / 2, markerSize / 2); // Set origin to the center of the rectangle
        rect.setPosition(vertex);
        rect.setFillColor(Color::Red); // Color the marker red for visibility
        window.draw(rect);
    }
}

/// <summary>
/// Initialize ImGui styles
/// </summary>
void SetupImGuiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    ImVec4* colors = style.Colors;

    // The colors are converted from hexadecimal to normalized RGBA
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f); // Text: #ffffff
    colors[ImGuiCol_WindowBg] = ImVec4(0.23f, 0.25f, 0.31f, 1.00f); // Menus and windows: #3a414e
    colors[ImGuiCol_ChildBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f); // Combo box and inside of stuff: #333845
    colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.94f); // Popup background, slightly transparent
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.23f, 0.25f, 0.31f, 1.00f); // Menus and windows
    colors[ImGuiCol_Header] = ImVec4(0.76f, 0.20f, 0.29f, 1.00f); // Highlighted items: #c1334b
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.76f, 0.20f, 0.29f, 0.80f); // Highlighted when hovered
    colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.20f, 0.29f, 1.00f); // Highlighted when active/clicked

    colors[ImGuiCol_Button] = ImVec4(0.26f, 0.33f, 0.38f, 1.00f); // Buttons: #435361
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.33f, 0.38f, 0.80f); // Buttons when hovered
    colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.33f, 0.38f, 1.00f); // Buttons when active

    // FrameBg is for the background of checkbox, radio button, plot, textbox, etc.
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f); // ComboBox text input and inside of stuff: #333845
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.22f, 0.27f, 0.80f); // Lighter when hovered
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f); // When active

    // Apply the theme to other elements as needed

    style.FrameBorderSize = 1.0f;
    style.WindowRounding = 5.0f;
}


