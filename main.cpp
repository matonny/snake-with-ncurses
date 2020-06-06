#include <iostream>
#include <curses.h>
#include <vector>
#include <random>
#include <zconf.h>

using snakeModel = std::vector<std::vector<int> >;

int maxWidth = 50;
int maxHeight = 50;

std::vector<int> spawnFood();

int randomNumberInRange(const int &min, const int &max);

int updateDirection(const char &inputDirection, const int &currentDirection);

void update(const snakeModel &snake, const std::vector<int> &food);

std::vector<int> moveHead(std::vector<int> head, const int &direction);

snakeModel moveSnake(snakeModel snake, const int &direction);

bool isInSnake(const int &targetX, const int &targetY, const snakeModel &snake);

int randomNumberInRange(const int &min, const int &max);

int main()
{
    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    std::vector<int> food = spawnFood();
    snakeModel snake = {{1, 3},

                        {1, 2},
                        {1, 1}};
    int direction = 0;
    while (true)
    {
        char inputDirection = getch();
        timeout(1000);
        if (inputDirection > 1 && inputDirection < 6)
        {
            direction = updateDirection(inputDirection, direction);
        }

        snake = moveSnake(snake, direction);
        std::vector<int> snakeHead = snake[snake.size() - 1];
        snakeModel snakeBody = snake;
        snakeBody.pop_back(); //removes head
        if (isInSnake(food[0], food[1], snake))
        {
            food = spawnFood();
        } else
        {
            snake.erase(snake.begin());
        }
        if (isInSnake(snakeHead[0], snakeHead[1], snakeBody))
        {
            break;
        }
        update(snake, food);
    }
    getch();
    endwin();
}

int updateDirection(const char &inputDirection, const int &currentDirection)
{
    if (inputDirection == 2)
    {
        if (currentDirection != 0)
        {
            return 2;
        }
    } else if (inputDirection == 3)
    {
        if (currentDirection != 2)
        {
            return 0;
        }
    } else if (inputDirection == 4)
    {
        if (currentDirection != 1)
        {
            return 3;
        }
    } else if (inputDirection == 5)
    {
        if (currentDirection != 3)
        {
            return 1;
        }
    }
    return currentDirection;
}

snakeModel moveSnake(snakeModel snake, const int &direction)
{
    std::vector<int> head = snake[snake.size() - 1];
    head = moveHead(head, direction);
    snake.push_back(head);
    return snake;
}

void update(const snakeModel &snake, const std::vector<int> &food)
{
    clear();
    attron(COLOR_PAIR(1));
    for (int i = snake.size() - 1; i >= 0; i--)
    {
        mvprintw(snake[i][1], snake[i][0], "*");
    }
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
    mvprintw(food[1], food[0], "*");
    attroff(COLOR_PAIR(2));
    refresh();
}

std::vector<int> moveHead(std::vector<int> head, const int &direction)
{
    if (direction == 0)
    {
        head[1]--;
        if (head[1] < 0)
        {
            (head[1] = maxHeight - 1);
        }
    } else if (direction == 1)
    {
        head[0]++;
        if (head[0] == maxWidth)
        {
            head[0] = 0;
        }
    } else if (direction == 2)
    {
        head[1]++;
        if (head[1] == maxHeight)
        {
            head[1] = 0;
        }
    } else if (direction == 3)
    {
        head[0]--;
        if (head[0] < 0)
        {
            head[0] = maxWidth - 1;
        }
    }
    return head;
}

std::vector<int> spawnFood()
{
    int foodX = 0;
    int foodY = 0;
    foodX = randomNumberInRange(0, maxWidth - 1);
    foodY = randomNumberInRange(0, maxHeight - 1);
    return {foodX, foodY};
}

int randomNumberInRange(const int &min, const int &max)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(dev);
}

bool isInSnake(const int &targetX, const int &targetY, const snakeModel &snake)
{
    for (int i = snake.size() - 1; i >= 0; i--)
    {
        if (targetX == snake[i][0] && targetY == snake[i][1])
        {
            return true;
        }
    }
    return false;
}
