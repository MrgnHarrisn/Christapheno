#include "Camera.h"

Camera::Camera() : base_move_speed(100.0f), zoom_level(1.0f), zoom_speed(0.1f),
shift_pressed(false), moving_up(false), moving_down(false),
moving_left(false), moving_right(false)
{
    reset(sf::FloatRect(0, 0, 800, 600));
}

Camera::Camera(int width, int height) : base_move_speed(100.0f), zoom_level(1.0f), zoom_speed(0.1f),
shift_pressed(false), moving_up(false), moving_down(false),
moving_left(false), moving_right(false)
{
    reset(sf::FloatRect(0, 0, width, height));
}

void Camera::handle_event(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        bool key_state = (event.type == sf::Event::KeyPressed);

        switch (event.key.code) {
        case sf::Keyboard::LShift:
        case sf::Keyboard::RShift:
            shift_pressed = key_state;
            break;
        case sf::Keyboard::W:
            moving_up = key_state;
            break;
        case sf::Keyboard::S:
            moving_down = key_state;
            break;
        case sf::Keyboard::A:
            moving_left = key_state;
            break;
        case sf::Keyboard::D:
            moving_right = key_state;
            break;
        default:
            break;
        }
    }
    else if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0)
            zoom(1.0f - zoom_speed);
        else
            zoom(1.0f + zoom_speed);

        zoom_level *= (event.mouseWheelScroll.delta > 0) ? (1.0f - zoom_speed) : (1.0f + zoom_speed);
    }
}

void Camera::update(float deltaTime)
{
    float adjusted_speed = get_adjusted_speed();

    if (moving_up) move(0, -adjusted_speed * deltaTime);
    if (moving_down) move(0, adjusted_speed * deltaTime);
    if (moving_left) move(-adjusted_speed * deltaTime, 0);
    if (moving_right) move(adjusted_speed * deltaTime, 0);
}

float Camera::get_adjusted_speed() const {
    // Calculate speed multiplier based on zoom level and whether shift is pressed
    float speed_multiplier = shift_pressed ? 4.0f : 1.0f;
    return base_move_speed * speed_multiplier / sqrt(zoom_level);
}
