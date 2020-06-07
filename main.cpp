#include <iostream>
#include <curses.h>
#include <vector>
#include <random>
#include <zconf.h>


int maxWidth = 50;
int maxHeight = 50;

enum directions {
    bottom = 0,
    top = 1,
    left = 2,
    right = 3
};
struct gameObject{
    int x;
    int y;
};
using snakeModel = std::vector< gameObject >;

gameObject spawnFood();

int randomNumberInRange(const int &min, const int &max);

int updateDirection(const int &inputDirection, const int &currentDirection);

void update(const snakeModel &snake, const gameObject &food, const char score[]);

gameObject moveHead(gameObject head, const int &direction);

snakeModel moveSnake(snakeModel snake, const int &direction);

bool isInSnake(const int &targetX, const int &targetY, const snakeModel &snake);

int randomNumberInRange(const int &min, const int &max);

void gameLoop();

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
    while(true){
        clear();
        mvprintw(0,0, "Welcome to snake!");
        mvprintw(1,0, "1. Snake without obstacles\n");
        printw("2. Quit \n");
        char userInput = getch();
        if(userInput == '1'){
            gameLoop();
        }else if(userInput == '2'){
            break;
        }else{
            printw("Not a proper option");
        }
    }
    refresh();
    getch();
    endwin();
}
void gameLoop(){
    struct gameObject food = spawnFood();
    int score = 0;

    snakeModel snake = {{1, 1},
                        {1, 2},
                        {1, 3}};
    int direction = directions(bottom);
    while (true)
    {


        snake = moveSnake(snake, direction);
        struct gameObject snakeHead = snake[snake.size() - 1];
        snakeModel snakeBody = snake;
        snakeBody.pop_back(); //removes head
        if (isInSnake(food.x, food.y, snake))//checks if snake ate
        {
            food = spawnFood();
            score += 10;

        } else
        {
            snake.erase(snake.begin());
        }
        if (isInSnake(snakeHead.x, snakeHead.y, snakeBody))
        {
            char loseMessage[] = "You lost! Going back to menu";
            mvprintw(maxHeight/2, (maxWidth/2)-sizeof(loseMessage)/2, loseMessage);
            refresh();
            getch();
            break;
        }
        std::string s = std::to_string(score);
        char const *pchar = s.c_str();
        update(snake, food, pchar);


        timeout(1000);
        int inputDirection = getch();
        if (inputDirection > 257 && inputDirection < 262)//range of numbers for arrow keys
        {
            direction = updateDirection((inputDirection-258), direction); //-258 used to assign numbers 0-3 to arrow keys
        }
    }
}
int updateDirection(const int &inputDirection, const int &currentDirection)
{
    if (inputDirection == directions(bottom))
    {
        if (currentDirection != directions(top))
        {
            return directions(bottom);
        }
    } else if (inputDirection == directions(top))
    {
        if (currentDirection != directions(bottom))
        {
            return directions(top);
        }
    } else if (inputDirection == directions(left))
    {
        if (currentDirection != directions(right))
        {
            return directions(left);
        }
    } else if (inputDirection == directions(right))
    {
        if (currentDirection != directions(left))
        {
            return directions(right);
        }
    }
    return currentDirection;
}

snakeModel moveSnake(snakeModel snake, const int &direction)
{
    gameObject head = snake[snake.size() - 1];
    head = moveHead(head, direction);
    snake.push_back(head);
    return snake;
}

void update(const snakeModel &snake, const gameObject &food, const char *score)
{
    clear();
    attron(COLOR_PAIR(1));
    for (int i = snake.size() - 1; i >= 0; i--)
    {
        mvprintw(snake[i].y, snake[i].x, "*");
    }
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
    mvprintw(food.y, food.x, "*");
    attroff(COLOR_PAIR(2));
    mvprintw(maxHeight, 0, "Score: ");
    mvprintw(maxHeight, 7, score);
    refresh();
}

gameObject moveHead(gameObject head, const int &direction)
{
    if (direction == directions(top))
    {
        head.y--;
        if (head.y < 0)
        {
            (head.y = maxHeight - 1);
        }
    } else if (direction == directions(right))
    {
        head.x++;
        if (head.x == maxWidth)
        {
            head.x = 0;
        }
    } else if (direction == directions(bottom))
    {
        head.y++;
        if (head.y == maxHeight)
        {
            head.y = 0;
        }
    } else if (direction == directions(left))
    {
        head.x--;
        if (head.x < 0)
        {
            head.x = maxWidth - 1;
        }
    }
    return head;
}

gameObject spawnFood()
{
    struct gameObject food;
    food.x = randomNumberInRange(0, maxWidth - 1);
    food.y = randomNumberInRange(0, maxHeight - 1);
    return food;
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
        if (targetX == snake[i].x && targetY == snake[i].y)
        {
            return true;
        }
    }
    return false;
}
