#include "Editor.h"

Editor::Editor(int width, int height)
{
    m_window = new sf::RenderWindow(sf::VideoMode(width, height), "Map Editor", sf::Style::None | sf::Style::Close);
    m_window->setFramerateLimit(60);

    

    m_camera = new Camera(width, height);
    m_mode = VISUAL;    // always visual by default

    // add first sector so we don't crash
    m_sectors.push_back(*(new Sector));

    /* default cloor, ceiling and wall colors */

    for (int i = 0; i < 3; ++i) {
        m_floor_color[i] = (i == 2 ? 255.f : 100.f) / 255;
        m_wall_color[i] = (i == 2 ? 255.f : 100.f) / 255;
        m_ceiling_color[i] = (i == 2 ? 255.f : 100.f) / 255;
    }

}

void Editor::start()
{

    // initialize imgui-sfml
    ImGui::SFML::Init(*m_window);
    setup_imgui_style();
    m_clock.restart();
    while (is_open())
    {

        /* Delta time */
        m_time = m_clock.restart();
        m_dt = m_time.asSeconds();

        handle_events();
        update();
    }
    close();
}

void Editor::update()
{
    
    m_camera->update(m_dt);

    m_window->clear();

    m_window->setView(*m_camera);

    /* Rendering here */

    Utils::draw_grid(*m_window, *m_camera);

    for (int i = 0; i < m_sectors.size(); i++) {
        /* Check that it isn't an empty sector */
        if (m_sectors[i].initialized) {
            m_window->draw(m_sectors[i].get_shape(i == m_current_sector));
        }
    }

    if ((m_mode == EDIT || m_mode == MOVE) && m_sectors[m_current_sector].initialized) {
        Utils::draw_vertex_markers(*m_window, m_sectors[m_current_sector].vertices);
    }

    Utils::draw_shape(*m_window, m_vertices);

    /* UI comes last! */
    render_imgui(); // bug here

    m_window->display();

}

bool Editor::is_open()
{
    return m_window->isOpen();
}

void Editor::close()
{
    m_window->close();
    ImGui::SFML::Shutdown();
}

void Editor::render_imgui()
{
    ImGui::SFML::Update(*m_window, m_clock.restart());

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N")) {
                m_mode = VISUAL;
                m_sectors.clear();
                m_current_sector = 0;
                m_sectors.push_back(*(new Sector));
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
                close();
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
    if (m_mode == VISUAL) {
        _mode += "VISUAL";
    }
    else if (m_mode == EDIT) {
        _mode += "EDIT";
    }
    else if (m_mode == MOVE) {
        _mode += "MOVE";
    }
    else {
        _mode += "CREATE";
    }
    ImGui::Text(_mode.c_str());
    ImGui::Text("Vertices: %i", m_vertices.size());
    ImGui::Text("Current Sector: %i", m_current_sector);
    if (m_sectors[m_current_sector].initialized) {
        ImGui::ColorEdit3("Floor Color", m_floor_color);
        ImGui::ColorEdit3("Wall Color", m_wall_color);
        ImGui::ColorEdit3("Ceiling Color", m_ceiling_color);
        /* Need to make sure that the sector exists */
        ImGui::SliderFloat("Floor Height", &m_sectors[m_current_sector].floor_height, -1, 1);
        ImGui::SliderFloat("Ceiling Height", &m_sectors[m_current_sector].ceiling_height, -1, 1);
        m_sectors[m_current_sector].floor_color = Utils::float_3_color(m_floor_color);
        m_sectors[m_current_sector].ceiling_color = Utils::float_3_color(m_ceiling_color);
        m_sectors[m_current_sector].wall_color = Utils::float_3_color(m_wall_color);
    }
    else {
        /* No convex shape in the sector */
        ImGui::Text("No Convex Shape");
    }
    ImGui::End();

    ImGui::SFML::Render(*m_window);
}

void Editor::setup_imgui_style()
{
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

void Editor::handle_events()
{
    
    sf::Event event;

    while (m_window->pollEvent(event))
    {

        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed)
            m_window->close();

        m_camera->handle_event(event);

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::C:
                m_mode = CREATE;
                m_vertices.clear();
                break;
            case sf::Keyboard::V:
                m_mode = VISUAL;
                break;
            case sf::Keyboard::E:
                m_mode = EDIT;
                break;
            case sf::Keyboard::M:
                m_mode = MOVE;
                break;
            case sf::Keyboard::Enter:
                if (m_mode == CREATE && !m_vertices.empty()) {
                    if (m_vertices.size() > 2) {
                        Sector sector;
                        sector.vertices = m_vertices;

                        sector.floor_color = Utils::float_3_color(m_floor_color);
                        sector.ceiling_color = Utils::float_3_color(m_ceiling_color);
                        sector.wall_color = Utils::float_3_color(m_wall_color);
                        m_sectors[m_current_sector] = sector;
                        m_sectors[m_current_sector].initialized = true; // makes it so that we know what is actually initialized
                        m_mode = VISUAL;   // stops you from drawing imediately after
                    }
                    else {
                        // They tried to make a convex shape with less than 3 points
                        printf("Invalid Shape!\n");
                        continue;
                    }
                    // clear the stuff
                    m_vertices.clear();
                }
                break;
            case sf::Keyboard::Right:
                if (m_current_sector + 1 <= m_sectors.size() && m_sectors[m_current_sector].initialized) {
                    if (m_sectors.size() == m_current_sector + 1) {
                        m_sectors.push_back(*(new Sector));
                    }
                    else {
                        Utils::color_float_3(m_sectors[m_current_sector + 1].floor_color, m_floor_color);
                        Utils::color_float_3(m_sectors[m_current_sector + 1].wall_color, m_wall_color);
                        Utils::color_float_3(m_sectors[m_current_sector + 1].ceiling_color, m_ceiling_color);
                    }
                    m_current_sector++;

                }
                break;
            case sf::Keyboard::Left:
                if (m_current_sector - 1 >= 0) {
                    m_current_sector--;
                    Utils::color_float_3(m_sectors[m_current_sector].floor_color, m_floor_color);
                    Utils::color_float_3(m_sectors[m_current_sector].wall_color, m_wall_color);
                    Utils::color_float_3(m_sectors[m_current_sector].ceiling_color, m_ceiling_color);
                }
                break;
            }
        }

        if (m_mode == EDIT || m_mode == MOVE) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mouseWorldPos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
                for (size_t i = 0; i < m_sectors[m_current_sector].vertices.size(); ++i) {
                    sf::FloatRect vertexRect(m_sectors[m_current_sector].vertices[i].x - 5, m_sectors[m_current_sector].vertices[i].y - 5, 10, 10);

                    if (vertexRect.contains(mouseWorldPos)) {
                        m_selected_vertex = i;
                        m_vertex_is_selected = true;
                        break;
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                m_vertex_is_selected = false;
                m_selected_vertex = -1;
            }
            else if (event.type == sf::Event::MouseMoved && m_vertex_is_selected == true) {
                if (m_selected_vertex != -1) {
                    if (m_mode == EDIT) {
                        // Snap new position to grid
                        sf::Vector2f new_pos = Utils::snap_to_grid(m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)), grid_size);

                        // If there's no vertex close to the new position, update the selected vertex position
                        
                        if (Utils::is_overlapping_vec(new_pos, m_sectors[m_current_sector].vertices, m_selected_vertex)) {
                            m_sectors[m_current_sector].vertices[m_selected_vertex] = new_pos;
                        }
                    }
                    else {
                        sf::Vector2f new_pos = Utils::snap_to_grid(m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window)), grid_size);
                        sf::Vector2f delta = new_pos - m_sectors[m_current_sector].vertices[m_selected_vertex];
                        for (sf::Vector2f& v : m_sectors[m_current_sector].vertices) {
                            v += delta;
                        }
                    }
                }
            }
        }

        if (m_mode == CREATE && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f worldPos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
            if (Utils::is_overlapping_vec(Utils::snap_to_grid(worldPos, grid_size), m_vertices)) {
                m_vertices.push_back(Utils::snap_to_grid(worldPos, grid_size));
            }
            
        }
    }

}
