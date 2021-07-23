#include <iostream>
#include <windows.h>
#include <conio.h>

bool running = true;
unsigned int TetrisTime = 0;

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

char GameBoard[16][20] = { // The game board where the pieces will be displayed
    "###################",
    "#                 #",
    "#                 #",
    "#                 #",
    "#                 #",
    "#                 #",
    "#                 #",
    "#                 #",
    "#                 #",
    "# G               #",
    "#GG             KK#",
    "#XGU             K#",
    "#XUU           OOK#",
    "#XUHH          OSS#",
    "#XHH           OSS#",
    "###################"
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
    if (TetrisTime >= 50){
        targy++;
        TetrisTime = 0;
    }
    // Input
    char input = getInput();
    switch(input){
        case 's':
            targy++;
            break;
        case 'a':
            targx--;
            break;
        case 'd':
            targx++;
            break;
        case 'q':
            running = false;
            break;
    }
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
    if (targy != t->y){
        bool collided = false;
        for (int i = 0; i < 4; i++){
            if (GameBoard[t->vertexes[i].y + targy][t->vertexes[i].x + targx] != ' '){
                collided = true;
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


int main(void){
    // Hiding the cursor
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo); // Get console cursor information
    CursorInfo.bVisible = false; // Hide the console cursor
    SetConsoleCursorInfo(handle, &CursorInfo); // Set the console cursor state
    
    auto CurrentTetronimo = new Tertromino(6);

    while(running){ // Game loop
        TetrisTime++;
        movePiece(CurrentTetronimo);
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
