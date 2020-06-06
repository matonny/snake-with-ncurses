#include <iostream>
#include <curses.h>
#include <vector>
#include <zconf.h>

using snakeModel = std::vector < std::vector < int > >;

int maxWidth = 0;
int maxHeight = 0;
int direction = 0;
snakeModel snake = {{1,3}, {1,2}, {1,1}};

int updateDirection(char inputDirection);
void update (const snakeModel &snake);
std::vector<int> moveHead (std::vector<int> head, const int &direction);
snakeModel moveSnake (snakeModel snake, const int &direction);
int main()
{
    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);
    curs_set(0);
    getmaxyx(stdscr, maxHeight, maxWidth);

    for(int i = 0; i <100; i++){
        char inputDirection = getch();
        timeout(1000);
        if(inputDirection > 1 && inputDirection < 6){
            direction = updateDirection(inputDirection);
        }
        snake = moveSnake(snake, direction);
        update(snake);

    }
    getch();
    endwin();
}
int updateDirection (char inputDirection){
    if(inputDirection == 2){
        if(direction != 0){
            return 2;
        }
    }else if(inputDirection == 3){
        if(direction != 2){
            return 0;
        }
    }else if(inputDirection == 4){
        if(direction != 1){
            return 3;
        }
    }else if(inputDirection == 5){
        if(direction != 3){
            return 1;
        }
    }
    return direction;
}
snakeModel moveSnake(snakeModel snake, const int &direction){
    std::vector<int> head = snake[snake.size()-1];
    head = moveHead(head, direction);
    snake.push_back(head);
    snake.erase(snake.begin());
    return snake;
}
void update (const snakeModel &snake){
    clear();
    for(int i = snake.size()-1; i >= 0; i--){
        mvprintw(snake[i][1],snake[i][0], "*");
    }
    refresh();
}
std::vector<int> moveHead (std::vector<int> head, const int &direction){
    if(direction == 0){
        head[1]--;
        if(head[1] < 0){
            (head[1] = maxHeight-1);
        }
    }
    else if(direction == 1){
        head[0]++;
        if(head[0] == maxWidth){
            head[0] = 0;
        }
    }
    else if(direction == 2){
        head[1]++;
        if(head[1] == maxHeight){
            head[1] = 0;
        }
    }
    else if(direction == 3){
        head[0]--;
        if(head[0] < 0){
            head[0] = maxWidth -1;
        }
    }
    return head;
}