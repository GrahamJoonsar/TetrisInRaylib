#include <iostream>
#include <windows.h>
#include <conio.h>


void ClearScreen()
{
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

struct Tertromino{

};

void movePiece(){

}

int main(void){
    // Hiding the cursor
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo); // Get console cursor information
    CursorInfo.bVisible = false; // Hide the console cursor
    SetConsoleCursorInfo(handle, &CursorInfo); // Set the console cursor state

    for(;;){ // Game loop
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
                }
                std::cout << (char)(219) << (char)(219);
            }
            std::cout << std::endl;
        }
        ClearScreen();
    }
}
