// Don’t hit a wall and don’t bite your own body.
// Crashing into a wall or your body will end the game immediately.
// 1 points will be added to the player’s score for eating the fruit (*).
// The player’s total score is calculated based on the number of fruits the snake consumed.
// The length of the snake will be increased after eating the fruits.
// Use w, a, s, d to move the snake.

#include <iostream>              
#include <ncurses.h>             
#include <unistd.h>              
#include <cstdlib>

using namespace std;

class SnakeGame
{
private:
    const int width=20;              // width of game area (no.of columns)
    const int height=10;             // height of game area (no.of rows)
    int x,y;                         // x,y - current position of Snake's head
    int fruitX,fruitY;               // fruitX,fruit - position of fruit on grid
    int score;                       // score - player's score     
    int bodyX[100], bodyY[100];      // arrays to store x and y coords of snake's body segments
    int snake_len;                       // no.of body segments
    enum eDirection { HALT = 0, LEFT, RIGHT, UP, DOWN };    

// HALT=0 : represents the state where snake is not moving
// LEFT : By default, if not explicitly assigned a value, LEFT will have the value 1, as it follows HALT.
// RIGHT : Similarly, RIGHT will have the value 2.
// UP : This will have the value 3
// DOWN : This will have the value 4.

    eDirection direction;                 
public:
    bool gameOver;                   // flag to indicate if game has ended or not
    void setup()
    {
        initscr();                   
        timeout(100);                
        cbreak();                    
        noecho();                    
        keypad(stdscr, TRUE);        
        curs_set(0);                 
    
        gameOver = false;            // initialises the game state to not be over
        direction = HALT;
        x = width/2;                 // snake head initialised at middle of grid
        y = height/2;                // same
        fruitX = rand() % width;     // randolmly places fruit's x-coord within grid
        fruitY = rand() % height;    // "           "       "    y-coord    "     "   
        score = 0;
        snake_len = 0;                    // no.of body segments initially = 0
    }

    void Layout()
    {
        clear(); 
        int i=0;                    
        while(i<width+2)
        {
            mvprintw(0, i, "*");     
            i++;                          // makes the top border
        }
    
        for(int j=0;j<height;j++)
        {
            mvprintw(j + 1, 0, "*");     // Layouts the left border of game area
            for(int k=0;k<width;k++)
            {
                
                if(j==y && k==x)
                {
                    mvprintw(j + 1, k + 1, "0");      // Layouts the snake's head at it's current position
                }
                else if(k==fruitX && j==fruitY)
                {
                    mvprintw(j + 1, k + 1, "$");      // Layouts fruit at it's current position
                }
                else
                {
                    bool print=false;  // checks if current position is part of snake's body
                    for(int l=0;l<snake_len;l++)
                    {
                        if(bodyX[l]==k && bodyY[l]==j)
                        {
                            mvprintw(j + 1, k + 1, "o");
                            print=true;
                            break;
                        }
                    }
                    if(!print)
                    {
                        mvprintw(j + 1, k + 1, " ");
                    }
                }
                
            }
            mvprintw(j + 1, width + 1, "*");// right border of game area
        }
        for(int j=0;j<width+2;j++)
        {
            mvprintw(height + 1, j, "*"); // bottom border of the game space
        }
        cout <<  endl;
        mvprintw(height + 2, 0, "Score: %d", score);
    
        refresh();                 
    }

    void input()
    {
        int key = getch();           
        switch(key)
        {
            case KEY_LEFT: 
                if (direction != RIGHT) direction = LEFT;
                break;
            case KEY_RIGHT: 
                if (direction != LEFT) direction = RIGHT;
                break;
            case KEY_DOWN: 
                if (direction != UP) direction = DOWN;
                break;
            case KEY_UP: 
                if (direction != DOWN) direction = UP;
                break;
            case 'q' : 
                gameOver = true;
                break;
        }
    }

    void logic()
    {
        int lastX,lastY,last2X,last2Y,i=1,j=0;           // lastX and lastY to store current position of snake's head i.e., last position of snake's body
        lastX=bodyX[0];                          // last2X and last2Y to store current position of current body segment
        lastY=bodyY[0];
 
        bodyX[0]=x;                 // current head position is saved as first elmt of bodyX and bodyY arrays
        bodyY[0]=y;
        

        while(i<snake_len)
        {
            last2X=bodyX[i];
            last2Y=bodyY[i];

            bodyX[i]=lastX;
            bodyY[i]=lastY;

            lastX=last2X;
            lastY=last2Y;

            i++;
        }

        switch(direction)
        {
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
            case UP:
                y--;
                break;
            case DOWN:
                y++;
                break;
            default:
                break;
        }

        if(x>=width)
        {
            x=0;                // if snake hits right wall then it comes out from left wall
        }
        else if (x<0)
        {
            x=width-1;          // if snake hits left wall, then it comes out from right wall 
        }

        if(y>=height)
        {
            y=0;                // if snake hits bottom wall, then it comes from upper wall
        }
        else if (y<0)
        {
            y=height-1;         // if snake hits upper wall, then it comes out from lower wall
        }

        while(j<snake_len)
        {
            if(bodyX[j]==x && bodyY[j]==y)
            {
                gameOver=true;  // head bumps it's body
                break;
            }

            j++;
        }
        if(x==fruitX && y==fruitY)
        {
            score++;
            fruitX = rand() % width;
            fruitY = rand() % height;
            snake_len++;
        }
    }
};

int main()
{
    SnakeGame S;
    S.setup();
    while(S.gameOver!=true)
    {
        S.Layout();
        S.input();
        S.logic();
        usleep(100000);
    }                           
    endwin();                    
    return 0;
}

