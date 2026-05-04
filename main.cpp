#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace sf;
using namespace std;

const int WIDTH = 40;
const int HEIGHT = 30;
const int CELL_SIZE = 20;

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum GameState
{
    Ready,
    Playing,
    GameOver
};

class Snake
{
private:
    vector<Vector2i> segments;
    Direction direction;
    bool alive;
public:
    Snake()
    {
        reset();
    }

    void reset()
    {
        segments = {{10, 10}, {9, 10}, {8, 10}};
        direction = RIGHT;
        alive = true;
    }

    void setDirection(Direction newDirection)
    {
        if ((direction == UP && newDirection == DOWN) ||
            (direction == DOWN && newDirection == UP) ||
            (direction == LEFT && newDirection == RIGHT) ||
            (direction == RIGHT && newDirection == LEFT))
        {
            return;
        }
        direction = newDirection;
    }

    void update()
    {
        if (!alive)
            return;

        for (int i = segments.size() - 1; i > 0; --i)
        {
            segments[i] = segments[i - 1];
        }

        if (direction == UP)
            --segments[0].y;
        else if (direction == DOWN)
            ++segments[0].y;
        else if (direction == LEFT)
            --segments[0].x;
        else if (direction == RIGHT)
            ++segments[0].x;

        if (segments[0].x < 0 || segments[0].x >= WIDTH ||
            segments[0].y < 0 || segments[0].y >= HEIGHT)
        {
            alive = false;
            return;
        }

        for (size_t i = 1; i < segments.size(); ++i)
        {
            if (segments[i] == segments[0])
            {
                alive = false;
                return;
            }
        }
    }

    void grow()
    {
        segments.push_back(segments.back());
    }

    bool occupies(const Vector2i &position) const
    {
        for (const auto &segment : segments)
        {
            if (segment == position)
                return true;
        }
        return false;
    }

    const vector<Vector2i> &getSegments() const { return segments; }
    const Vector2i &getHead() const { return segments[0]; }
    bool isAlive() const { return alive; }


};

class Food
{
private:
    Vector2i position;

    bool isOnSnake(const vector<Vector2i> &snakeSegments) const
    {
        for (const auto &segment : snakeSegments)
        {
            if (segment == position)
                return true;
        }
        return false;
    }
public:
    Food() { position = {0, 0}; }

    void spawn(const vector<Vector2i> &snakeSegments)
    {
        do
        {
            position.x = rand() % WIDTH;
            position.y = rand() % HEIGHT;
        } while (isOnSnake(snakeSegments));
    }

    const Vector2i &getPosition() const { return position; }


};

class Game
{
private:
    RenderWindow window;
    Snake snake;
    Food food;
    Clock clock;
    GameState state;
    float delay;
    int score;
    bool fontLoaded;

    RectangleShape block;
    RectangleShape foodShape;
    Font font;
    Text statusText;
    Text infoText;

    void processEvents()
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case Keyboard::W:
                case Keyboard::Up:
                    snake.setDirection(UP);
                    startIfReady();
                    break;
                case Keyboard::S:
                case Keyboard::Down:
                    snake.setDirection(DOWN);
                    startIfReady();
                    break;
                case Keyboard::A:
                case Keyboard::Left:
                    snake.setDirection(LEFT);
                    startIfReady();
                    break;
                case Keyboard::D:
                case Keyboard::Right:
                    snake.setDirection(RIGHT);
                    startIfReady();
                    break;
                case Keyboard::Enter:
                    if (state == GameOver || state == Ready)
                        restart();
                    break;
                default:
                    break;
                }
            }
        }
    }

    void startIfReady()
    {
        if (state == Ready)
        {
            state = Playing;
            clock.restart();
        }
    }

    void update()
    {
        if (clock.getElapsedTime().asSeconds() < delay)
            return;

        snake.update();
        if (!snake.isAlive())
        {
            state = GameOver;
            return;
        }

        if (snake.getHead() == food.getPosition())
        {
            snake.grow();
            score += 10;
            food.spawn(snake.getSegments());
        }

        clock.restart();
    }

    void render()
    {
        window.clear(Color(20, 20, 20));

        for (const auto &segment : snake.getSegments())
        {
            block.setPosition(segment.x * CELL_SIZE, segment.y * CELL_SIZE);
            window.draw(block);
        }

        foodShape.setPosition(food.getPosition().x * CELL_SIZE,
                              food.getPosition().y * CELL_SIZE);
        window.draw(foodShape);

        if (fontLoaded)
            drawText();

        window.display();
    }

    void drawText()
    {
        string statusMessage;
        if (state == Ready)
            statusMessage = "Press Enter or move to start";
        else if (state == Playing)
            statusMessage = "Score: " + to_string(score);
        else
            statusMessage = "Game Over! Press Enter to restart";

        statusText.setString(statusMessage);
        statusText.setPosition(8.f, 8.f);
        window.draw(statusText);

        if (state != Playing)
        {
            infoText.setString("Use W/A/S/D or arrow keys to move.");
            infoText.setPosition(8.f, 36.f);
            window.draw(infoText);
        }
    }

    void restart()
    {
        snake.reset();
        food.spawn(snake.getSegments());
        score = 0;
        state = Ready;
        clock.restart();
    }
public:
    Game()
        : window(VideoMode(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE), "Snake Game"),
          state(Ready),
          delay(0.15f),
          score(0),
          fontLoaded(false)
    {
        window.setFramerateLimit(60);
        foodShape.setSize(Vector2f(CELL_SIZE, CELL_SIZE));
        foodShape.setFillColor(Color::Red);
        block.setSize(Vector2f(CELL_SIZE, CELL_SIZE));
        block.setFillColor(Color::Green);

        if (font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        {
            fontLoaded = true;
            statusText.setFont(font);
            statusText.setCharacterSize(22);
            statusText.setFillColor(Color::White);
            statusText.setStyle(Text::Bold);

            infoText.setFont(font);
            infoText.setCharacterSize(16);
            infoText.setFillColor(Color::White);
        }

        restart();
    }

    void run()
    {
        while (window.isOpen())
        {
            processEvents();
            if (state == Playing)
                update();
            render();
        }
    }


};

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));
    Game game;
    game.run();
    return 0;
}
