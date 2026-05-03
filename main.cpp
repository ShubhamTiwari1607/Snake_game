#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

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

    vector<Vector2i> snake = {Vector2i(10, 10), Vector2i(9, 10), Vector2i(8, 10)};
    RectangleShape snakeShape(Vector2f(CELL_SIZE, CELL_SIZE));
    snakeShape.setPosition(100.f, 100.f);

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
            for (int i = snake.size() - 1; i > 0; i--)
            {
                snake[i] = snake[i - 1];
            }
            if (dir == UP)
                snake[0].y--;
            if (dir == DOWN)
                snake[0].y++;
            if (dir == LEFT)
                snake[0].x--;
            if (dir == RIGHT)
                snake[0].x++;
            clock.restart();
        }

        window.clear();
        for (auto segment : snake)
        {
            snakeShape.setPosition(segment.x * CELL_SIZE,
                                   segment.y * CELL_SIZE);
            window.draw(snakeShape);
        }
        window.display();
    }
    return 0;
}