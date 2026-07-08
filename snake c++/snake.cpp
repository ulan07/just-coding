#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

bool gameOver;
int score;
const int width = 20, height = 20;
int x, y;
int fruitx, fruity;

enum eD { stop = 0, LEFT, RIGHT, UP, DOWN };
eD dir;

const int maxTailLength = 100;
int tailX[maxTailLength];     
int tailY[maxTailLength];     
int nTail;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void GotoXY(int x, int y)
{
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, coord);
}

void HideCursor()
{
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void Setup()
{
    gameOver = false;
    dir = stop;
    x = width / 2;
    y = height / 2;
    fruitx = rand() % (width - 2) + 1;
    fruity = rand() % (height - 2) + 1;
    score = 0;
    nTail=0;
}

void Draw()
{
    GotoXY(0, 0);

    for (int i = 0; i < width; i++) {
        cout << "#";
    }
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) {
                cout << "#";
            }
            else if (j == width - 1) {
                cout << "#";
            }
            else if (i == y && j == x) {
                cout << "O";
            }
            else if (i == fruity && j == fruitx) {
                cout << "F";
            }
            else {
                bool tailprin = false;
                for(int k = 0; k < nTail; k++){
                    if(i == tailY[k] && j == tailX[k]){
                        cout << "o";
                        tailprin = true;
                    }
                }                
                if(!tailprin) cout << " ";
            }
        }
        cout << endl;
    }

    for (int i = 0; i < width; i++) {
        cout << "#";
    }

    cout << "\nScore: " << score << endl;
}

void Logic()
{
    int prevx = tailX[0];
    int prevy = tailY[0];
    int prevx1, prevy1;

    tailX[0] = x;
    tailY[0] = y;

    for(int i=1; i<nTail; i++){
        prevx1=tailX[i];
        prevy1=tailY[i];
        tailX[i]=prevx;
        tailY[i]=prevy;
        prevx=prevx1;
        prevy=prevy1;
        }


    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    default: break;
    }

    if(x>=width-1 || x<=0 || y>=height || y<=0) gameOver=true;

    if(x==fruitx && y==fruity){
        score+=1;
        nTail++;
        fruitx = rand() % (width - 2) + 1;
        fruity = rand() % (height - 2) + 1;
    }
}

void Input()
{
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': dir = LEFT; break;
        case 'd': dir = RIGHT; break;
        case 'w': dir = UP; break;
        case 's': dir = DOWN; break;
        case 'x': gameOver = true; break;
        }
    }
}

int main()
{
    system("cls");
    HideCursor();

    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100);
    }

    return 0;
}