#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>       /* time */

bool running = true;
double TetrisTime = 0;
bool pieceSetDown = false;

void ClearScreen(){
    COORD position;
    position.X = 0;
    position.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);//This is the way, just remember the format
}

// Colors for the console
const unsigned short BRIGHT_BLUE = 11;
const unsigned short BRIGHT_RED = 12;
const unsigned short BRIGHT_GREEN = 10;
const unsigned short BRIGHT_YELLOW = 14;
const unsigned short BRIGHT_PINK = 13;
const unsigned short BRIGHT_ORANGE = 6;
const unsigned short DARK_BLUE = 9;
const unsigned short DARK = 8;
const unsigned short WHITE = 15;

void setConsoleColour(unsigned short color){
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    SetConsoleTextAttribute(hOut, color);
}

// Console colors

char GameBoard[16][21] = { // The game board where the pieces will be displayed
    "####################",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "####################"
};

char Tetrominos[7][4][5] = {
    {
        "X   ",
        "X   ",
        "X   ",
        "X   "
    },
    {
        "H   ",
        "HH  ",
        " H  ",
        "    "
    },
    {
        " U  ",
        "UU  ",
        "U   ",
        "    "
    },
    {
        "SS  ",
        "SS  ",
        "    ",
        "    "
    },
    {
        "KK  ",
        " K  ",
        " K  ",
        "    "
    },
    {
        "OO  ",
        "O   ",
        "O   ",
        "    "
    },
    {
        "G   ",
        "GG  ",
        "G   ",
        "    "
    },
};

struct TertrominoVertex{
    short y, x;
};

class Tertromino{
    public:
    short x = 10, y = 1;
    int TetroNum; // The array index of the tetromino
    char disp;
    TertrominoVertex vertexes[4];
    Tertromino(int _TetroNum){
        TetroNum = _TetroNum;
        short numOfNonWhiteSpace = 0;
        for (short i = 0; i < 4; i++){
            for (short j = 0; j < 4; j++){
                if (numOfNonWhiteSpace >= 4){return;}
                if (Tetrominos[TetroNum][i][j] != ' '){
                    numOfNonWhiteSpace++;
                    disp = Tetrominos[TetroNum][i][j];                      
                    vertexes[numOfNonWhiteSpace-1] = {i, j};
                }
            }
        }
    }
};

// Get input without pausing
char getInput(){
    if (_kbhit()){
        return (_getch());
    }
    return 0;
}

void movePiece(Tertromino * t){
    // Clearing where the piece was
    int targx = t->x, targy = t->y;
    for (int i = 0; i < 4; i++){
        GameBoard[t->vertexes[i].y + t->y][t->vertexes[i].x + t->x] = ' ';
    }
    /* Proccess the tetronimo */
    // Move it down every period of time
    if (clock() - TetrisTime > 750){
        targy++;
        TetrisTime = clock();
    }
    // Input
    fflush(stdin);
    char input = getInput();
    switch(input){
        case 's':
            if (targy == t->y){ // If the tetris timer didnt go off
                targy++;
            }
            break;
        case 'a':
            targx--;
            break;
        case 'd':
            targx++;
            break;
        case 'r': // rotate (-y, x)
            {
            auto vtemp = t->vertexes;
            for (int i = 0; i < 4; i++){ // Rotate the vertexes 90 degrees
                auto temp = t->vertexes[i].x;
                t->vertexes[i].x = -t->vertexes[i].y; 
                t->vertexes[i].y = temp;
            }
            for (int i = 0; i < 4; i++){
                if (GameBoard[t->vertexes[i].y + t->y][t->vertexes[i].x + t->x] != ' '){ // Rotation causes collision
                    for (int k = 0; k < 4; k++){
                        t->vertexes[k].x = vtemp[k].x;
                        t->vertexes[k].y = vtemp[k].y;
                    }
                }
            }}
            break;
        case 'q':
            running = false;
            break;
    }
    /* Collision */
    if (targx != t->x){ // Moved horizontally
        bool collided = false;
        for (int i = 0; i < 4; i++){
            if (GameBoard[t->vertexes[i].y + targy][t->vertexes[i].x + targx] != ' '){
                collided = true;
                break;
            }
        }
        if (!collided){
            t->x = targx;
        }
    }
    if (targy != t->y){ // Moved vertically
        bool collided = false;
        for (int i = 0; i < 4; i++){
            if (GameBoard[t->vertexes[i].y + targy][t->vertexes[i].x + t->x] != ' '){
                collided = true;
                pieceSetDown = true; // The piece has hit the ground
                break;
            }
        }
        if (!collided){
            t->y = targy;
        }
    }
    
    // Draw the tetronimo to the screen
    for (int i = 0; i < 4; i++){
        GameBoard[t->vertexes[i].y + t->y][t->vertexes[i].x + t->x] = t->disp;
    }
}

void ClearFullLines(){
    for (int i = 14; i > 0; i--){
        bool full = true;
        for (int j = 0; j < 20; j++){
            if (GameBoard[i][j] == ' '){
                full = false;
                break;
            }
        }
        if (full){ // Full line
            for (int j = i; j > 0; j--){
                auto temp = GameBoard[j];
                for (int k = 0; k < 20; k++){
                    GameBoard[j][k] = GameBoard[j-1][k];
                    GameBoard[j-1][k] = temp[k];
                } 
            }
            for (int k = 1; k < 19; k++){
                GameBoard[1][k] = ' ';
            }

        }
    }
}

int main(void){
    // Hiding the cursor
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo); // Get console cursor information
    CursorInfo.bVisible = false; // Hide the console cursor
    SetConsoleCursorInfo(handle, &CursorInfo); // Set the console cursor state
    
    TetrisTime = clock();

    srand(time(NULL));
    Tertromino * CurrentTetronimo = new Tertromino(rand()%7);

    while(running){ // Game loop
        movePiece(CurrentTetronimo);
        if (pieceSetDown){
            delete CurrentTetronimo;
            CurrentTetronimo = new Tertromino(rand()%7);
            ClearFullLines();
            for (int i = 0; i < 4; i++){ // If at the top and colliding with a piece
                if (GameBoard[CurrentTetronimo->vertexes[i].y + CurrentTetronimo->y][CurrentTetronimo->vertexes[i].x + CurrentTetronimo->x] != ' '){
                    running = false;
                    break;
                } 
            }
            pieceSetDown = false;
        }
        //qClearFullLines();
        for (int i = 0; i < 16; i++){ // Outputting to the console
            for (int j = 0; j < 20; j++){
                switch (GameBoard[i][j]){
                    case 'X':
                        setConsoleColour(BRIGHT_BLUE);
                        break;
                    case 'O':
                        setConsoleColour(DARK_BLUE);
                        break;
                    case 'G':
                        setConsoleColour(BRIGHT_PINK);
                        break;
                    case 'U':
                        setConsoleColour(BRIGHT_RED);
                        break;
                    case 'H':
                        setConsoleColour(BRIGHT_GREEN);
                        break;
                    case 'S':
                        setConsoleColour(BRIGHT_YELLOW);
                        break;
                    case 'K':
                        setConsoleColour(WHITE);
                        break;
                    default:
                        setConsoleColour(DARK);
                        break;
                }
                std::cout << (char)(219) << (char)(219); // Two cause horizontal is half of vertical
            }
            std::cout << std::endl;
        }
        ClearScreen();
    }
}
