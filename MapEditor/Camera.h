#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

class Camera : public sf::View
{
public:

    /// <summary>
    /// Default constructor
    /// </summary>
    Camera();


    /// <summary>
    /// Constructor with size
    /// </summary>
    /// <param name="width">int</param>
    /// <param name="height">int</param>
    Camera(int width, int height);

    // Update method to be called every frame
    void update(float deltaTime);

    // Function to handle input events
    void handle_event(const sf::Event& event);

private:
    float base_move_speed; // Base speed of camera movement
    float zoom_level;      // Current zoom level
    float zoom_speed;      // Speed of zooming in/out

    // Movement flags
    bool shift_pressed;    // Indicates if Shift key is pressed
    bool moving_up;
    bool moving_down;
    bool moving_left;
    bool moving_right;

    // Adjust move speed based on zoom level and shift key
    float get_adjusted_speed() const;
};
