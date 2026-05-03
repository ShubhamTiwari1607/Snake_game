#include <SFML/Graphics.hpp>

using namespace sf;

const int CELL_SIZE = 20;
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
int main()
{
    RenderWindow window(VideoMode(800, 600), "Snake Game");
    RectangleShape snake(Vector2f(20.f, 20.f));
    snake.setPosition(100.f, 100.f);

    int x = 10;
    int y = 10;

    Direction dir = RIGHT;
    Clock clock;
    float delay = 0.15f;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::W))
            dir = UP;
        if (Keyboard::isKeyPressed(Keyboard::S))
            dir = DOWN;
        if (Keyboard::isKeyPressed(Keyboard::A))
            dir = LEFT;
        if (Keyboard::isKeyPressed(Keyboard::D))
            dir = RIGHT;
        if (clock.getElapsedTime().asSeconds() > delay)
        {
            if (dir == UP)
                y--;
            if (dir == DOWN)
                y++;
            if (dir == LEFT)
                x--;
            if (dir == RIGHT)
                x++;
            clock.restart();
        }
        snake.setPosition(x * CELL_SIZE, y * CELL_SIZE);
        window.clear();
        window.draw(snake);
        window.display();
    }
    return 0;
}